#include "pch.h"
#include "main_view.h"

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::System;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::ViewManagement;

namespace constants
{
	constexpr auto min_allowed_radius{ 1.f };
}

namespace helpers
{
	static auto customize_title_bar() noexcept
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

	[[nodiscard]] static auto size_of(const CoreWindow& window) noexcept
	{
		const auto bounds{ window.Bounds() };
		return Size{ bounds.Width, bounds.Height };
	}

	[[nodiscard]] static auto next_float_after(float value) noexcept
	{
		return std::nextafter(value, std::numeric_limits<float>::max());
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
	set_sizes(helpers::size_of(window));

	prepare_window(window);
	prepare_painter(window);
	prepare_drawing_timer(window);
}

void main_view::Uninitialize() const noexcept
{
}

void main_view::prepare_window(const CoreWindow& window) noexcept
{
	helpers::customize_title_bar();

	window.ResizeStarted({ this, &main_view::on_resize_started });
	window.ResizeCompleted({ this, &main_view::on_resize_completed });
	window.SizeChanged({ this, &main_view::on_size_changed });
}

void main_view::prepare_painter(const CoreWindow& window) noexcept
{
	painter.create_resources(window);
}

void main_view::prepare_drawing_timer(const CoreWindow& window) noexcept
{
	drawing_timer = window.DispatcherQueue().CreateTimer();

	drawing_timer.Interval(std::chrono::milliseconds{ 250 });
	drawing_timer.Tick({ this, &main_view::on_drawing_timer_tick });

	drawing_timer.Start();
}

void main_view::resize(const Size& new_window_size) noexcept
{
	if (window_size != new_window_size)
	{
		set_sizes(new_window_size);
		painter.resize();
	}
}

void main_view::set_sizes(const Size& new_window_size) noexcept
{
	window_size = new_window_size;

	next_value_after_max_allowed_coordinates.X = 
		helpers::next_float_after(window_size.Width - constants::min_allowed_radius);

	next_value_after_max_allowed_coordinates.Y =
		helpers::next_float_after(window_size.Height - constants::min_allowed_radius);
}

Point main_view::random_coordinates() noexcept
{
	const std::uniform_real_distribution<float> distribution{ 
		constants::min_allowed_radius, next_value_after_max_allowed_coordinates.X };

	const std::uniform_real_distribution<float>::param_type param{ 
		constants::min_allowed_radius, next_value_after_max_allowed_coordinates.Y };

	return { distribution(random_engine), distribution(random_engine, param) };
}

float main_view::random_radius(const Point& coordinates) noexcept
{
	const auto max_allowed_radius{ std::max(
		std::min({ coordinates.X, coordinates.Y, window_size.Width - coordinates.X, window_size.Height - coordinates.Y }),
		constants::min_allowed_radius) };

	const std::uniform_real_distribution<float> distribution{ 
		constants::min_allowed_radius, helpers::next_float_after(max_allowed_radius) };

	return distribution(random_engine);
}

Color main_view::random_color() noexcept
{
	// can't have a uniform distribution of std::uint8_t
	const std::uniform_int_distribution<std::uint16_t> distribution{ 0, std::numeric_limits<std::uint8_t>::max() };

	return { static_cast<std::uint8_t>(distribution(random_engine)), static_cast<std::uint8_t>(distribution(random_engine)),
		static_cast<std::uint8_t>(distribution(random_engine)), static_cast<std::uint8_t>(distribution(random_engine)) };
}

void main_view::on_resize_started([[maybe_unused]] const CoreWindow& window, [[maybe_unused]] const IInspectable& inspectable) noexcept
{
	is_resizing = true;
	drawing_timer.Stop();
}

void main_view::on_resize_completed(const CoreWindow& window, [[maybe_unused]] const IInspectable& inspectable) noexcept
{
	is_resizing = false;
	resize(helpers::size_of(window));
	drawing_timer.Start();
}

void main_view::on_size_changed([[maybe_unused]] const CoreWindow& window, const WindowSizeChangedEventArgs& arguments) noexcept
{
	if (!is_resizing)
		resize(arguments.Size());
}

void main_view::on_drawing_timer_tick([[maybe_unused]] const DispatcherQueueTimer& timer, [[maybe_unused]] const IInspectable& inspectable) noexcept
{
	const auto coordinates{ random_coordinates() };
	const auto radius{ random_radius(coordinates) };

	painter.draw_ellipse(coordinates, { radius, radius }, random_color());
}
