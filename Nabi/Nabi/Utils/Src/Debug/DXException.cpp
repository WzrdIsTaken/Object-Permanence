#include "EngineCore.h"

#include "Debug/DXException.h"

#include "DebugUtils.h"

#ifdef USE_DEBUG_UTILS

namespace nabi::DirectX
{
	DXException::DXException(std::string const file, int const line, HRESULT const hr) NABI_NOEXCEPT
		: m_File(file)
		, m_Line(line)
		, m_hR(hr)
	{
	}

	const char* DXException::what() const noexcept
	{
		std::ostringstream oStream;
		oStream << GetType() << std::endl
			<< "[Error Code] " << GetErrorCode() << std::endl
			<< "[Description] " << GetErrorString() << std::endl
			<< GetOriginString();

		m_WhatBuffer = oStream.str();
		ASSERT_FAIL(m_WhatBuffer);

		return m_WhatBuffer.c_str(); 
	}

	std::string const DXException::TranslateErrorCode(HRESULT const hr) NABI_NOEXCEPT
	{
		char* pMsgBuf = nullptr;
		DWORD nMsgLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPWSTR>(&pMsgBuf), 0, nullptr
		);

		if (nMsgLen == 0)
		{
			return "Unidentified error code";
		}

		std::string errorString = pMsgBuf;
		LocalFree(pMsgBuf);

		return errorString;
	}

	std::string const DXException::GetType() const NABI_NOEXCEPT
	{
		return "DirectX Exception";
	}

	std::string const DXException::GetOriginString() const NABI_NOEXCEPT
	{
		std::ostringstream oStream;
		oStream << "[File] " << m_File << std::endl
			    << "[Line] " << m_Line;

		return oStream.str();
	}

	HRESULT const DXException::GetErrorCode() const NABI_NOEXCEPT
	{
		return m_hR;
	}

	std::string const DXException::GetErrorString() const NABI_NOEXCEPT
	{
		return TranslateErrorCode(m_hR);
	}

	int DXException::GetLine() const NABI_NOEXCEPT
	{
		return m_Line;
	}

	std::string const& DXException::GetFile() const NABI_NOEXCEPT
	{
		return m_File;
	}
} // namespace nabi::DirectX

#endif // USE_DEBUG_UTILS
