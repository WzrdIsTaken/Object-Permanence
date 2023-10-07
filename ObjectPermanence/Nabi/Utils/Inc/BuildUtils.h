#pragma once
#include "EngineCore.h"

namespace nabi::BuildUtils
{
	enum class BuildConfiguration
	{
		Debug,
		Release,
		// Might add an, eg, ReleaseFinal in the future
		ENUM_COUNT
	};

	// Holds the build configuration of the project
	// Can sneakily be changed in debug mode for tests
#ifndef USE_DEBUG_UTILS
	constexpr
#else
	inline
#endif // ifndef USE_DEBUG_UTILS
		BuildConfiguration c_BuildConfiguration =
#ifdef _DEBUG
			BuildConfiguration::Debug;
#else
			BuildConfiguration::Release;
#endif // ifdef _DEBUG
} // namespace nabi::BuildUtils
