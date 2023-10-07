#pragma once
#include "Core.h"

#include "DirectXIncludes.h"

#include "entt.h"

#include "ComponentBase.h"

// Just a note for future Ben - for some reason the camera x/y movement is inverted. Past Ben was too tired of rendering to work out why (sorry about that..)
// So you can either debug it, or just bear this in mind when creating your games camera system! 

// I am very done with rendering right now

namespace ecs
{
	struct CameraIndex final
	{
		enum Enum
		{
			Perspective = 0u,
			Orthographic,
			ENUM_COUNT
		};
	};

	struct CameraComponent final : public nabi::ECS::ComponentBase
	{
		// Used for constant buffers, updated by CameraModule + CameraSystem
		dx::XMFLOAT4X4 m_View;
		dx::XMFLOAT4X4 m_Projection;

		// Change these values in code (eg, to move the camera)
		dx::XMFLOAT3 m_Position;
		dx::XMFLOAT3 m_Rotation;
		dx::XMFLOAT3 m_Target;

		bool m_HasToBeUpdated;

		CameraComponent()
			: m_View{}
			, m_Projection{}
			, m_Position{}
			, m_Rotation{}
			, m_Target{}
			, m_HasToBeUpdated(false)
		{
		}
	};

	struct CameraGroupComponent final : public nabi::ECS::ComponentBase
	{
		std::array<CameraComponent, CameraIndex::Enum::ENUM_COUNT> m_Cameras;

		CameraGroupComponent()
			: m_Cameras{}
		{
		}
	};
} // namespace ecs
