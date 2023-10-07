#pragma once
#include "EngineCore.h"

#include "WinIncludes.h"

#include "entt.h"

#include "Context.h"
#include "DXObjects.h"
#include "GameTime.h"
#include "InitSettings.h"
#include "ThreadingObjects.h"
#include "WindowEventsListener.h"
#include "XAudioObjects.h"

// Used to test / develop Nabi's core features
//#define INCLUDE_NABI_EXAMPLES
#ifdef INCLUDE_NABI_EXAMPLES
	#include "Examples\TestAudio.h"
	#include "Examples\TestDraw.h"
	#include "Examples\TestInput.h"
	#include "Examples\TestPhysics.h"
	#include "Examples\TestThreading.h"
	// There is no TestReflection as it was the first thing I did. However, there are plenty of unit tests

	// Just call the Example's Init/Update/Render functions at the end of the matching NabiCore function
#endif // ifdef INCLUDE_EXAMPLES

namespace nabi
{
	/// <summary>
	/// The base of all things Nabi. This is kinda big deal
	/// </summary>
	class NabiCore 
#ifndef ENGINE_DEVELOPMENT
		abstract // So we can create a base instance of Nabi in main to easily test core features
#endif // !ENGINE_DEVELOPMENT
	{
	public:
		NabiCore(HINSTANCE const hInstance, NabiCoreSettings const& initSettings) NABI_NOEXCEPT;
		virtual ~NabiCore();

		[[nodiscard]] virtual int Init();
		[[nodiscard]] virtual int Run();

		[[nodiscard]] inline Context& GetContext() NABI_NOEXCEPT { return m_Context; }

	protected:
		virtual void Update() NABI_NOEXCEPT;
		virtual void FixedUpdate() NABI_NOEXCEPT;
		virtual void Render() NABI_NOEXCEPT;

		bool InitGraphicsEntity() NABI_NOEXCEPT;
		bool InitDxPipeline() NABI_NOEXCEPT;
		bool InitInputEntity() NABI_NOEXCEPT;
		bool InitPhysicsEntity() NABI_NOEXCEPT;
		bool InitAudioEntity() NABI_NOEXCEPT;

		bool ParseECSData() NABI_NOEXCEPT; // User defined xml ecs data

		// Windows
		HINSTANCE const m_hInstance;
		WindowEventsListener m_WindowEventsListener;

		// Core Objects
		Rendering::DXObjects m_DXObjects;
		Audio::XAudioObjects m_XAudioObjects;
		Threading::ThreadingObjects m_ThreadingObjects;

		// Nabi
		Context m_Context;
		GameTime m_GameTime;
#ifdef USE_EVENT_SYSTEM_UPDATE
		std::unordered_map<std::string, entt::meta_any> m_Systems;
#endif // ifdef USE_EVENT_SYSTEM_UPDATE
		NabiCoreSettings m_InitSettings;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(NabiCore)
	};
} // namespace nabi

#undef INCLUDE_NABI_EXAMPLES
