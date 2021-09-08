#pragma once

namespace build_configuration
{
#ifdef _DEBUG
	constexpr inline auto is_debug{ true };
#else
	constexpr inline auto is_debug{ false };
#endif // _DEBUG
}