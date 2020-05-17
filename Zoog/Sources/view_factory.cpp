#include "pch.h"
#include "view_factory.h"

using namespace winrt::Windows::ApplicationModel::Core;

view_factory::view_factory(const IFrameworkView& framework_view) noexcept
	: view{ framework_view }
{
}

IFrameworkView view_factory::CreateView() const noexcept
{
	return view;
}
