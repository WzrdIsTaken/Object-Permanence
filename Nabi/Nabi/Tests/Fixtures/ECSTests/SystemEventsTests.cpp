#include "TestCore.h"

#include "GameTime.h"
#include "MockObjects\MockSystem.h"

#ifdef RUN_TESTS

namespace nabi::Tests::ECSTests
{
#ifdef USE_EVENT_SYSTEM_UPDATE
	// Check that system update / render events are fired correctly
	TEST(ECSTests, CheckSystemEvents)
	{
		// Create a mock objects
		nabi::Context context;
		nabi::GameTime gameTime;
		MockSystem mockSystem(context, "id"_hs, "group"_hs);

		// Fire the events
		context.m_NabiEventsManager.FireSystemUpdateEvent(gameTime);
		context.m_NabiEventsManager.FireSystemRenderEvent(gameTime);

		// Compair initial results
		Comparison<int> updateCalled(1, mockSystem.GetUpdateCalledCount());
		Comparison<int> renderCalled(0, mockSystem.GetRenderCalledCount());

		COMPAIR_EQ(updateCalled);
		COMPAIR_EQ(renderCalled);

		// Compair results when registering an event
		mockSystem.RegisterSystemRenderEvent();
		context.m_NabiEventsManager.FireSystemRenderEvent(gameTime);

		renderCalled.m_Expected = 1;
		renderCalled.m_Actual = mockSystem.GetRenderCalledCount();

		COMPAIR_EQ(renderCalled);

		// Compair results when disabling that event
		mockSystem.DisableSystemRenderEvent();
		context.m_NabiEventsManager.FireSystemRenderEvent(gameTime);

		renderCalled.m_Expected = 1;
		renderCalled.m_Actual = mockSystem.GetRenderCalledCount();

		COMPAIR_EQ(renderCalled);

		// And again when enabling it again
		mockSystem.EnabledSystemRenderEvent();
		context.m_NabiEventsManager.FireSystemRenderEvent(gameTime);

		renderCalled.m_Expected = 2;
		renderCalled.m_Actual = mockSystem.GetRenderCalledCount();

		COMPAIR_EQ(renderCalled);

		// Disable the event to clean up nicely
		mockSystem.UnregisterSystemRenderEvent();
	}
#endif // ifdef USE_EVENT_SYSTEM_UPDATE
} // namespace nabi::Tests::ECSTests

#endif // #ifdef RUN_TESTS
