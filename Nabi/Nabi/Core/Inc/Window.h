#pragma once
#include "EngineCore.h"

#include <optional>

#include "WinIncludes.h"

#include "entt.h"

#include "InitSettings.h"
#include "TypeUtils.h"

// Macros for registering a windows event. For example key input or resizing. 
// Use REGISTER_WINDOWS_EVENT_SUBSCRIBER in constructor and UNREGISTER_WINDOWS_EVENT_SUBSCRIBER in destructor
#define REGISTER_WINDOWS_EVENT_SUBSCRIBER(msg, subscriber) \
	{ \
		REGISTER_UNREGISTER_WINDOWS_EVENT_HELPER(msg) \
		sink.connect<&subscriber>(this); \
	}
#define UNREGISTER_WINDOWS_EVENT_SUBSCRIBER(msg, subscriber) \
	{ \
		REGISTER_UNREGISTER_WINDOWS_EVENT_HELPER(msg) \
		sink.disconnect<&subscriber>(this); \
	}
#define REGISTER_UNREGISTER_WINDOWS_EVENT_HELPER(msg) \
	nabi::Window::WindowsMsg& sigh = m_Context.m_Window->GetOrAddEvent(msg); \
	entt::sink sink{ sigh };

namespace nabi
{
	/// <summary>
	/// The main game window.
	/// </summary>
	class Window final
	{
	public:
		typedef entt::sigh<void(WPARAM, LPARAM)> WindowsMsg; 

		Window(HINSTANCE const hInstance, WindowSettings const& settings) NABI_NOEXCEPT;
		~Window();

		/// <summary>
		/// Creates a WindowsMsgPair in m_WindowsEvents if a pair with the id messageId doesn't exist, else returns the existing one.
		/// There will only be one WindowsMsgPair per messageId, an events can subscribe to it.
		/// For convenience, just use the REGISTER_WINDOWS_EVENT_SUBSCRIBER macro above.
		/// </summary>
		/// <param name="messageId">- The id to get or create a WindowsMsgPair for.</param>
		/// <returns>The WindowsMsg for messageId</returns>
		[[nodiscard]] WindowsMsg& GetOrAddEvent(UINT const messageId) NABI_NOEXCEPT;
		/// <summary>
		/// Removes a WindowsMsgPair from m_WindowsEvents if a pair with messageId exists.
		/// For convenience, just use the UNREGISTER_WINDOWS_EVENT_SUBSCRIBER macro above.
		/// </summary>
		/// <param name="messageId">- The WindowsMsgPair to remove</param>
		/// <returns>If the WindowsMsgPair was successfully removed</returns>
		bool RemoveEvent(UINT const messageId) NABI_NOEXCEPT;

		/// <summary>
		/// The main message loop. 
		/// This function is static because it processes messages for all windows
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] static std::optional<int> ProcessMessages() NABI_NOEXCEPT;
		/// <summary>
		/// Returns a const ptr to the window
		/// </summary>
		[[nodiscard]] HWND const GetHWND() const NABI_NOEXCEPT;

		/// <summary>
		/// Sets the window title
		/// </summary>
		void SetWindowTitle(std::wstring const& windowName) const NABI_NOEXCEPT;
		/// <summary>
		/// Returns the sizeof the window
		/// </summary>
		POINT GetWindowSize() const NABI_NOEXCEPT;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(Window)

		struct WindowsMsgPair;
		typedef std::vector<WindowsMsgPair>::iterator WindowMsgItr;

		// Message id - message signal. When the signal is published all subscribers are notified.
		struct WindowsMsgPair
		{
			UINT m_Msg;
			WindowsMsg m_Event;
		};

		// Some jank thing I will never do again
		enum class FindMode : int
		{
			Find,
			Remove,
			ENUM_COUNT
		};

		// Helper functions for setting up the message pump. Again static because they handle this for all windows.
		[[nodiscard]] static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) NABI_NOEXCEPT;
		[[nodiscard]] static LRESULT CALLBACK HandleMsgProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) NABI_NOEXCEPT;

		// Main message processing function. Forwards messages onto event subscribers.
		[[nodiscard]] LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) NABI_NOEXCEPT;

		// Helper functions for GetOrAddEvent and RemoveEvent
		[[nodiscard]] WindowMsgItr FindMsgItr(UINT const messageId, FindMode const findMode) NABI_NOEXCEPT;
		[[nodiscard]] inline bool IsMsgItrValid(WindowMsgItr const itr) const NABI_NOEXCEPT { return itr != m_WindowsEvents.end(); }

		HWND m_hWnd;
		std::vector<WindowsMsgPair> m_WindowsEvents;

		HINSTANCE const c_hInstance;
		LPCWSTR const c_WindowClassName;
	};
} // namespace nabi::Rendering
