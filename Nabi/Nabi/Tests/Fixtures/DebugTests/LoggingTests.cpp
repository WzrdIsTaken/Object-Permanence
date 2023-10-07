#include "TestCore.h"

#include "UtilClasses/Logger.h"

#ifdef RUN_TESTS

namespace nabi::Tests::DebugTests
{
#ifdef USE_DEBUG_UTILS
	// Test that logging works corretly
	TEST(DebugTests, CheckLogCalls)
	{
		// Grab the logger and cache the expected number of messages logged
		using nabi::DebugUtils::Logger;
		Logger logger;
		Logger::LogCount const expectedMessagesLogged = logger.GetLogCount() + 1ull;

		// I know we could use EXPECT_CALL here, but this is easier...
		Comparison<Logger::LogCount> comparison(expectedMessagesLogged);

		std::ostringstream logStream;
		logStream << "Hello Nabi!";

		logger.SetLogLevel(LOG_LEVEL_NONE); 
		logger.Log(LOG_INFO, logStream);

		// Check that everything lines up
		comparison.m_Actual = logger.GetLogCount();
		COMPAIR_EQ(comparison);
	}

	// Idea - Could write tests for asserts?
#endif // #ifdef USE_DEBUG_UTILS
} // namespace nabi::Tests::DebugTests
#endif // #ifdef RUN_TESTS
