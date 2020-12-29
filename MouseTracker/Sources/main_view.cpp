#include "pch.h"
#include "main_view.h"

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::ViewManagement;

namespace constants
{
	constexpr static auto maximum_stroke_width_divergence{ 0.8f };
}

namespace helpers
{
	static void customize_title_bar() noexcept
	{
		const auto title_bar{ ApplicationView::GetForCurrentView().TitleBar() };

		const auto background_color{ Colors::Black() };
		const auto foreground_color{ Colors::White() };

		const auto inactive_background_color{ Colors::Black() };
		const auto inactive_foreground_color{ Colors::DimGray() };

		title_bar.BackgroundColor(background_color);
		title_bar.ForegroundColor(foreground_color);

		title_bar.InactiveBackgroundColor(inactive_background_color);
		title_bar.InactiveForegroundColor(inactive_foreground_color);

		title_bar.ButtonBackgroundColor(background_color);
		title_bar.ButtonForegroundColor(foreground_color);

		title_bar.ButtonInactiveBackgroundColor(inactive_background_color);
		title_bar.ButtonInactiveForegroundColor(inactive_foreground_color);
	}

	static auto distance(const Point& point_1, const Point& point_2) noexcept
	{
		return std::hypot(point_2.X - point_1.X, point_2.Y - point_1.Y);
	}
}

void main_view::Initialize([[maybe_unused]] const CoreApplicationView& application_view) const noexcept
{
}

void main_view::Load([[maybe_unused]] const winrt::hstring& entry_point) const noexcept
{
}

void main_view::Run() const noexcept
{
	const auto window{ CoreWindow::GetForCurrentThread() };

	window.Activate();
	window.Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
}

void main_view::SetWindow(const CoreWindow& window) noexcept
{
	prepare_window(window);
	prepare_painter(window);
}

void main_view::Uninitialize() const noexcept
{
}

void main_view::prepare_window(const CoreWindow& window) noexcept
{
	helpers::customize_title_bar();

	window.PointerEntered({ this, &main_view::on_pointer_entered });
	window.PointerMoved({ this, &main_view::on_pointer_moved });

	window.ResizeStarted({ this, &main_view::on_resize_started });
	window.ResizeCompleted({ this, &main_view::on_resize_completed });
	window.SizeChanged({ this, &main_view::on_size_changed });
}

void main_view::prepare_painter(const CoreWindow& window) noexcept
{
	painter.create_resources(window);
	painter.draw_background();
}

float main_view::smooth_stroke_width(const Point& current_pointer_position) const noexcept
{
	return std::clamp(
		helpers::distance(previous_pointer_position, current_pointer_position),
		previous_stroke_width - constants::maximum_stroke_width_divergence,
		previous_stroke_width + constants::maximum_stroke_width_divergence
	);
}

void main_view::on_pointer_entered([[maybe_unused]] const CoreWindow& window, const PointerEventArgs& arguments) noexcept
{
	previous_pointer_position = arguments.CurrentPoint().Position();
}

void main_view::on_pointer_moved([[maybe_unused]] const CoreWindow& window, const PointerEventArgs& arguments) noexcept
{
	const auto current_pointer_position{ arguments.CurrentPoint().Position() };
	const auto stroke_width{ smooth_stroke_width(current_pointer_position) };

	painter.draw_line(previous_pointer_position, current_pointer_position, stroke_width);

	previous_pointer_position = current_pointer_position;
	previous_stroke_width = stroke_width;
}

void main_view::on_resize_started([[maybe_unused]] const CoreWindow& window, [[maybe_unused]] const IInspectable& inspectable) noexcept
{
	is_resizing = true;
}

void main_view::on_resize_completed(const CoreWindow& window, [[maybe_unused]] const IInspectable& inspectable) noexcept
{
	is_resizing = false;
	painter.resize(window.Bounds());
}

void main_view::on_size_changed(const CoreWindow& window, [[maybe_unused]] const WindowSizeChangedEventArgs& arguments) noexcept
{
	if (!is_resizing)
		painter.resize(window.Bounds());
}
