#include "Core.h"

#include "CoreSystems\RenderSystem.h"

#include "Context.h"
#include "CoreComponents\EntityInfoComponent.h"
#include "DirectXUtils.h"
#include "GameTime.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(RenderSystem)
	RELFECT_SYSTEM_END(RenderSystem)

	RenderSystem::RenderSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
#ifdef USE_DEBUG_UTILS
		, m_DebugProjectionMatrix{}
		, m_DebugViewMatrix{}
		, m_DebugModelMatrix{}
#endif // ifdef USE_DEBUG_UTILS
	{
		REGISTER_SYSTEM_RENDER_EVENT_SUBSCRIBER(RenderSystem)
	}

	RenderSystem::~RenderSystem()
	{
		UNREGISTER_SYSTEM_RENDER_EVENT_SUBSCRIBER(RenderSystem)
	}

	void RenderSystem::Render(nabi::GameTime const& /*gameTime*/)
	{
		// Get the graphics entity. This stores the camera, constant buffers, etc
		entt::entity graphicEntity = m_Context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Graphic);

		// Cache the graphics and camera component
		SComp::GraphicsComponent& graphicsComponent = m_Context.m_Registry.get<SComp::GraphicsComponent>(graphicEntity);
		CameraGroupComponent const& cameraGroupComponent = m_Context.m_Registry.get<CameraGroupComponent>(graphicEntity);

		// Complete render passes
		RenderPerspective(graphicsComponent, cameraGroupComponent);
		RenderOrthographic(graphicsComponent, cameraGroupComponent);
	}

	void RenderSystem::RenderPerspective(SComp::GraphicsComponent& graphicsComponent, CameraGroupComponent const& cameraGroupComponent)
	{
		CameraComponent const& cameraComponent = cameraGroupComponent.m_Cameras.at(CameraIndex::Perspective);
		RenderInternal<TComp::DrawPerspectiveTagComponent>(graphicsComponent, cameraComponent);
	}

	void RenderSystem::RenderOrthographic(SComp::GraphicsComponent& graphicsComponent, CameraGroupComponent const& cameraGroupComponent)
	{
		CameraComponent const& cameraComponent = cameraGroupComponent.m_Cameras.at(CameraIndex::Orthographic);
		RenderInternal<TComp::DrawOrthographicTagComponent>(graphicsComponent, cameraComponent);
	}

#ifdef USE_DEBUG_UTILS
	void RenderSystem::DebugTraceOutput(entt::entity const entity) const
	{
		// Work out what the resultant matrix should be
		dx::XMMATRIX result;
		result = dx::XMMatrixMultiply(m_DebugModelMatrix, m_DebugViewMatrix);
		result = dx::XMMatrixMultiply(result, m_DebugProjectionMatrix);

		// Find the entities name for logging
		std::string entityName = "";
		if (ecs::EntityInfoComponent* entityInfoComponent = m_Context.m_Registry.try_get<ecs::EntityInfoComponent>(entity))
		{
			entityName = entityInfoComponent->m_EntityName.data();
		}

		// Build the log message
		using namespace nabi::DirectXUtils;
		std::ostringstream logMessage;

		logMessage <<
			"The model on entity "        << WRAP(entityName, "'") << " has a: "     << NEWLINE
			"Model Matrix of          : " << MatrixToString(m_DebugModelMatrix)      << NEWLINE
			"View Matrix of           : " << MatrixToString(m_DebugViewMatrix)       << NEWLINE
			"Projection Matrix of     : " << MatrixToString(m_DebugProjectionMatrix) << NEWLINE
			"And a resultant matrix of: " << MatrixToString(result);

		// Log
		LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_RENDERING, logMessage.str(), LOG_END);
	}
#endif // USE_DEBUG_UTILS
} // namespace ecs
