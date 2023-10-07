#pragma once

// The Lib Level file for test code. Also has some other stuff to make life easier.

// Nabi Core
#include "Core.h"
// (see Core.h for why there is no lib level check)

// gTest
#include "gtest/gtest.h" // Note - This has been done https://stackoverflow.com/questions/68193449/disable-warnings-from-google-test

// Helpers
#include "Helpers/Comparison.h"

#ifdef RUN_TESTS
	// Using - I thought this might be good so in the tests every test thing exists at the same level + its easy to differentiate between mock ecs stuff and 'real' ecs stuff 
	namespace nabi::Tests::ECS {};
	using namespace nabi::Tests::ECS;

	#define LOG_NOT_RUNNING_SLOW_TEST_WARNING \
		LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_TEST, "Not running slow test " << WRAP(__FUNCTION__, "'") << " - make sure this is done before committing", LOG_END);
#endif // ifdef RUN_TESTS
