#include "Core.h"

#include "CoreSystems\LightingSystem.h"

#include "Context.h"
#include "CoreComponents\LightComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreSingletonComponents\GraphicsComponent.h"
#include "CoreSingletonComponents\LightStateComponent.h"
#include "CoreModules\RenderModule.h"
#include "GameTime.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(LightingSystem)
	RELFECT_SYSTEM_END(LightingSystem)

#define MANAGE_LIGHT_LISTENERS(_event, action, callback) \
	m_Context.m_Registry._event<DirectionalLightComponent>().action<&callback>(this); \
	m_Context.m_Registry._event<SpotLightComponent>().action<&callback>(this); \
	m_Context.m_Registry._event<PointLightComponent>().action<&callback>(this);

	LightingSystem::LightingSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
	{
		REGISTER_SYSTEM_RENDER_EVENT_SUBSCRIBER(LightingSystem)
		DISABLE_SYSTEM_RENDER(LightingSystem)

		// Set up listeners for the creation, updating and destruction of lights
		MANAGE_LIGHT_LISTENERS(on_construct, connect, LightingSystem::OnLightCreated);
		MANAGE_LIGHT_LISTENERS(on_update,    connect, LightingSystem::OnLightUpdated);
		MANAGE_LIGHT_LISTENERS(on_destroy,   connect, LightingSystem::OnLightDestroyed);
	}

	LightingSystem::~LightingSystem()
	{
		UNREGISTER_SYSTEM_RENDER_EVENT_SUBSCRIBER(LightingSystem)

		// Remove listeners for the creation, updating and destruction of lights
		MANAGE_LIGHT_LISTENERS(on_construct, disconnect, LightingSystem::OnLightCreated);
		MANAGE_LIGHT_LISTENERS(on_update,    disconnect, LightingSystem::OnLightUpdated);
		MANAGE_LIGHT_LISTENERS(on_destroy,   disconnect, LightingSystem::OnLightDestroyed);		
	}

	void LightingSystem::Render(nabi::GameTime const& /*gameTime*/)
	{
		// Cache the light state component
		SComp::LightStateComponent& lightStateComponent = RenderModule::GetLightStateComponent(m_Context);

#ifndef USE_EVENT_SYSTEM_UPDATE
		// Check if the lights need to be updated
		// If USE_EVENT_SYSTEM_UPDATE is defined, then ENABLE/DISABLE_SYSTEM_RENDER is used instead
		if (lightStateComponent.m_UpdateLights)
#endif // ifndef USE_EVENT_SYSTEM_UPDATE
		{
			// Cache the graphics component
			SComp::GraphicsComponent& graphicsComponent = RenderModule::GetGraphicsComponent(m_Context);

			// Get the light constant buffer
			nabi::Rendering::ConstantBuffer const lightConstantBuffer = 
				graphicsComponent.m_ConstantBuffers.at(nabi::Rendering::ConstantBufferIndex::PerLightChange);
			std::vector<nabi::Rendering::PerLightChange> lightConstantBufferData;

			// Calculate how large the per light constant buffer should be
			size_t const lightCount = static_cast<size_t>(lightStateComponent.m_LightCount);
			lightConstantBufferData.resize(lightCount);

			// --- Loop through all the lights ---
			size_t currentLightCount = 0u;

			// Directional
			m_Context.m_Registry.view<TransformComponent const, DirectionalLightComponent const>()
				.each([&](auto& transformComponent, auto& directionalLightComponent) -> void
					{
						nabi::Rendering::PerLightChange& light = lightConstantBufferData.at(currentLightCount);

						// Directional light
						light.m_Position = transformComponent.m_Position;
						light.m_Direction = directionalLightComponent.m_Direction;
						light.m_Colour = directionalLightComponent.m_Colour;
						light.m_Intensity = directionalLightComponent.m_Intensity;

						// Defaults (things a DirectionalLight doesn't have but are in the PerLightChange constant buffer)
						light.m_AttenuationRadius = 1.0f;
						light.m_SpotAngle = 1.0f;

						// Light properties
						AssignLightingProperties(light, directionalLightComponent);
						
						++currentLightCount;
					});

			/*
			// Spot
			m_Context.m_Registry.view<TransformComponent const, SpotLightComponent const>()
				.each([&](auto const& transformComponent, auto const& spotLightComponent) -> void
					{
						++currentLightCount;
					});

			// Point
			m_Context.m_Registry.view<TransformComponent const, PointLightComponent const>()
				.each([&](auto const& transformComponent, auto const& pointLightComponent) -> void
					{

						++currentLightCount;
					});
			*/

			ASSERT(sizeof(nabi::Rendering::PerLightChange) * lightConstantBufferData.size(),
				"The allocated space for the light constant buffer does not match the number of lights created!");

			// --- Let there be light! ---

			m_Context.m_RenderCommand->UpdateConstantBuffer(lightConstantBuffer, lightConstantBufferData.data());
#ifdef USE_EVENT_SYSTEM_UPDATE
			DISABLE_SYSTEM_RENDER(LightingSystem)
#else
			lightStateComponent.m_UpdateLights = false;
#endif // ifdef USE_EVENT_SYSTEM_UPDATE
		}
	}

	void LightingSystem::AssignLightingProperties(nabi::Rendering::PerLightChange& perLightChangeConstantBuffer, LightingProperties const& lightProperties) const
	{
		perLightChangeConstantBuffer.m_AmbientIntensity    = lightProperties.m_AmbientIntensity;
		perLightChangeConstantBuffer.m_DiffuseIntensity    = lightProperties.m_DiffuseIntensity;
		perLightChangeConstantBuffer.m_SpecularAttenuation = lightProperties.m_SpecularAttenuation;
		perLightChangeConstantBuffer.m_SpecularIntensity   = lightProperties.m_SpecularIntensity;
	}

	void LightingSystem::OnLightCreated(entt::registry& /*registry*/, entt::entity const /*entity*/)
	{
		SComp::LightStateComponent& lightStateComponent = RenderModule::GetLightStateComponent(m_Context);
		++lightStateComponent.m_LightCount;

#ifdef USE_EVENT_SYSTEM_UPDATE
		ENABLE_SYSTEM_RENDER(LightingSystem);
#else
		lightStateComponent.m_UpdateLights = true;
#endif // ifdef USE_EVENT_SYSTEM_UPDATE
	}

	void LightingSystem::OnLightUpdated(entt::registry& /*registry*/, entt::entity const /*entity*/)
	{
#ifdef USE_EVENT_SYSTEM_UPDATE
		ENABLE_SYSTEM_RENDER(LightingSystem);
#else
		SComp::LightStateComponent& lightStateComponent = RenderModule::GetLightStateComponent(m_Context);
		lightStateComponent.m_UpdateLights = true;
#endif // ifdef USE_EVENT_SYSTEM_UPDATE
	}

	void LightingSystem::OnLightDestroyed(entt::registry& /*registry*/, entt::entity const /*entity*/)
	{
		// Need to try_get because on shutdown registry.get might fail
		SComp::LightStateComponent* const lightStateComponent = RenderModule::TryGetLightStateComponent(m_Context);
		if (lightStateComponent) --lightStateComponent->m_LightCount;

#ifdef USE_EVENT_SYSTEM_UPDATE
		ENABLE_SYSTEM_RENDER(LightingSystem);
#else
		if (lightStateComponent) lightStateComponent->m_UpdateLights = true;
#endif // ifdef USE_EVENT_SYSTEM_UPDATE
	}
} // namespace ecs
