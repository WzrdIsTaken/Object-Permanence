#pragma once
#include "Core.h"

#include <bitset>

#include "InputIncludes.h"

#include "ComponentBase.h"

namespace ecs::SComp
{
	struct KeyboardState
	{
		static size_t constexpr c_NumberOfKeys = 256u;

		std::bitset<c_NumberOfKeys> m_PendingKeyStates;
		std::bitset<c_NumberOfKeys> m_CurrentKeyStates;
		std::bitset<c_NumberOfKeys> m_PreviousKeyStates;
	};

	struct MouseState
	{
		static unsigned int constexpr c_NumberOfButtons = 3u;

		std::bitset<c_NumberOfButtons> m_PendingButtonStates;
		std::bitset<c_NumberOfButtons> m_CurrentButtonStates;
		std::bitset<c_NumberOfButtons> m_PreviousButtonStates;

		dx::XMFLOAT2 m_MousePosition;
	};

	struct ControllerState
	{
		struct Controller
		{
			bool m_Connected{};
			WORD m_Buttons{};
			std::array<float, 6> m_Axis{};
		};

		static float constexpr c_StickConversion = 32767.0f;
		static float constexpr c_TriggerConversion = 255.0f;
		static int constexpr c_MaxControllers = XUSER_MAX_COUNT;

		std::array<Controller, c_MaxControllers> m_CurrentControllerStates{};
		std::array<Controller, c_MaxControllers> m_PreviousControllerStates{};
	};

	struct InputStateComponent final : public nabi::ECS::ComponentBase
	{
		KeyboardState m_KeyboardState;
		MouseState m_MouseState;
		ControllerState m_ControllerState;

		InputStateComponent()
			: m_KeyboardState{}
			, m_MouseState{}
			, m_ControllerState{}
		{
		}
	};
} // namespace ecs::SComp
