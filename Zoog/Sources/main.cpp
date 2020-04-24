#include "pch.h"

#include "main_view.h"
#include "view_factory.h"

using namespace winrt::Windows::ApplicationModel;

int WINAPI wWinMain([[maybe_unused]] HINSTANCE instance, [[maybe_unused]] HINSTANCE previous_instance,
	[[maybe_unused]] PWSTR command_line, [[maybe_unused]] int show_command)
{
	Core::CoreApplication::Run(winrt::make<view_factory>(winrt::make<main_view>()));
	return 0;
}