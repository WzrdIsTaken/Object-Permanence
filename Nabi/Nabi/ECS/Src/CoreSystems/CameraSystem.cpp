#include "Core.h"

#include "CoreSystems\CameraSystem.h"

#include "Context.h"
#include "CoreComponents\CameraComponent.h"
#include "GameTime.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(CameraSystem)
	RELFECT_SYSTEM_END(CameraSystem)

	CameraSystem::CameraSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
	{
		REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(CameraSystem)
	}

	CameraSystem::~CameraSystem()
	{
		UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(CameraSystem)
	}

	void CameraSystem::Update(nabi::GameTime const& /*gameTime*/)
	{
		m_Context.m_Registry.view<CameraGroupComponent>().each(
			[&](auto& cameraGroupComponent) -> void
			{
				for (auto& cameraComponent : cameraGroupComponent.m_Cameras)
				{
					CheckAndUpdateCamera(cameraComponent);
				}
			});

		m_Context.m_Registry.view<CameraComponent>().each(
			[&](auto& cameraComponent) -> void
			{
				CheckAndUpdateCamera(cameraComponent);
			});
	}

	void CameraSystem::CheckAndUpdateCamera(CameraComponent& cameraComponent) const
	{
		if (cameraComponent.m_HasToBeUpdated)
		{
			UpdateCamera(cameraComponent);
			cameraComponent.m_HasToBeUpdated = false;
		}
	}

	void CameraSystem::UpdateCamera(CameraComponent& cameraComponent) const
	{
		// Note: The render system actually forms the constant buffer from the camera's data

		dx::XMMATRIX const translationMatrix = 
			dx::XMMatrixTranslation(-cameraComponent.m_Position.x, cameraComponent.m_Position.y, cameraComponent.m_Position.z);
		dx::XMMATRIX const rotationMatrix = 
			dx::XMMatrixRotationRollPitchYaw(cameraComponent.m_Rotation.x, cameraComponent.m_Rotation.y, cameraComponent.m_Rotation.z);

		dx::XMMATRIX const viewMatrix = dx::XMMatrixMultiply(translationMatrix, rotationMatrix);
		dx::XMStoreFloat4x4(&cameraComponent.m_View, viewMatrix);
	}
} // namespace ecs
