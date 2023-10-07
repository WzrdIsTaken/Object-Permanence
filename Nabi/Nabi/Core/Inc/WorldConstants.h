#pragma once
#include "EngineCore.h"

#include "DirectXIncludes.h"

namespace nabi::WorldConstants
{
	dx::XMFLOAT3 constexpr c_Up      = { 0.0f, 1.0f, 0.0f };
	dx::XMFLOAT3 constexpr c_Forward = { 0.0f, 0.0f, 1.0f };

	dx::XMFLOAT3 constexpr c_Gravity = { 0.0f, -9.8f, 0.0f };
} // namespace nabi::WorldConstants
