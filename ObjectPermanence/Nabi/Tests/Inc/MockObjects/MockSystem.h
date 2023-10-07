#pragma once
#include "Core.h"

#include "ReflectionIncludes.h"

#include "SystemBase.h"

#ifdef RUN_TESTS

namespace nabi::Tests::ECS
{
	class MockSystem final : public nabi::ECS::SystemBase
	{
	public:
		MockSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~MockSystem();

		void Update(nabi::GameTime const& gameTime);
		void Render(nabi::GameTime const& gameTime);

		void RegisterSystemRenderEvent();
		void UnregisterSystemRenderEvent();
		void EnabledSystemRenderEvent();
		void DisableSystemRenderEvent();
		 
		inline int GetMockSystemData()    const { return m_MockSystemData;    }
		inline int GetUpdateCalledCount() const { return m_UpdateCalledCount; }
		inline int GetRenderCalledCount() const { return m_RenderCalledCount; }

	private:
		int m_MockSystemData;

		// should really use MOCK_METHOD for this
		int m_UpdateCalledCount;
		int m_RenderCalledCount;

		REFLECT_PRIVATES(MockSystem)
	};
} // namespace nabi::Tests::ECS

#endif // #ifdef RUN_TESTS
