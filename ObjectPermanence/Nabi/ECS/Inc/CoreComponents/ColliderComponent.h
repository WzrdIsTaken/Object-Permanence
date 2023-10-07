#pragma once
#include "Core.h"

#include "DirectXIncludes.h"

#include "ComponentBase.h"
#include "ReflectionGlobals.h"

namespace ecs
{
	struct ColliderComponent final : public nabi::ECS::ComponentBase
	{
		// Game defined collision enums should use this as there underlying type, and reflect the enum with REFLECT_ENUM_UNDERLYING
		typedef uint64_t ColliderMask;

		enum class ColliderType : int
		{
			Cube,
			Sphere,

			ENUM_COUNT
		};

		enum class InteractionType : int
		{
			Dynamic,
			Static,

			ENUM_COUNT
		};

		ColliderType m_ColliderType;
		dx::XMFLOAT3 m_ColliderDimensions;

		ColliderMask m_Mask;
		InteractionType m_InteractionType;

		entt::hashed_string m_OnCollisionEnterType;   // The type which the action is on
		entt::hashed_string m_OnCollisionEnterAction; // The function to call 
		entt::hashed_string m_OnCollisionExitType;
		entt::hashed_string m_OnCollisionExitAction;

		ColliderComponent()
			: m_ColliderType(ColliderType::Cube)
			, m_ColliderDimensions{ 0.0f, 0.0f, 0.0f }
			, m_Mask(~0ull)
			, m_InteractionType(InteractionType::Dynamic)
			, m_OnCollisionEnterType(nabi::Reflection::ReflectionGlobals::c_InvalidType)
			, m_OnCollisionEnterAction(nabi::Reflection::ReflectionGlobals::c_InvalidFunction)
			, m_OnCollisionExitType(nabi::Reflection::ReflectionGlobals::c_InvalidType)
			, m_OnCollisionExitAction(nabi::Reflection::ReflectionGlobals::c_InvalidFunction)
		{
		}
	};
} // namespace ecs
