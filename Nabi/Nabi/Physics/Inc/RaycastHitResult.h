#pragma once
#include "EngineCore.h"

#include "entt.h"

namespace nabi::Physics
{
	struct RaycastHitResult final 
	{
		entt::entity m_Entity;
		float m_Distance;

		explicit constexpr inline operator bool() const NABI_NOEXCEPT
		{
			return m_Entity != entt::null;
		}
	};
	RaycastHitResult constexpr c_EmptyRaycastResult
	{
		.m_Entity = entt::null,
		.m_Distance = FLT_MAX
	};
} // namespace nabi::Physics
