#include "pch.h"
#include "painter.h"
#include "build_configuration.h"

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::Graphics::Display;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;

namespace helpers
{
	namespace details
	{
		[[nodiscard]] static auto create_d3d_device() noexcept
		{
			constexpr auto core_flags{ D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT };
			constexpr auto debugging_flags{ D3D11_CREATE_DEVICE_DEBUG }; // D3D11_CREATE_DEVICE_DEBUGGABLE, while better, seems to fail every time on Nvidia drivers

			winrt::com_ptr<ID3D11Device> device;

			auto failed_with_debugging_flags{ false };
			auto failed_with_hardware_driver{ false };
			HRESULT result{};

			do
			{
				const auto driver_type{ failed_with_hardware_driver ? D3D_DRIVER_TYPE_WARP : D3D_DRIVER_TYPE_HARDWARE };
				const auto flags{ failed_with_debugging_flags || !build_configuration::is_debug ? core_flags : (core_flags | debugging_flags) };
				constexpr auto feature_level{ D3D_FEATURE_LEVEL_11_1 };

				result = D3D11CreateDevice(nullptr, driver_type, nullptr, flags, &feature_level, 1, D3D11_SDK_VERSION, device.put(), nullptr, nullptr);

				if (FAILED(result))
				{
					if (result == E_INVALIDARG)
					{
						if (failed_with_hardware_driver)
							winrt::throw_hresult(result);

						failed_with_hardware_driver = true;
					}
					else if (result == DXGI_ERROR_SDK_COMPONENT_MISSING)
					{
						if (failed_with_debugging_flags)
							winrt::throw_hresult(result);

						failed_with_debugging_flags = true;
					}
					else
						winrt::throw_hresult(result);
				}

			} while (FAILED(result));

			return device;
		}

		[[nodiscard]] static auto create_d2d_device(ID2D1Factory1& factory, IDXGIDevice1& dxgi_device)
		{
			winrt::com_ptr<ID2D1Device> device;
			winrt::check_hresult(factory.CreateDevice(&dxgi_device, device.put()));

			return device;
		}
	}

	[[nodiscard]] static auto create_d2d_factory() noexcept
	{
		winrt::com_ptr<ID2D1Factory1> factory;
		const D2D1_FACTORY_OPTIONS factory_options{ build_configuration::is_debug ? D2D1_DEBUG_LEVEL_INFORMATION : D2D1_DEBUG_LEVEL_NONE };

		winrt::check_hresult(
			D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factory_options, factory.put())
		);

		return factory;
	}

	[[nodiscard]] static auto create_d2d_device_context(ID2D1Factory1& factory, IDXGIDevice1& dxgi_device)
	{
		winrt::com_ptr<ID2D1DeviceContext> device_context;

		winrt::check_hresult(
			details::create_d2d_device(factory, dxgi_device)->CreateDeviceContext(
				D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, device_context.put())
		);

		return device_context;
	}

	[[nodiscard]] static auto create_dxgi_device()
	{
		return details::create_d3d_device().as<IDXGIDevice1>();
	}

	[[nodiscard]] static auto create_swap_chain(IDXGIDevice1& dxgi_device, const CoreWindow& window)
	{
		winrt::com_ptr<IDXGIAdapter> adapter;
		winrt::check_hresult(dxgi_device.GetAdapter(adapter.put()));

		winrt::com_ptr<IDXGIFactory2> factory;
		factory.capture(adapter, &IDXGIAdapter::GetParent);

		DXGI_SWAP_CHAIN_DESC1 swap_chain_description{};
		swap_chain_description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swap_chain_description.Stereo = false;
		swap_chain_description.SampleDesc.Count = 1;
		swap_chain_description.SampleDesc.Quality = 0;
		swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_description.BufferCount = 2;
		swap_chain_description.Scaling = DXGI_SCALING_NONE;
		swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swap_chain_description.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		winrt::com_ptr<IDXGISwapChain1> swap_chain;

		winrt::check_hresult(
			factory->CreateSwapChainForCoreWindow(&dxgi_device, winrt::get_unknown(window),
				&swap_chain_description, nullptr, swap_chain.put())
		);

		return swap_chain;
	}

	[[nodiscard]] static auto create_brush(ID2D1DeviceContext& device_context)
	{
		winrt::com_ptr<ID2D1SolidColorBrush> brush;

		winrt::check_hresult(
			device_context.CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), brush.put())
		);

		return brush;
	}

	static auto set_render_target(ID2D1DeviceContext& device_context, IDXGISwapChain1& swap_chain)
	{
		winrt::com_ptr<IDXGISurface> back_buffer;
		back_buffer.capture(&swap_chain, &IDXGISwapChain1::GetBuffer, 0);

		const auto dpi{ DisplayInformation::GetForCurrentView().LogicalDpi() };
		
		const auto bitmap_properties{ D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE), dpi, dpi) };

		winrt::com_ptr<ID2D1Bitmap1> bitmap;

		winrt::check_hresult(
			device_context.CreateBitmapFromDxgiSurface(back_buffer.get(), bitmap_properties, bitmap.put())
		);

		device_context.SetTarget(bitmap.get());
		device_context.SetDpi(dpi, dpi);
	}

	static auto resize(IDXGISwapChain1& swap_chain)
	{
		winrt::check_hresult(swap_chain.ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));
	}

	[[nodiscard]] constexpr static auto d2d1_color_from(const Color& color) noexcept
	{
		return D2D1_COLOR_F{ color.R / 255.f, color.G / 255.f, color.B / 255.f, color.A / 255.f };
	}
}

void painter::create_factory() noexcept
{
	factory = helpers::create_d2d_factory();
}

void painter::create_device_dependent_resources(const CoreWindow& window)
{
	const auto dxgi_device{ helpers::create_dxgi_device() };
	device_context = helpers::create_d2d_device_context(*factory, *dxgi_device);

	previous_ellipse_brush = helpers::create_brush(*device_context);

	swap_chain = helpers::create_swap_chain(*dxgi_device, window);
	helpers::set_render_target(*device_context, *swap_chain);
}

void painter::resize_resources() noexcept
{
	recreate_device_dependent_resources_if_needed(
		[&] {
			device_context->SetTarget(nullptr);
			set_previous_ellipse_information(D2D1::Ellipse(D2D1::Point2F(), 0.f, 0.f), D2D1::ColorF(D2D1::ColorF::Black));

			helpers::resize(*swap_chain);
			helpers::set_render_target(*device_context, *swap_chain);
		}
	);
}

void painter::draw_background() noexcept
{
	draw([&]() noexcept { device_context->Clear(); });
}

void painter::set_previous_ellipse_information(const D2D1_ELLIPSE& ellipse, const D2D1_COLOR_F& color) noexcept
{
	previous_ellipse = ellipse;
	previous_ellipse_brush->SetColor(color);
}

void painter::create_resources(const CoreWindow& window) noexcept
{
	create_factory();
	recreate_device_dependent_resources_if_needed([&] { create_device_dependent_resources(window); });
}

void painter::resize() noexcept
{
	resize_resources();
	draw_background();
}

void painter::draw_ellipse(const Point& center, const float2& radius, const Color& color) noexcept
{
	draw(
		[&]() noexcept {
			/*
				We need to draw the previous ellipse in the current swap chain buffer
				as it has been drawn only in the previous swap chain buffer.
			*/

			device_context->FillEllipse(previous_ellipse, previous_ellipse_brush.get());

			set_previous_ellipse_information(
				D2D1::Ellipse(D2D1::Point2F(center.X, center.Y), radius.x, radius.y), helpers::d2d1_color_from(color));
			
			device_context->FillEllipse(previous_ellipse, previous_ellipse_brush.get());
		}
	);
}
