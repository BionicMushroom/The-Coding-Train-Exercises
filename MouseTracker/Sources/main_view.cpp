#include "pch.h"
#include "main_view.h"

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Core;

namespace constants
{
	constexpr static auto maximum_stroke_width_divergence{ 0.8f };
}

namespace helpers
{
	static auto distance(const Point& point_1, const Point& point_2) noexcept
	{
		return std::sqrt(std::pow(point_2.X - point_1.X, 2.f) + std::pow(point_2.Y - point_1.Y, 2.f));
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
	/*
		We could have used ResizeCompleted for improved efficiency but for some reason
		it doesn't get called when the window is maximized/minimized or when the DPI is changed.
	*/

	window.SizeChanged({ this, &main_view::on_size_changed });
	window.PointerEntered({ this, &main_view::on_pointer_entered });
	window.PointerMoved({ this, &main_view::on_pointer_moved });
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

void main_view::on_size_changed(const CoreWindow& window, [[maybe_unused]] const WindowSizeChangedEventArgs& arguments) noexcept
{
	painter.resize(window.Bounds());
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
