#include "Core.h"

#include "CoreModules\CameraModule.h"

namespace ecs::CameraModule
{
	void DefaultCameraValues(nabi::Context const& /*context*/, ecs::CameraComponent& cameraComponent, DefaultCameraValuesSettings const& defaultSettings)
	{
		float const aspectRatio = defaultSettings.m_WindowWidth / defaultSettings.m_WindowHeight;
		dx::XMMATRIX const projMatrix = dx::XMMatrixPerspectiveFovLH(defaultSettings.m_FovAngleY, aspectRatio, defaultSettings.m_NearZ, defaultSettings.m_FarZ);

		dx::XMFLOAT3 const view = defaultSettings.m_CameraViewMatrixTranslation;
		dx::XMMATRIX const viewMatrix = dx::XMMatrixTranslation(view.x, view.y, view.z);

		dx::XMStoreFloat4x4(&cameraComponent.m_Projection, projMatrix);
		dx::XMStoreFloat4x4(&cameraComponent.m_View, viewMatrix);

		cameraComponent.m_Position = defaultSettings.m_CameraPosition;
		cameraComponent.m_Target = defaultSettings.m_CameraTarget;
	}

	dx::XMFLOAT2 ConvertWorldCoordToScreenCoord(nabi::Context const& context, CameraComponent const& perspectiveCamera, dx::XMFLOAT3 const& worldCoord)
	{
		FUNCTION_NOT_IMPLEMENTED
		return {};
	}

	dx::XMFLOAT3 ConvertScreenCoordToWorldCoord(nabi::Context const& context, CameraComponent const& orthographicCamera, dx::XMFLOAT2 const& screenCoord)
	{
		FUNCTION_NOT_IMPLEMENTED
		return {};
	}
} // namespace ecs::CameraModule
