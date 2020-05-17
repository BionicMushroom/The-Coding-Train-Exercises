#include "pch.h"
#include "view_factory.h"

using namespace winrt::Windows::ApplicationModel;

view_factory::view_factory(const winrt::Windows::ApplicationModel::Core::IFrameworkView& framework_view) noexcept
	: view{ framework_view }
{
}

Core::IFrameworkView view_factory::CreateView() const noexcept
{
	return view;
}
