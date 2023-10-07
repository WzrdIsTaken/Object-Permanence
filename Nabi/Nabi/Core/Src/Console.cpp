#include "EngineCore.h"

#include "Console.h"

#include <iostream>

namespace nabi
{
	void AllocateConsole(UINT const maxLines) NABI_NOEXCEPT
	{
		// Console Variables
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		FILE* file;

		// Allocate the console for this app
		AllocConsole();

		// Set the screen buffer to be big enough to let us scroll text
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
		consoleInfo.dwSize.Y = maxLines;
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), consoleInfo.dwSize);

		// Redirect unbuffed STDIN to the console
		if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE)
		{
			if (!(freopen_s(&file, "CONIN$", "r", stdin) != NULL))
			{
				setvbuf(stdin, NULL, _IONBF, 0);
			}
		}
		// Redirect unbuffed STDOUT to the console
		if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
		{
			if (!(freopen_s(&file, "CONOUT$", "w", stdout) != NULL))
			{
				setvbuf(stdout, NULL, _IONBF, 0);
			}
		}
		// Redirect unbuffed STDERR to the console
		if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE)
		{
			if (!(freopen_s(&file, "CONOUT$", "w", stderr) != NULL))
			{
				setvbuf(stderr, NULL, _IONBF, 0);
			}
		}

		// Make C++ standard streams point to the console as well
		std::ios::sync_with_stdio(true);

		// Clear the error state for each of the C++ standard streams
		std::wcout.clear();
		std::cout.clear();
		std::wcerr.clear();
		std::cerr.clear();
		std::wcin.clear();
		std::cin.clear();
	}

	void ReleaseConsole() NABI_NOEXCEPT
	{
		// Console variables
		FILE* file;

		// Just to be safe, redirect standard IO to NULL before releasing
		// Redirect STDIN to NULL
		if (!(freopen_s(&file, "NUL:", "r", stdin) != NULL))
		{
			setvbuf(stdin, NULL, _IONBF, 0);
		}
		// Redirect STDOUT to NULL
		if (!(freopen_s(&file, "NUL:", "w", stdout) != NULL))
		{
			setvbuf(stdout, NULL, _IONBF, 0);
		}
		// Redirect STDERR to NULL
		if (!(freopen_s(&file, "NUL:", "w", stderr) != NULL))
		{
			setvbuf(stderr, NULL, _IONBF, 0);
		}

		// Detach from console
		FreeConsole();
	}
} // namespace nabi
