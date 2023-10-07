#pragma once
#include "EngineCore.h"

#include "DirectXIncludes.h"

#include "InitSettings.h"

namespace nabi
{
	struct Context;
} // namespace nabi

namespace nabi
{
	/// <summary>
	/// Listens and handles events to do with the window itself (eg, resizing)
	/// </summary>
	class WindowEventsListener
	{
	public:
		WindowEventsListener(Context const& context, WindowSettings const& settings) NABI_NOEXCEPT;

		void RegisterEvents() NABI_NOEXCEPT;
		void UnRegisterEvents() NABI_NOEXCEPT;

		[[nodiscard]] inline bool InSizeMove () const NABI_NOEXCEPT { return m_InSizeMove;  };
		[[nodiscard]] inline bool IsMinimized() const NABI_NOEXCEPT { return m_IsMinimized; };

	private:
		// --- Window resizing ---
		// Windows events
		void OnSize(WPARAM const wParam, LPARAM const lParam) NABI_NOEXCEPT;
		void OnEnterSizeMove(WPARAM const wParam, LPARAM const lParam) NABI_NOEXCEPT;
		void OnExitSizeMove(WPARAM const wParam, LPARAM const lParam) NABI_NOEXCEPT;
		void OnGetMinMaxInfo(WPARAM const wParam, LPARAM const lParam) const NABI_NOEXCEPT;
		// Helper
		void ExtractNewSizeAndUpdateWindow(LPARAM const lParam) const NABI_NOEXCEPT;

		Context const& m_Context;
		WindowSettings const m_WindowSettings;

		LPARAM m_LastLPARAM; // LPARAM contains the window size

		bool m_InSizeMove;
		bool m_IsMinimized;
	};
} // namespace nabi
