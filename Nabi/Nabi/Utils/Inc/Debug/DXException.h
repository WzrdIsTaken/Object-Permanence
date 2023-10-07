#pragma once
#include "EngineCore.h"

#include <string>

#include "WinIncludes.h"
#include <d3d11.h>

#ifdef USE_DEBUG_UTILS

namespace nabi::DirectX
{
	// DX_ASSERT macro defined in DebugUtils.h

	/// <summary>
	/// Super cool class to make sure that when I make mistakes they don't go unnoticed!
	/// </summary>
	class DXException final : public std::exception
	{
	public:
		DXException(std::string const file, int const line, HRESULT const hr) NABI_NOEXCEPT;
		[[nodiscard]] virtual const char* what() const noexcept override; // From std::exception

		[[nodiscard]] static std::string const TranslateErrorCode(HRESULT const hr) NABI_NOEXCEPT;

		[[nodiscard]] std::string const GetType() const NABI_NOEXCEPT; // Could be made virtual and inherit from DXException in the future
		[[nodiscard]] std::string const GetOriginString() const NABI_NOEXCEPT;

		[[nodiscard]] HRESULT const GetErrorCode() const NABI_NOEXCEPT;
		[[nodiscard]] std::string const GetErrorString() const NABI_NOEXCEPT;

		[[nodiscard]] int GetLine() const NABI_NOEXCEPT;
		[[nodiscard]] std::string const& GetFile() const NABI_NOEXCEPT;

	private:
		mutable std::string m_WhatBuffer; // Mutable because what() is const

		std::string m_File;
		int m_Line;
		HRESULT m_hR;
	};

	// Functions called in the DX_ASSERT macro

	inline void ThrowIfFailed(std::string const file, int const line, HRESULT const hr)
	{
		if (FAILED(hr))
		{
			// Set a breakpoint on this line to catch DirectX API errors
			throw DXException(file, line, hr);
		}
	}

	inline void ThrowIfFailed(std::string const file, int const line, void const* const obj)
	{
		if (!obj)
		{
			throw DXException(file, line, GetLastError());
		}
	}

	inline void ThrowIfFailed(std::string const file, int const line, BOOL const result)
	{
		if (result < 0)
		{
			throw DXException(file, line, GetLastError());
		}
	}

	inline void ThrowIfFailed(std::string const file, int const line, DWORD const result)
	{
		if (result == INVALID_SET_FILE_POINTER)
		{
			throw DXException(file, line, GetLastError());
		}
	}
} // namespace nabi::DirectX

#endif // USE_DEBUG_UTILS
