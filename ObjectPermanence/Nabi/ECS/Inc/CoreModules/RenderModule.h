
#pragma once
#include "Core.h"

#include "Buffers\RenderBuffers.h"
#include "Context.h"
#include "CoreSingletonComponents\GraphicsComponent.h"
#include "CoreSingletonComponents\LightStateComponent.h"

namespace ecs::RenderModule
{
	// --- Primative Mesh Creation ---

	[[nodiscard]] nabi::Rendering::Mesh CreatePrimativeCube(nabi::Context const& context, float const width, float const height, float const depth);
	[[nodiscard]] nabi::Rendering::Mesh CreatePrimativeSphere(nabi::Context const& context, int const latLines, int const longLines);

	// --- Light State Getters ---

	[[nodiscard]] inline SComp::LightStateComponent const* const TryGetLightStateComponent(nabi::Context const& context)
	{
		return context.m_Registry.try_get<SComp::LightStateComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Graphic));
	}
	[[nodiscard]] inline SComp::LightStateComponent* const TryGetLightStateComponent(nabi::Context& context)
	{
		return const_cast<SComp::LightStateComponent* const>(TryGetLightStateComponent(const_cast<nabi::Context const&>(context)));
	}
	[[nodiscard]] inline SComp::LightStateComponent const& GetLightStateComponent(nabi::Context const& context)
	{
		return context.m_Registry.get<SComp::LightStateComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Graphic));
	}
	[[nodiscard]] inline SComp::LightStateComponent& GetLightStateComponent(nabi::Context& context)
	{
		return const_cast<SComp::LightStateComponent&>(GetLightStateComponent(const_cast<nabi::Context const&>(context)));
	}

	// --- Graphics State Getters ---

	[[nodiscard]] inline SComp::GraphicsComponent const& GetGraphicsComponent(nabi::Context const& context)
	{
		return context.m_Registry.get<SComp::GraphicsComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Graphic));
	}
	[[nodiscard]] inline SComp::GraphicsComponent& GetGraphicsComponent(nabi::Context& context)
	{
		return const_cast<SComp::GraphicsComponent&>(GetGraphicsComponent(const_cast<nabi::Context const&>(context)));
	}
} // namespace ecs::RenderModule
