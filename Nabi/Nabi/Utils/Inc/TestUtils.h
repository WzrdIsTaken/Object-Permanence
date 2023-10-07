#pragma once
#include "EngineCore.h"

#ifndef RUN_TESTS
	#define MAKE_TESTABLE

	#define MAKE_LOGIC_TESTABLE
	#define BUILD_CONFIGURATION_CHANGE_BEGIN
	#define BUILD_CONFIGURATION_CHANGE_END
#else
	// Used for functions
	#define MAKE_TESTABLE virtual 

	// Used for code inside functions
	/*
	  Sometimes we will want to have different behaviour in debug and release builds, for example in release omit some code, but still want to 
	  be able to test the release branch while in debug mode (so all tests are run at once). To do this we can switch the c_BuildConfiguration
	  variable, which isn't marked as constexpr in debug builds. However, doing this is risky as it could cause strange behaviour and hard
	  to track down bugs if we don't change it back. These macros should hopefully alleviate that problem.
	*/
	#define MAKE_LOGIC_TESTABLE \
		using namespace nabi::BuildUtils; \
		if (c_BuildConfiguration == BuildConfiguration::Release)

	// Used inside test cases
	#define BUILD_CONFIGURATION_CHANGE_BEGIN \
		using namespace nabi::BuildUtils; \
		BuildConfiguration const startBuildConfiguration = c_BuildConfiguration;
	#define BUILD_CONFIGURATION_CHANGE_END \
		c_BuildConfiguration = startBuildConfiguration; \
		ASSERT_EQ(startBuildConfiguration, c_BuildConfiguration); // These macros should only be run in tests, so we can do this gMock check
#endif // #ifdef RUN_TESTS
