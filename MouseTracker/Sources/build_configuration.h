#pragma once

namespace build_configuration
{
#ifdef _DEBUG
	constexpr auto is_debug{ true };
#else
	constexpr auto is_debug{ false };
#endif // _DEBUG
}