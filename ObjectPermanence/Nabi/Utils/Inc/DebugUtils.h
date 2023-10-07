#pragma once
#include "EngineCore.h"

#include <sstream>

#include "UtilClasses\Logger.h"
#include "Debug\DXException.h" // For DX_ASSERT - So we have a single include debug file

#ifndef USE_DEBUG_UTILS
	// Solve's the problem of these macros in release being void(0), so the compiler warns that there are too many arguements
	#pragma warning( push ) // Push but never pop
	#pragma warning( disable : 4002 ) // Disables 'too many arguements for function-like macro' warning

	#define ASSERT_FATAL(condition, message) NOT_DEFINED
	#define ASSERT(condition, message) NOT_DEFINED 

	#define ASSERT_FAIL_FATAL(message) NOT_DEFINED
	#define ASSERT_FAIL(message) NOT_DEFINED

	#define FINAL_CODE(code) code
	#define ASSERT_CODE(code) NOT_DEFINED
	#define DX_ASSERT(logic) logic

	#define STATIC_ASSERT(...) NOT_DEFINED

	#define LOG(message) NOT_DEFINED
	#define CONDITIONAL_LOG(condition, message) NOT_DEFINED
	#define LOG_RAW(message) NOT_DEFINED

	#define FUNCTION_NOT_IMPLEMENTED NOT_DEFINED
#else
	#define ASSERT_FATAL(condition, message) ASSERT_BASE(condition, message, "ASSERT FATAL", LOG_FATAL)
	#define ASSERT(condition, message) ASSERT_BASE(condition, message, "ASSERT", LOG_ERROR)

	#define ASSERT_FAIL_FATAL(message) ASSERT_FATAL(false, message)
	#define ASSERT_FAIL(message) ASSERT(false, message)

	#define FINAL_CODE(code) NOT_DEFINED
	#define ASSERT_CODE(code) code
	#define DX_ASSERT(result) nabi::DirectX::ThrowIfFailed(__FILE__, __LINE__, result); // Compiler is not guaranteed to inline so pass in __LINE__ and __FILE__ here

	#define STATIC_ASSERT(...) static_assert(__VA_ARGS__);

	#define LOG(prep, severity, category, message, end) \
			{ \
				using nabi::DebugUtils::Logger; \
				\
				std::ostringstream debugStream; \
				debugStream << prep << severity << LEVEL_MESSAGE_DIVIDER << category << message << end; \
				\
				if (Logger::IsInstanceValid()) \
				{ \
					Logger::Instance()->Log(severity, debugStream); \
				} \
				else \
				{ \
					LOG_RAW(debugStream); \
				} \
			}
	#define CONDITIONAL_LOG(condition, prep, severity, category, message, end) \
		if ((condition)) \
		{ \
			LOG(prep, severity, category, message, end); \
		}

	#define LOG_RAW(message) nabi::DebugUtils::Logger::LogRaw(message);
	#define FAST_LOG(message) LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST, message, LOG_END); // for when you just want to quickly write a log message to check something. deliberately not accounted for in release

	#define FUNCTION_NOT_IMPLEMENTED ASSERT_FAIL("The function " << __FUNCTION__  << " is not implemented!");
#endif // ifndef USE_DEBUG_UTILS

#define TOSTRING(thing)  #thing
#define STRINGIFY(thing) #thing
#define CONCAT(A, B) A ## B 

#define NOT_DEFINED (void(0));
#define ASSERT_BASE(condition, message, messagePrefix, logLevel) \
		do { \
			if (!(condition)) { \
				std::ostringstream assertStream; \
				assertStream << messagePrefix << LEVEL_MESSAGE_DIVIDER << message; \
				std::string const assertMessage = assertStream.str(); \
				\
				LOG(LOG_PREP, logLevel, BLANK_LOG_CATEGORY, message, LOG_END); \
				_RPTF0(_CRT_ASSERT, assertMessage.c_str()); \
			} \
		} while (false)
		// We use _RPTF0 because: It creates the dialogue box / makes the noise like assert, but allows us to add a custom message to the popup

#define FULL_LOCATION  __FILE__ << " " << __LINE__ << " "
#define SHORT_LOCATION \
	std::string(__FILE__).substr(std::string(__FILE__).find_last_of('\\') + 1u, std::string(__FILE__).size()) << " " << __LINE__ <<

#define SHORT_LOCATION_END(line) SPACE(4 - std::string(TOSTRING(line)).length()) // Because of the way these macros work, we have to do this pass __LINE__ in TOSTRING stuff
#define LEVEL_MESSAGE_DIVIDER ": "

#define LOG_PREP SHORT_LOCATION SHORT_LOCATION_END(__LINE__) // Can add anything else on here if need be
#define LOG_TRACE   "TRACE"
#define LOG_INFO    "INFO"
#define LOG_WARN    "WARNING"
#define LOG_ERROR   "ERROR"
#define LOG_FATAL   "FATAL ERROR"

#define LOG_END ENDLINE
#define LOG_END_NEWLINE NEWLINE
#define LOG_END_BLANK ""
#define NEWLINE "\n"
#define ENDLINE std::endl
#define WRAP(item, wrapCharacter) wrapCharacter << item << wrapCharacter
#define SPACE(numberOfSpaces) std::string(numberOfSpaces, ' ')

#define INDENT_1 3
#define INDENT_2 6
#define INDENT_3 9

// A bunch more macros in here!
#include "Debug\EngineLoggingCatagories.h"
