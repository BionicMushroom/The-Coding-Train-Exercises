#include "pch.h"
#include "main_view.h"

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Core;

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
	painter.create_shapes();
	painter.resize({ window_bounds.Width, window_bounds.Height });
}

void main_view::on_size_changed([[maybe_unused]] const CoreWindow& window, const WindowSizeChangedEventArgs& arguments) noexcept
{
	painter.resize(arguments.Size());
}
