#include "pch.h"
#include "painter.h"

using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Composition;

void painter::create_and_append_sprite_shape(const CompositionGeometry& geometry, std::uint8_t stroke_color, std::uint8_t fill_color) noexcept
{
	const auto compositor{ root_visual.Compositor() };

	const auto sprite{ compositor.CreateSpriteShape(geometry) };
	sprite.StrokeBrush(compositor.CreateColorBrush({ 255, stroke_color, stroke_color, stroke_color }));
	sprite.FillBrush(compositor.CreateColorBrush({ 255, fill_color, fill_color, fill_color }));

	root_visual.Shapes().Append(sprite);
}

void painter::resize_lines(const float2& scale_factor) const noexcept
{
	for (const auto& line : lines)
	{
		line.Start(line.Start() * scale_factor);
		line.End(line.End() * scale_factor);
	}
}

void painter::resize_rectangles(const float2& scale_factor) const noexcept
{
	for (const auto& rectangle : rectangles)
	{
		rectangle.Offset(rectangle.Offset() * scale_factor);
		rectangle.Size(rectangle.Size() * scale_factor);
	}
}

void painter::resize_ellipses(const float2& scale_factor) const noexcept
{
	for (const auto& ellipse : ellipses)
	{
		ellipse.Center(ellipse.Center() * scale_factor);
		ellipse.Radius(ellipse.Radius() * scale_factor);
	}
}

void painter::create_render_target_and_root_visual() noexcept
{
	const Compositor compositor{};

	render_target = compositor.CreateTargetForCurrentView();
	root_visual = compositor.CreateShapeVisual();
	
	render_target.Root(root_visual);
}

void painter::draw_line(const float2& start, const float2& end, std::uint8_t stroke_color) noexcept
{
	const auto& line{ lines.emplace_back(render_target.Compositor().CreateLineGeometry()) };
	line.Start(start);
	line.End(end);

	create_and_append_sprite_shape(line, stroke_color, stroke_color);
}

void painter::draw_rectangle(const float2& offset, const float2& size, std::uint8_t stroke_color, std::uint8_t fill_color) noexcept
{
	const auto& rectangle{ rectangles.emplace_back(render_target.Compositor().CreateRectangleGeometry()) };
	rectangle.Offset(offset);
	rectangle.Size(size);

	create_and_append_sprite_shape(rectangle, stroke_color, fill_color);
}

void painter::draw_ellipse(const float2& center, const float2& radius, std::uint8_t stroke_color, std::uint8_t fill_color) noexcept
{
	const auto& ellipse{ ellipses.emplace_back(render_target.Compositor().CreateEllipseGeometry()) };
	ellipse.Center(center);
	ellipse.Radius(radius);

	create_and_append_sprite_shape(ellipse, stroke_color, fill_color);
}

void painter::resize(const float2& window_size) noexcept
{
	root_visual.Size(window_size);

	const auto scale_factor{ window_size / previous_window_size };

	resize_lines(scale_factor);
	resize_rectangles(scale_factor);
	resize_ellipses(scale_factor);

	previous_window_size = window_size;
}
