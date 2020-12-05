#pragma once

struct view_factory
	: winrt::implements<view_factory, winrt::Windows::ApplicationModel::Core::IFrameworkViewSource>
{
	explicit view_factory(const winrt::Windows::ApplicationModel::Core::IFrameworkView& framework_view) noexcept;

	view_factory(const view_factory& other) noexcept = default;
	view_factory(view_factory&& other) noexcept = default;

	~view_factory() noexcept = default;

	view_factory& operator= (const view_factory& other) noexcept = default;
	view_factory& operator= (view_factory&& other) noexcept = default;

	winrt::Windows::ApplicationModel::Core::IFrameworkView CreateView() const noexcept;

private:
	const winrt::Windows::ApplicationModel::Core::IFrameworkView& view;
};