#pragma once
#include "EngineCore.h"

#include "entt.h"

// The idea beheind this is that if all system updates are event driven, then we can do cool stuff like enabling / disabling them in a slick way and fully data drive systems.
// Though I could of just used virtual inheritance for exactly the same thing and saved myself a lot of time and pain... im good as solving bugs that shouldnt of existed in the first place ;_;

#pragma region System Events

// Perhaps this could be made more generic for all events?
// But it is also verbose, and its likely that these events will be the only things this manager is used for

#ifdef USE_EVENT_SYSTEM_UPDATE
	#define REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(system) \
		{ \
			REGISTER_UNREGISTER_NABI_EVENT_HELPER(GetSystemUpdateEvent) \
			sink.connect<&system::Update>(this); \
		}
	#define REGISTER_SYSTEM_FIXED_UPDATE_EVENT_SUBSCRIBER(system) \
		{ \
			REGISTER_UNREGISTER_NABI_EVENT_HELPER(GetSystemFixedUpdateEvent) \
			sink.connect<&system::FixedUpdate>(this); \
		}
	#define REGISTER_SYSTEM_RENDER_EVENT_SUBSCRIBER(system) \
		{ \
			REGISTER_UNREGISTER_NABI_EVENT_HELPER(GetSystemRenderEvent) \
			sink.connect<&system::Render>(this); \
		}

	#define UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(system) \
		{ \
			REGISTER_UNREGISTER_NABI_EVENT_HELPER(GetSystemUpdateEvent) \
			sink.disconnect<&system::Update>(this); \
		}
	#define UNREGISTER_SYSTEM_FIXED_UPDATE_EVENT_SUBSCRIBER(system) \
		{ \
			REGISTER_UNREGISTER_NABI_EVENT_HELPER(GetSystemFixedUpdateEvent) \
			sink.disconnect<&system::FixedUpdate>(this); \
		}
	#define UNREGISTER_SYSTEM_RENDER_EVENT_SUBSCRIBER(system) \
		{ \
			REGISTER_UNREGISTER_NABI_EVENT_HELPER(GetSystemRenderEvent) \
			sink.disconnect<&system::Render>(this); \
		}	
#else
	#define REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(system)   (void(0));
	#define REGISTER_SYSTEM_RENDER_EVENT_SUBSCRIBER(system)   (void(0));

	#define UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(system) (void(0));
	#define UNREGISTER_SYSTEM_RENDER_EVENT_SUBSCRIBER(system) (void(0));
#endif // ifdef USE_EVENT_SYSTEM_UPDATE

#define ENABLE_SYSTEM_UPDATE(system)        REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(system)
#define ENABLE_SYSTEM_FIXED_UPDATE(system)  REGISTER_SYSTEM_FIXED_UPDATE_EVENT_SUBSCRIBER(system)
#define ENABLE_SYSTEM_RENDER(system)        REGISTER_SYSTEM_RENDER_EVENT_SUBSCRIBER(system)
#define DISABLE_SYSTEM_UPDATE(system)       UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(system)
#define DISABLE_SYSTEM_FIXED_UPDATE(system) UNREGISTER_SYSTEM_FIXED_UPDATE_EVENT_SUBSCRIBER(system)
#define DISABLE_SYSTEM_RENDER(system)       UNREGISTER_SYSTEM_RENDER_EVENT_SUBSCRIBER(system)

#pragma endregion

#define REGISTER_UNREGISTER_NABI_EVENT_HELPER(eventGetter) \
		nabi::NabiEventsManager::SystemFrameEvent& sigh = m_Context.m_NabiEventsManager.eventGetter(); \
		entt::sink sink{ sigh };

namespace nabi
{
	class GameTime;
} // namespace nabi

namespace nabi
{
	/// <summary>
	/// Handles all core Nabi events. Eg, like those which update systems.
	/// </summary>
	class NabiEventsManager
	{
	public:
#ifdef USE_EVENT_SYSTEM_UPDATE
		typedef entt::sigh<void(GameTime const&)> SystemFrameEvent;

		inline void FireSystemUpdateEvent(GameTime const& gameTime)      const NABI_NOEXCEPT { m_UpdateEvent.publish(gameTime);      };
		inline void FireSystemFixedUpdateEvent(GameTime const& gameTime) const NABI_NOEXCEPT { m_FixedUpdateEvent.publish(gameTime); };
		inline void FireSystemRenderEvent(GameTime const& gameTime)      const NABI_NOEXCEPT { m_RenderEvent.publish(gameTime);      };

		inline SystemFrameEvent& GetSystemUpdateEvent()      NABI_NOEXCEPT { return m_UpdateEvent;      };
		inline SystemFrameEvent& GetSystemFixedUpdateEvent() NABI_NOEXCEPT { return m_FixedUpdateEvent; };
		inline SystemFrameEvent& GetSystemRenderEvent()      NABI_NOEXCEPT { return m_RenderEvent;      };
#endif // ifdef USE_EVENT_SYSTEM_UPDATE

	private:
#ifdef USE_EVENT_SYSTEM_UPDATE
		SystemFrameEvent m_UpdateEvent      = {};
		SystemFrameEvent m_FixedUpdateEvent = {};
		SystemFrameEvent m_RenderEvent      = {};
#endif // ifdef USE_EVENT_SYSTEM_UPDATE 
	};
} // namespace nabi
