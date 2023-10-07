#pragma once
#include "EngineCore.h"

#include <string>

#include "WinIncludes.h"

namespace nabi
{
	// --- Window Settings ---
	struct WindowSettings final
	{
		LONG m_Width;
		LONG m_Height;

		LONG m_MinWidth;
		LONG m_MinHeight;
	
		LPCWSTR m_WindowName;
		LPCWSTR m_ClassName;

		bool m_AllowResizing;
		bool m_UseApplicationIcon;
		size_t m_StartingEventsSize;
	};

	WindowSettings const c_WindowDefaultSettings = {
		.m_Width = 1280l,
		.m_Height = 720l,

		.m_MinWidth = 320l,
		.m_MinHeight = 200l,

		.m_WindowName = L"Nabi",
		.m_ClassName = L"Butterfly",

		.m_AllowResizing = true,
		.m_UseApplicationIcon = true,
		.m_StartingEventsSize = 10u
	};

	// --- Data Settings ---
	struct DataSettings final
	{
		enum class NabiCoreParseMode : int
		{
			All,
			None,
			Systems,
			Components,

			ENUM_COUNT
		};

		std::string m_RouteDocument;
		NabiCoreParseMode m_NabiCoreParseDocuments;
	};

	DataSettings const c_DataDefaultSettings = {
		.m_RouteDocument = "Data/Core.xml",
		.m_NabiCoreParseDocuments = DataSettings::NabiCoreParseMode::None
	};

	// --- Threading Settings ---
	struct ThreadingSettings final
	{
		unsigned int m_ThreadPoolSize;
	};

	ThreadingSettings const c_ThreadingDefaultSettings = {
		.m_ThreadPoolSize = 8u // Limited to std::thread::hardware_concurrency() in ThreadCommand
	};

	// --- Nabi Core Settings ---
	struct NabiCoreSettings final
	{
		WindowSettings m_WindowSettings;
		DataSettings m_DataSettings;
		ThreadingSettings m_ThreadingSettings;
	};

	NabiCoreSettings const c_NabiCoreDefaultSettings = {
		.m_WindowSettings = c_WindowDefaultSettings,
		.m_DataSettings   = c_DataDefaultSettings,
		.m_ThreadingSettings = c_ThreadingDefaultSettings
	};
} // namespace nabi
