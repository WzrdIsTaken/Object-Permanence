#pragma once
#include "Core.h"

#include "ReflectionIncludes.h"

#include "SystemBase.h"

namespace ecs
{
	struct LightingProperties;
} // namespace ecs
namespace nabi
{
	struct Context;
	class GameTime;
} // namespace nabi
namespace nabi::Rendering
{
	struct PerLightChange;
} // namespace nabi::Rendering

namespace ecs
{
	class LightingSystem final : public nabi::ECS::SystemBase
	{
	public:
		LightingSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~LightingSystem();

		void Render(nabi::GameTime const& gameTime);

	private:
		void AssignLightingProperties(nabi::Rendering::PerLightChange& perLightChangeConstantBuffer, LightingProperties const& lightProperties) const;

		void OnLightCreated(entt::registry& registry, entt::entity const entity);
		void OnLightUpdated(entt::registry& registry, entt::entity const entity);
		void OnLightDestroyed(entt::registry& registry, entt::entity const entity);

		REFLECT_PRIVATES(LightingSystem)
	};
} // namespace ecs
