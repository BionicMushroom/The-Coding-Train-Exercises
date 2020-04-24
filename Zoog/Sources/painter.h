#pragma once

class painter final
{
	winrt::Windows::Foundation::Numerics::float2 previous_window_size{ ideal_window_size() };

	winrt::Windows::UI::Composition::CompositionTarget render_target{ nullptr };
	winrt::Windows::UI::Composition::ShapeVisual root_visual{ nullptr };

	std::vector<winrt::Windows::UI::Composition::CompositionLineGeometry> lines;
	std::vector<winrt::Windows::UI::Composition::CompositionRectangleGeometry> rectangles;
	std::vector<winrt::Windows::UI::Composition::CompositionEllipseGeometry> ellipses;

	void create_and_append_sprite_shape(
		const winrt::Windows::UI::Composition::CompositionGeometry& geometry,
		std::uint8_t stroke_color,
		std::uint8_t fill_color) noexcept;

	void resize_lines(const winrt::Windows::Foundation::Numerics::float2& scale_factor) const noexcept;
	void resize_rectangles(const winrt::Windows::Foundation::Numerics::float2& scale_factor) const noexcept;
	void resize_ellipses(const winrt::Windows::Foundation::Numerics::float2& scale_factor) const noexcept;

public:
	void create_render_target_and_root_visual() noexcept;

	void draw_line(
		const winrt::Windows::Foundation::Numerics::float2& start,
		const winrt::Windows::Foundation::Numerics::float2& end,
		std::uint8_t stroke_color) noexcept;

	void draw_rectangle(
		const winrt::Windows::Foundation::Numerics::float2& offset,
		const winrt::Windows::Foundation::Numerics::float2& size,
		std::uint8_t stroke_color,
		std::uint8_t fill_color) noexcept;

	void draw_ellipse(
		const winrt::Windows::Foundation::Numerics::float2& center,
		const winrt::Windows::Foundation::Numerics::float2& radius,
		std::uint8_t stroke_color,
		std::uint8_t fill_color) noexcept;

	void resize(const winrt::Windows::Foundation::Numerics::float2& window_size) noexcept;

	[[nodiscard]] constexpr static winrt::Windows::Foundation::Numerics::float2 ideal_window_size() noexcept
	{
		return { 2560.f, 1408.f };
	}
};
