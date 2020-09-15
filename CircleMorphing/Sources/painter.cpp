#include "pch.h"
#include "painter.h"

using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Composition;

#define ANIMATED_RADIUS L"AnimatedRadius"
#define THIS_TARGET_ANIMATED_RADIUS L"this.Target." ANIMATED_RADIUS
// I know macros are ugly but unfortunately there is no easy way to implement and read constexpr strings in C++ 17.

namespace helpers
{
	namespace details
	{
		static auto set_common_animation_parameters(const KeyFrameAnimation& animation) noexcept
		{
			animation.Duration(std::chrono::seconds{ 10 });
			animation.IterationBehavior(AnimationIterationBehavior::Forever);
		}
	}

	static auto create_color_animation(const CompositionSpriteShape& sprite, const Color& from, const Color& to) noexcept
	{
		const auto animation{ sprite.Compositor().CreateColorKeyFrameAnimation() };

		animation.InsertKeyFrame(.5f, to);
		animation.InsertKeyFrame(1.f, from);

		details::set_common_animation_parameters(animation);
		sprite.FillBrush().StartAnimation(L"Color", animation);
	}

	static auto set_animated_radius(const CompositionEllipseGeometry& circle, const float2& radius) noexcept
	{
		circle.Properties().InsertVector2(ANIMATED_RADIUS, radius);
	}

	static auto create_size_animation(const CompositionEllipseGeometry& circle) noexcept
	{
		const auto animation{ circle.Compositor().CreateVector2KeyFrameAnimation() };

		animation.InsertExpressionKeyFrame(0.f, THIS_TARGET_ANIMATED_RADIUS);
		animation.InsertExpressionKeyFrame(.5f, L"Scale(" THIS_TARGET_ANIMATED_RADIUS L", 0.5f)");
		animation.InsertExpressionKeyFrame(1.f, THIS_TARGET_ANIMATED_RADIUS);

		details::set_common_animation_parameters(animation);

		set_animated_radius(circle, float2::zero());
		circle.StartAnimation(L"Radius", animation);
	}
}

void painter::create_render_target_and_root_visual() noexcept
{
	const Compositor compositor{};

	render_target = compositor.CreateTargetForCurrentView();
	root_visual = compositor.CreateShapeVisual();

	render_target.Root(root_visual);
}

void painter::create_background() noexcept
{
	background = root_visual.Compositor().CreateRectangleGeometry();
	create_and_append_sprite_shape(background, background_color());
}

void painter::create_circle() noexcept
{
	circle = root_visual.Compositor().CreateEllipseGeometry();

	helpers::create_color_animation(create_and_append_sprite_shape(circle, circle_original_color()), circle_original_color(), circle_transitioned_color());
	helpers::create_size_animation(circle);
}

CompositionSpriteShape painter::create_and_append_sprite_shape(const CompositionGeometry& geometry, const Color& color) const noexcept
{
	const auto compositor{ root_visual.Compositor() };

	const auto sprite{ compositor.CreateSpriteShape(geometry) };
	sprite.FillBrush(compositor.CreateColorBrush(color));

	root_visual.Shapes().Append(sprite);
	return sprite;
}

void painter::resize_root_visual_and_background(const float2& window_size) const noexcept
{
	root_visual.Size(window_size);
	background.Size(window_size);
}

void painter::resize_circle(const float2& window_size) const noexcept
{
	const auto window_size_halved{ window_size / 2.f };
	circle.Center(window_size_halved);

	const auto radius{ std::min(window_size_halved.x, window_size_halved.y) };
	helpers::set_animated_radius(circle, { radius, radius });
}

void painter::create_shapes() noexcept
{
	create_render_target_and_root_visual();
	create_background();
	create_circle();
}

void painter::resize(const float2& window_size) const noexcept
{
	resize_root_visual_and_background(window_size);
	resize_circle(window_size);
}
