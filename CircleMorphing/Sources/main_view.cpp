#include "pch.h"
#include "main_view.h"

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::ViewManagement;

namespace helpers
{
	namespace details
	{
		static auto customize_title_bar_buttons(const ApplicationViewTitleBar& title_bar) noexcept
		{
			title_bar.ButtonBackgroundColor(painter::background_color());
			title_bar.ButtonForegroundColor(painter::circle_original_color());

			title_bar.ButtonHoverBackgroundColor(painter::circle_transitioned_color());
			title_bar.ButtonHoverForegroundColor(painter::background_color());

			title_bar.ButtonInactiveBackgroundColor(painter::background_color());
			title_bar.ButtonInactiveForegroundColor(painter::circle_transitioned_color());

			title_bar.ButtonPressedBackgroundColor(painter::circle_original_color());
			title_bar.ButtonPressedForegroundColor(painter::background_color());
		}
	}

	static auto customize_title_bar() noexcept
	{
		CoreApplication::GetCurrentView().TitleBar().ExtendViewIntoTitleBar(true);
		details::customize_title_bar_buttons(ApplicationView::GetForCurrentView().TitleBar());
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
	helpers::customize_title_bar();
	window.SizeChanged({ this, &main_view::on_size_changed });
}

void main_view::prepare_painter(const Rect& window_bounds) noexcept
{
	painter.create_shapes();
	painter.resize({ window_bounds.Width, window_bounds.Height });
}

void main_view::on_size_changed([[maybe_unused]] const CoreWindow& window, const WindowSizeChangedEventArgs& arguments) noexcept
{
	painter.resize(arguments.Size());
}
