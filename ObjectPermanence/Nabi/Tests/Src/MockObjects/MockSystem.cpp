#include "Core.h"

#include "MockObjects/MockSystem.h"

#ifdef RUN_TESTS

namespace nabi::Tests::ECS
{
	REFLECT_SYSTEM_BEGIN(MockSystem, "MockSystem")
		REFLECT_SYSTEM_PROPERTY(MockSystem::m_MockSystemData, "MockSystemData")
	RELFECT_SYSTEM_END(MockSystem)

	MockSystem::MockSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
		, m_MockSystemData(0)
		, m_UpdateCalledCount(0)
		, m_RenderCalledCount(0)
	{
		REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(MockSystem)
	}

	MockSystem::~MockSystem()
	{
		UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(MockSystem)
	}

	void MockSystem::Update(nabi::GameTime const& /*gameTime*/)
	{
		++m_UpdateCalledCount;
	}

	void MockSystem::Render(nabi::GameTime const& /*gameTime*/)
	{
		++m_RenderCalledCount;
	}

	void MockSystem::RegisterSystemRenderEvent()
	{
		REGISTER_SYSTEM_RENDER_EVENT_SUBSCRIBER(MockSystem)
	}

	void MockSystem::UnregisterSystemRenderEvent()
	{
		UNREGISTER_SYSTEM_RENDER_EVENT_SUBSCRIBER(MockSystem)
	}

	void MockSystem::EnabledSystemRenderEvent()
	{
		ENABLE_SYSTEM_RENDER(MockSystem)
	}

	void MockSystem::DisableSystemRenderEvent()
	{
		DISABLE_SYSTEM_RENDER(MockSystem)
	}
} // namespace nabi::Tests::ECS

#endif // #ifdef RUN_TESTS

// back in the day... a classic
//int some_var_so_i_can_easily_put_a_breakpoint_in_here = 0;
//++some_var_so_i_can_easily_put_a_breakpoint_in_here;
