#pragma once
#include "Core.h"

#include "gtest/gtest.h" 
#include "shellapi.h" // For CommandLineToArgvW

#include "Console.h"
#include "InitSettings.h"
#include "MathUtils.h"
#include "NabiCore.h"
#include "StringStore.h"

namespace nabi
{
	struct NabiMainParams final
	{
        NabiCoreSettings m_CoreSettings;
	};

	struct WinMainParams final
	{
        HINSTANCE m_hInstance;
        HINSTANCE m_hPrevInstance;
        LPWSTR m_lpCmdLine;
        int m_nShowCmd;
	};

    template<typename T>
    [[nodiscard]] int Main(NabiMainParams const& nabiParams, WinMainParams const& winParams) NABI_NOEXCEPT
    {
		STATIC_ASSERT(std::is_base_of<NabiCore, T>::value, "Main's templated parameter must inherit from NabiCore");

		// --- Application Setup ---

		int argc;
		LPWSTR* argv = CommandLineToArgvW(winParams.m_lpCmdLine, &argc); // Prev used GetCommandLineW()

		DX_ASSERT(CoInitializeEx(nullptr, COINIT_MULTITHREADED));

		// --- Nabi Setup ---

		// Create a console window
#ifdef USE_DEBUG_UTILS
		UINT constexpr consoleMaxLines = 4096u;
		AllocateConsole(consoleMaxLines);
#endif // #ifdef USE_DEBUG_UTILS

		// Setup Debug Utils
#ifdef USE_DEBUG_UTILS
		_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW); // Without calling this function, the assert dialogue window doesn't appear
		DebugUtils::Logger::CreateInstance();
#endif // #ifdef USE_DEBUG_UTILS

		// Setup Nabi's global stuff
		Reflection::StringStore::CreateInstance(); // ):
		MathUtils::SeedRandom();

		// --- Run Tests ---

#ifdef RUN_TESTS
		// Set the log level to warning because otherwise it spams the console when tests are run
#ifdef USE_DEBUG_UTILS
		DebugUtils::Logger::Instance()->SetLogLevel(LOG_WARN);
#endif // #ifdef USE_DEBUG_UTILS

		// Run tests
		::testing::InitGoogleTest(&argc, argv);
		int const testResults = RUN_ALL_TESTS();

		ASSERT(testResults == NABI_SUCCESS, "One or more of the tests failed! See the console output for details, or run the test explorer.");

		// Reset Nabi after tests
		Reflection::StringStore::Instance()->Clear();
#ifdef USE_DEBUG_UTILS
		DebugUtils::Logger::Instance()->SetLogLevel(LOG_INFO);
#endif // #ifdef USE_DEBUG_UTILS

		LOG(NEWLINE << LOG_PREP, LOG_INFO, LOG_CATEGORY_CORE, "All tests run. Initializing Nabi..." << NEWLINE, LOG_END);
#endif // #ifdef RUN_TESTS

		// --- Nabi Initialization ---

		auto app = T(winParams.m_hInstance, nabiParams.m_CoreSettings);
		LOG(NEWLINE << LOG_PREP, LOG_INFO, LOG_CATEGORY_CORE, "Nabi has initialized successfully!" << NEWLINE, LOG_END);

		int appRunResult = app.Init();
		ASSERT(appRunResult == NABI_SUCCESS, "The app failed to initialize!");

		appRunResult = app.Run();
		ASSERT(appRunResult == NABI_SUCCESS, "The app hit an error while running!");

		// --- Nabi Shutdown ---

#ifdef USE_DEBUG_UTILS
		ReleaseConsole();
#endif // #ifdef USE_DEBUG_UTILS

		// --- Application Shutdown ---

		CoUninitialize();

		LOG(NEWLINE << LOG_PREP, LOG_INFO, LOG_CATEGORY_CORE, "Shutting down Nabi with code " << appRunResult, LOG_END);
		return appRunResult;
    }
} // namespace nabi

// If we need another Nabi level singleton, I'll make a CreateSingletons() func to group them together. But I don't think we will..
// Note - we cannot have a cpp file called "Main" because there is already "main.cpp".
