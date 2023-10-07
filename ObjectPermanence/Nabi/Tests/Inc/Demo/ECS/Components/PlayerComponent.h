#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "ComponentBase.h"
#include "InputCodes.h"

namespace ecs
{
	struct PlayerComponent final : public nabi::ECS::ComponentBase
	{
		nabi::Input::InputCode m_ForwardKey;
		nabi::Input::InputCode m_BackwardKey;
		nabi::Input::InputCode m_LeftKey;
		nabi::Input::InputCode m_RightKey;
		nabi::Input::InputCode m_UpKey;
		nabi::Input::InputCode m_DownKey;

		nabi::Input::InputCode m_RotateForwardKey;
		nabi::Input::InputCode m_RotateBackwardKey;
		nabi::Input::InputCode m_RotateLeftKey;
		nabi::Input::InputCode m_RotateRightKey;

		nabi::Input::InputCode m_RaycastKey;

		float m_Speed;
		float m_RotateSpeed;

		PlayerComponent()
			: m_ForwardKey(nabi::Input::InputCode::Key_W)
			, m_BackwardKey(nabi::Input::InputCode::Key_S)
			, m_LeftKey(nabi::Input::InputCode::Key_A)
			, m_RightKey(nabi::Input::InputCode::Key_D)
			, m_UpKey(nabi::Input::InputCode::Key_Q)
			, m_DownKey(nabi::Input::InputCode::Key_E)
			, m_RotateForwardKey(nabi::Input::InputCode::Key_UpArrow)
			, m_RotateBackwardKey(nabi::Input::InputCode::Key_DownArrow)
			, m_RotateLeftKey(nabi::Input::InputCode::Key_LeftArrow)
			, m_RotateRightKey(nabi::Input::InputCode::Key_RightArrow)
			, m_RaycastKey(nabi::Input::InputCode::Key_R)
			, m_Speed(10.0f)
			, m_RotateSpeed(10.0f)
		{
		}
	};
} // namespace ecs

#endif // ifdef INCLUDE_DEMO
