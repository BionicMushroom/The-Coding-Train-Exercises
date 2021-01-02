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
	void prepare_painter(const winrt::Windows::UI::Core::CoreWindow& window) noexcept;
	void prepare_drawing_timer(const winrt::Windows::UI::Core::CoreWindow& window) noexcept;

	void resize(const winrt::Windows::Foundation::Size& new_window_size) noexcept;
	void set_sizes(const winrt::Windows::Foundation::Size& new_window_size) noexcept;

	[[nodiscard]] winrt::Windows::Foundation::Point random_coordinates() noexcept;
	[[nodiscard]] float random_radius(const winrt::Windows::Foundation::Point& coordinates) noexcept;
	[[nodiscard]] winrt::Windows::UI::Color random_color() noexcept;

	void on_resize_started(
		const winrt::Windows::UI::Core::CoreWindow& window,
		const winrt::Windows::Foundation::IInspectable& inspectable) noexcept;

	void on_resize_completed(
		const winrt::Windows::UI::Core::CoreWindow& window,
		const winrt::Windows::Foundation::IInspectable& inspectable) noexcept;

	void on_size_changed(
		const winrt::Windows::UI::Core::CoreWindow& window,
		const winrt::Windows::UI::Core::WindowSizeChangedEventArgs& arguments) noexcept;

	void on_drawing_timer_tick(
		const winrt::Windows::System::DispatcherQueueTimer& timer,
		const winrt::Windows::Foundation::IInspectable& inspectable) noexcept;

	painter painter;

	std::default_random_engine random_engine{ static_cast<std::default_random_engine::result_type>(
		std::chrono::system_clock::now().time_since_epoch().count()) };

	winrt::Windows::System::DispatcherQueueTimer drawing_timer{ nullptr };
	
	winrt::Windows::Foundation::Size window_size{};
	winrt::Windows::Foundation::Point next_value_after_max_allowed_coordinates{};

	bool is_resizing{ false };
};