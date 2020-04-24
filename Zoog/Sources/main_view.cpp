#include "pch.h"
#include "main_view.h"

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::UI::Core;

namespace constants
{
	namespace body
	{
		constexpr float2 size{ 110.f, 500.f };
		constexpr float2 offset{ painter::ideal_window_size().x / 2.f - size.x / 2.f, painter::ideal_window_size().y / 2.f - size.y / 2.f };
		constexpr float maximum_distance_y{ size.y / 8.f };
	}
	
	namespace color
	{
		constexpr std::uint8_t black{ 0 };
		constexpr std::uint8_t gray{ 127 };
		constexpr std::uint8_t white{ 255 };
	}

	namespace head
	{		
		constexpr float2 radius{ body::size.x * 1.3f, body::size.x * 1.3f };
		constexpr float2 center{ body::offset.x + body::size.x / 2.f, body::offset.y + radius.y - body::maximum_distance_y };
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
	prepare_painter(window.Bounds());
}

void main_view::Uninitialize() const noexcept
{
}

void main_view::prepare_window(const CoreWindow& window) noexcept
{
	window.SizeChanged({ this, &main_view::on_size_changed });
}

void main_view::prepare_painter(const Rect& window_bounds) noexcept
{
	painter.create_render_target_and_root_visual();
	draw();
	painter.resize({ window_bounds.Width, window_bounds.Height });
}

void main_view::draw() noexcept
{
	draw_background();

	draw_body();
	draw_legs();
	draw_head();
	draw_eyes();
}

void main_view::draw_background() noexcept
{
	painter.draw_rectangle({ 0.f, 0.f }, painter::ideal_window_size(), constants::color::white, constants::color::white);
}

void main_view::draw_body() noexcept
{
	painter.draw_rectangle(constants::body::offset, constants::body::size, constants::color::black, constants::color::gray);
}

void main_view::draw_legs() noexcept
{
	constexpr auto leg_end_offset{ constants::body::maximum_distance_y };

	float2 start{ constants::body::offset.x , constants::body::offset.y + constants::body::size.y };
	float2 end{ start.x - leg_end_offset, start.y + leg_end_offset };
	painter.draw_line(start, end, constants::color::black);

	start.x = constants::body::offset.x + constants::body::size.x;
	end.x = start.x + leg_end_offset;
	painter.draw_line(start, end, constants::color::black);
}

void main_view::draw_head() noexcept
{
	painter.draw_ellipse(constants::head::center, constants::head::radius, constants::color::black, constants::color::white);
}

void main_view::draw_eyes() noexcept
{
	constexpr float offset_from_head_margin{ constants::head::radius.x / 15.f };
	constexpr auto radius_x{ constants::head::radius.x / 3.4f };
	constexpr auto radius_y{ radius_x * 2 };
	constexpr float2 radius{ radius_x, radius_y };

	float2 center{ constants::head::center.x - constants::head::radius.x + radius.x + offset_from_head_margin , constants::head::center.y };
	painter.draw_ellipse(center, radius, constants::color::black, constants::color::black);

	center.x = constants::head::center.x + constants::head::radius.x - radius.x - offset_from_head_margin;
	painter.draw_ellipse(center, radius, constants::color::black, constants::color::black);
}

void main_view::on_size_changed([[maybe_unused]] const CoreWindow& window, const WindowSizeChangedEventArgs& arguments) noexcept
{
	painter.resize(arguments.Size());
}
