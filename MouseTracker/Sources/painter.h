#pragma once

class painter final
{
	winrt::com_ptr<ID2D1Factory1> factory;
	winrt::com_ptr<ID2D1DeviceContext> device_context;
	winrt::com_ptr<ID2D1LinearGradientBrush> red_brush;
	winrt::com_ptr<ID2D1LinearGradientBrush> blue_brush;
	winrt::com_ptr<IDXGISwapChain1> swap_chain;

	D2D1_POINT_2F previous_line_begin{ D2D1::Point2F() };
	D2D1_POINT_2F previous_line_end{ D2D1::Point2F() };
	float previous_line_stroke_width{ 1.f };

	void create_factory() noexcept;
	void create_device_dependent_resources(const winrt::Windows::UI::Core::CoreWindow& window);

	void set_previous_line_information(const D2D1_POINT_2F& begin, const D2D1_POINT_2F& end, float stroke_width) noexcept;

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
	void resize(const winrt::Windows::Foundation::Rect& window_bounds) noexcept;

	void draw_background() noexcept;

	void draw_line(
		const winrt::Windows::Foundation::Point& begin,
		const winrt::Windows::Foundation::Point& end,
		float stroke_width) noexcept;
};
