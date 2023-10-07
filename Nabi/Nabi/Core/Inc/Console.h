#pragma once
#include "EngineCore.h"

#include "WinIncludes.h"

namespace nabi
{
	/// <summary>
	/// Creates a console window
	/// </summary>
	/// <param name="maxLines">- How many lines to allocate the console</param>
	void AllocateConsole(UINT const maxLines) NABI_NOEXCEPT;

	/// <summary>
	/// Destroys the console window
	/// </summary>
	void ReleaseConsole() NABI_NOEXCEPT;
} // namespace nabi
