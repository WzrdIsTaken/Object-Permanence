#include "TestCore.h"

#include "Examples\TestThreading.h"

#include "Context.h"
#include "CoreModules\InputModule.h"
#include "CoreSystems\InputSystem.h"
#include "LockCriticalSection.h"

#ifdef RUN_TESTS

// This example is a lot shorter than the others.
// For other (and probably better) examples of ThreadCommand being used see NabiCore and TestPhysics.

namespace nabi::Tests::Examples
{
	long long constexpr c_ThreadSleepTime = 2000ll;

	TestThreading::TestThreading(nabi::Context& context)
		: m_Context(context)
	{
	}

	TestThreading::~TestThreading()
	{
	}

	bool TestThreading::Init()
	{
		m_InputSystem = std::make_unique<ecs::InputSystem>(m_Context, "Input"_hs, "NabiThreadingTestSystems"_hs);
		return true;
	}

	bool TestThreading::Update()
	{
		using namespace nabi::Input;
		InputState const wKeyState = ecs::InputModule::GetKeyboardKey(m_Context, InputCode::Key_W);

		if (wKeyState == InputState::Pressed)
		{
			/*
			m_Context.m_ThreadCommand->EnqueueTaskDetach("VoidFunc", SHORT_TASK, HIGH_PRIORITY,
				[&]() -> void
				{
					VoidFunc();
				});
			*/

			/*
			std::future<int> future = m_Context.m_ThreadCommand->EnqueueTask("IntFunc", SHORT_TASK, HIGH_PRIORITY,
				[&]() -> int
				{
					return IntFunc();
				});

			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST, "Other work here", LOG_END);
			int result = future.get();
			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST, result, LOG_END);
			*/

			/*
			std::future<float> future = m_Context.m_ThreadCommand->EnqueueTask("ParamFunc", SHORT_TASK, HIGH_PRIORITY,
				[&](float const f) -> float
				{
					return ParamFunc(f);
				}, 5.9f);

			float result = future.get();
			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST, result, LOG_END);
			*/

			/*
			{
				LOCK_CRITICAL_SECTION(m_Context)

				int i = 0;
				i++;
			}
			*/

			m_Context.m_ThreadCommand->EnqueueTaskDetach("ThreadInAThread", SHORT_TASK, HIGH_PRIORITY,
				[&]() -> void
				{
					ThreadInsideAThreadFunc();
				});
		}
		return true;
	}

	bool TestThreading::Render()
	{
		return false;
	}

	void TestThreading::VoidFunc() const
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(c_ThreadSleepTime));
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST, "Hello!", LOG_END);
	}

	int TestThreading::IntFunc() const
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(c_ThreadSleepTime));
		return 21;
	}

	float TestThreading::ParamFunc(float const f) const
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(c_ThreadSleepTime));
		return f;
	}

	void TestThreading::ThreadInsideAThreadFunc() const
	{
		std::future<int> result = m_Context.m_ThreadCommand->EnqueueTask("SubThread", SHORT_TASK, MEDIUM_PRIORITY,
			[&]() -> int 
			{
				return IntFunc();
			});

		int value = result.get();
		++value;
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST, value, LOG_END);
	}
} // namespace nabi::Tests::Examples

#endif // ifdef RUN_TESTS
