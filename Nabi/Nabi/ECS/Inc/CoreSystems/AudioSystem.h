#pragma once
#include "Core.h"

#include "ReflectionIncludes.h"

#include "SystemBase.h"

namespace nabi
{
	struct Context;
	class GameTime;
} // namespace nabi

namespace ecs
{
	class AudioSystem final : public nabi::ECS::SystemBase
	{
	public:
		AudioSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~AudioSystem();

		void Update(nabi::GameTime const& gameTime);

	private:
		void OnAudioEmitterCreated(entt::registry& registry, entt::entity const entity) const;

		REFLECT_PRIVATES(AudioSystem)
	};
} // namespace ecs
