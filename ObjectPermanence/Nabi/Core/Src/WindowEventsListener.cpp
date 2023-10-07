#include "EngineCore.h"

#include "WinIncludes.h"

#include "WindowEventsListener.h"

#include "Context.h"
#include "DebugUtils.h"

namespace nabi
{
	WindowEventsListener::WindowEventsListener(Context const& context, WindowSettings const& settings) NABI_NOEXCEPT
		: m_Context(context)
		, m_WindowSettings(settings)
		, m_LastLPARAM(0)
		, m_InSizeMove(false)
		, m_IsMinimized(false)
	{
	}

	void WindowEventsListener::RegisterEvents() NABI_NOEXCEPT
	{
		// Window resizing 
		REGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_SIZE, WindowEventsListener::OnSize);
		REGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_ENTERSIZEMOVE, WindowEventsListener::OnEnterSizeMove);
		REGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_EXITSIZEMOVE, WindowEventsListener::OnExitSizeMove);
		REGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_GETMINMAXINFO, WindowEventsListener::OnGetMinMaxInfo);
	}

	void WindowEventsListener::UnRegisterEvents() NABI_NOEXCEPT
	{
		// Window resizing
		UNREGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_SIZE, WindowEventsListener::OnSize);
		UNREGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_ENTERSIZEMOVE, WindowEventsListener::OnEnterSizeMove);
		UNREGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_EXITSIZEMOVE, WindowEventsListener::OnExitSizeMove);
		UNREGISTER_WINDOWS_EVENT_SUBSCRIBER(WM_GETMINMAXINFO, WindowEventsListener::OnGetMinMaxInfo);
	}

	void WindowEventsListener::OnSize(WPARAM const wParam, LPARAM const lParam) NABI_NOEXCEPT
	{
		// Cache the last lParam. This is because when OnExitSizeMove is called its lParam will be 0
		m_LastLPARAM = lParam;

		// The window was minimized
		if (wParam == SIZE_MINIMIZED)
		{
			m_IsMinimized = true;
		}
		// The window was minimized and is now restored (resume from suspend)
		else if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
		{
			m_IsMinimized = false;
		}

		// The user has maximised / minimised the window - resize the swapchain
		if (!m_InSizeMove)
		{
			ExtractNewSizeAndUpdateWindow(m_LastLPARAM);
		}
	}

	void WindowEventsListener::OnEnterSizeMove(WPARAM const wParam, LPARAM const lParam) NABI_NOEXCEPT
	{
		// If the user is currently resizing the window, don't want to update the swapchain until they are done
		m_InSizeMove = true;
	}

	void WindowEventsListener::OnExitSizeMove(WPARAM const wParam, LPARAM const lParam) NABI_NOEXCEPT
	{
		// The window's size is no longer updating - resize the swapchain
		m_InSizeMove = false;
		ExtractNewSizeAndUpdateWindow(m_LastLPARAM);
	}

	void WindowEventsListener::OnGetMinMaxInfo(WPARAM const wParam, LPARAM const lParam) const NABI_NOEXCEPT
	{
		// Prevent the window from getting too small
		MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(lParam);
		info->ptMinTrackSize.x = m_WindowSettings.m_MinWidth;
		info->ptMinTrackSize.y = m_WindowSettings.m_MinHeight;
	}

	void WindowEventsListener::ExtractNewSizeAndUpdateWindow(LPARAM const lParam) const NABI_NOEXCEPT
	{
		UINT const newWidth  = LOWORD(lParam);
		UINT const newHeight = HIWORD(lParam);

		m_Context.m_RenderCommand->UpdateWindowSize(newWidth, newHeight);
	}
} // namespace nabi
