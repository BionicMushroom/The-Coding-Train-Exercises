#pragma once

class painter final
{
	winrt::com_ptr<ID2D1Factory1> factory;
	winrt::com_ptr<ID2D1DeviceContext> device_context;
	winrt::com_ptr<ID2D1SolidColorBrush> previous_ellipse_brush;
	winrt::com_ptr<IDXGISwapChain1> swap_chain;

	D2D1_ELLIPSE previous_ellipse{ D2D1::Ellipse(D2D1::Point2F(), 0.f, 0.f) };

	void create_factory() noexcept;
	void create_device_dependent_resources(const winrt::Windows::UI::Core::CoreWindow& window);

	void resize_resources() noexcept;
	void draw_background() noexcept;

	void set_previous_ellipse_information(const D2D1_ELLIPSE& ellipse, const D2D1_COLOR_F& color) noexcept;

	template<typename function>
	bool recreate_device_dependent_resources_if_needed(const function& f) noexcept try
	{
		f();
		return false;
	}
	catch (const winrt::hresult_error& err)
	{
		if (const auto result{ err.code() }; result == D2DERR_RECREATE_TARGET || result == DXGI_ERROR_DEVICE_RESET || result == DXGI_ERROR_DEVICE_REMOVED)
		{
			swap_chain = nullptr;
			create_device_dependent_resources(winrt::Windows::UI::Core::CoreWindow::GetForCurrentThread());

			return true;
		}

		std::terminate(); // simply re-throwing the exception causes a compiler warning
	}

	template<typename drawing_function>
	void draw(const drawing_function& function) noexcept
	{
		const auto drawing_function{
			[&] {
				device_context->BeginDraw();
				
				function();

				winrt::check_hresult(device_context->EndDraw());
				winrt::check_hresult(swap_chain->Present(0, 0));
			}
		};

		if (recreate_device_dependent_resources_if_needed(drawing_function))
			drawing_function();
	}

public:
	void create_resources(const winrt::Windows::UI::Core::CoreWindow& window) noexcept;
	void resize() noexcept;

	void draw_ellipse(
		const winrt::Windows::Foundation::Point& center,
		const winrt::Windows::Foundation::Numerics::float2& radius,
		const winrt::Windows::UI::Color& color) noexcept;
};
