#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "ReflectionIncludes.h"

#include "SystemBase.h"

// this is really really bot, but its just a really quick way to test 3d audio effects
namespace ecs
{
	struct AudioEmitterComponent;
} // namespace ecs
namespace nabi
{
	struct Context;
	class GameTime;
} // namespace nabi

namespace ecs
{
	class AsteroidSystem final : public nabi::ECS::SystemBase
	{
	public:
		AsteroidSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~AsteroidSystem();

		void Update(nabi::GameTime const& gameTime);

	private:
		void MoveAndRotateAsteroids(float const dt) const;
		void CheckInput() const;
		void PlaySoundEffect() const;

		static entt::hashed_string constexpr c_AsteroidGroupName = "DemoEntities"_hs;
		mutable AudioEmitterComponent* m_LastAudioEmitter; // see above. this is super bot! just want to be done with this project though...

		REFLECT_PRIVATES(AsteroidSystem)
	};
} // namespace ecs

#endif // ifdef INCLUDE_DEMO
