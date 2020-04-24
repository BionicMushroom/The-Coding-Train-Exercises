#pragma once
#include "painter.h"

struct main_view
	: winrt::implements<main_view, winrt::Windows::ApplicationModel::Core::IFrameworkView>
{
	void Initialize(const winrt::Windows::ApplicationModel::Core::CoreApplicationView& application_view) const noexcept;
	void Load(const winrt::hstring& entry_point) const noexcept;
	void Run() const noexcept;
	void SetWindow(const winrt::Windows::UI::Core::CoreWindow& window) noexcept;
	void Uninitialize() const noexcept;

private:
	void prepare_window(const winrt::Windows::UI::Core::CoreWindow& window) noexcept;
	void prepare_painter(const winrt::Windows::Foundation::Rect& window_bounds) noexcept;

	void draw() noexcept;
	void draw_background() noexcept;
	void draw_body() noexcept;
	void draw_legs() noexcept;
	void draw_head() noexcept;
	void draw_eyes() noexcept;

	void on_size_changed(
		const winrt::Windows::UI::Core::CoreWindow& window,
		const winrt::Windows::UI::Core::WindowSizeChangedEventArgs& arguments) noexcept;

	painter painter;
};