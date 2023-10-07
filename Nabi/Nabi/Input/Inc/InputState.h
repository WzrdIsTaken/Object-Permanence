#pragma once
#include "EngineCore.h"

namespace nabi::Input
{
	enum class InputState : int
	{
		Invalid,

		Up,        // Key is not being pressed (continuous)
		Pressed,   // Key has been pressed     (one shot)
		Held,      // Key is being held down   (continuous)
		Released,  // Key has been released    (one shot)

		ENUM_COUNT
	};
} // namespace nabi::Input
