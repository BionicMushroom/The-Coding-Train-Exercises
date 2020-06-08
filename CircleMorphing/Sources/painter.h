#pragma once

class painter final
{
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
};
