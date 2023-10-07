#pragma once
#include "Core.h"

#include "DirectXIncludes.h"

#include "ComponentBase.h"

namespace ecs
{
	struct RigidbodyComponent final : public nabi::ECS::ComponentBase
	{
#pragma pack(push, 1)
		struct Constraints final
		{
			bool m_FreezeX : 1;
			bool m_FreezeY : 1;
			bool m_FreezeZ : 1;
			bool const c_Padding : 1;

			Constraints& operator=(Constraints const& other);
			static Constraints FromString(std::string const& string);
		};
#pragma pack(pop)

		dx::XMFLOAT3 m_Velocity;
		dx::XMFLOAT3 m_AngularVelocity;

		float m_Drag;
		float m_AngularDrag;

		float m_Mass;
		float m_GravityScale;

		Constraints m_PositionConstraints;
		Constraints m_RotationConstraints;

		//bool m_IsKinematic;?

		RigidbodyComponent()
			: m_Velocity{0.0f, 0.0f, 0.0f}
			, m_AngularVelocity{0.0f, 0.0f, 0.0f}
			, m_Drag(1.0f)
			, m_AngularDrag(1.0f)
			, m_Mass(1.0f)
			, m_GravityScale(1.0f)
			, m_PositionConstraints{false, false, false}
			, m_RotationConstraints{false, false, false}
		{
		}
	};
} // namespace ecs
