#pragma once

// A little clunky, but these guys can be used to toggle on / off different engine features

// Core defines
#define USE_EVENT_SYSTEM_UPDATE
#define USE_CORE_FUNCTIONALITY_MULTITHREADING
//#define USE_NABI_NOEXCEPT

// Debug defines
#ifdef _DEBUG
	//#define INCLUDE_DEMO     // Demo of all Nabi's features working together
	#define RUN_TESTS		 // gMock Tests
	//#define RUN_SLOW_TESTS   // Any tests which take over ~10ms to run. Make sure to run before commiting
	#define USE_DEBUG_UTILS  // Asserts + Logging + Sneaky hacks

	// Exactly what you think. Effectively means we double the registry.get() calls, so may be worth toggling outside of release builds
	#define ENTT_CHECK_IF_COMPONENT_EXISTS_BEFORE_GET 
#else
	// Can shift around the defines if need to test stuff
#endif // ifdef _DEBUG

// Value defines
#define NABI_SUCCESS EXIT_SUCCESS 
#define NABI_FAIL    EXIT_FAILURE

// Development defines
//#define ENGINE_DEVELOPMENT

// Defines based off other defines!
#ifndef USE_NABI_NOEXCEPT
	/*
	* About noexcept...
	* So, when I first started developing nabi I heard that functions marked as noexcept can have performace improvements 
	* and the compiler will generate less code for them (so the file size would be smaller) as it doesn't need to create
	* any exception handling code. Therefore, I started just marking every function as noexcept behind a define.
	* 
	* The thought was that if games made with nabi are ever distributed, then it could be enabled to eke out a tiny bit 
	* more performance. But as I developed nabi, I became more on the fence about this decision. 
	* However, I kept going with the style out of habit.
	*
	* Now at the end of nabi's development, I think I may have made a mistake. I didn't properly understand noexcept and when to
	* use it. I think I understand more now, and I should use noexcept more like const - so show intent more than anything, but it
	* would take a decent chunk of time to completely removed noexcept and I'm still a tiny bit unsure.. Running nabi with this
	* define enabled works fine at any rate (though I think I'll leave it undefined for now..).
	* 
	* So, I'll leave it in for the engine code - but won't use it any more for game code. I will only use noexcept when I want 
	* to show intent rather than just sticking it everywhere. 
	* noexcept can be just one of many learnings that have come from making nabi...
	*/

	#define NABI_NOEXCEPT 
#else
	#define NABI_NOEXCEPT noexcept
#endif // ifndef USE_NABI_NOEXCEPT

	// ifdef USE_CORE_FUNCTIONALITY_MULTITHREADING (but we want these task name defines regardless of if its enabled)
#define CORE_TASK_RENDER Render
#define CORE_TASK_SIMULATION Simulation
