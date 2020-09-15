#pragma once

class painter final
{
	constexpr static winrt::Windows::UI::Color mint{ 0xFF, 0xAD, 0xEF, 0xD1 };
	constexpr static winrt::Windows::UI::Color pacific_blue{ 0xFF, 0x1C, 0xA9, 0xC9 };
	constexpr static winrt::Windows::UI::Color sailor_blue{ 0xFF, 0x00, 0x20, 0x3F };

	winrt::Windows::UI::Composition::CompositionTarget render_target{ nullptr };
	winrt::Windows::UI::Composition::ShapeVisual root_visual{ nullptr };

	winrt::Windows::UI::Composition::CompositionRectangleGeometry background{ nullptr };
	winrt::Windows::UI::Composition::CompositionEllipseGeometry circle{ nullptr };

	void create_render_target_and_root_visual() noexcept;
	void create_background() noexcept;
	void create_circle() noexcept;

	winrt::Windows::UI::Composition::CompositionSpriteShape create_and_append_sprite_shape(
		const winrt::Windows::UI::Composition::CompositionGeometry& geometry,
		const winrt::Windows::UI::Color& color) const noexcept;

	void resize_root_visual_and_background(const winrt::Windows::Foundation::Numerics::float2& window_size) const noexcept;
	void resize_circle(const winrt::Windows::Foundation::Numerics::float2& window_size) const noexcept;

public:
	void create_shapes() noexcept;
	void resize(const winrt::Windows::Foundation::Numerics::float2& window_size) const noexcept;

	[[nodiscard]] constexpr static winrt::Windows::UI::Color circle_original_color() noexcept
	{
		return mint;
	}

	[[nodiscard]] constexpr static winrt::Windows::UI::Color circle_transitioned_color() noexcept
	{
		return pacific_blue;
	}

	[[nodiscard]] constexpr static winrt::Windows::UI::Color background_color() noexcept
	{
		return sailor_blue;
	}
};
