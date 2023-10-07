#include "EngineCore.h"

#include "InputCodes.h"

#include "ReflectionIncludes.h"

namespace nabi::Input
{
	// Types

	REFLECT_ENUM_BEGIN(InputType, "InputType")
		REFLECT_ENUM_VALUE(InputType::Keyboard, "Keyboard")
		REFLECT_ENUM_VALUE(InputType::Mouse, "Mouse")
		REFLECT_ENUM_VALUE(InputType::Controller, "Controller")
	REFLECT_ENUM_END(InputType)

	REFLECT_ENUM_BEGIN(Controller, "Controller")
		REFLECT_ENUM_VALUE(Controller::ControllerOne, "ControllerOne")
		REFLECT_ENUM_VALUE(Controller::ControllerTwo, "ControllerTwo")
		REFLECT_ENUM_VALUE(Controller::ControllerThree, "ControllerThree")
		REFLECT_ENUM_VALUE(Controller::ControllerFour, "ControllerFour")
		REFLECT_ENUM_VALUE(Controller::MasterController, "MasterController")
	REFLECT_ENUM_END(Controller)

	REFLECT_ENUM_BEGIN(InputCode, "InputCode")
		// Keys

		REFLECT_ENUM_VALUE(InputCode::Key_Space, "Key_Space")
		REFLECT_ENUM_VALUE(InputCode::Key_Apostrophe, "Key_Apostrophe")
		REFLECT_ENUM_VALUE(InputCode::Key_Comma, "Key_Comma")
		REFLECT_ENUM_VALUE(InputCode::Key_Minus, "Key_Minus")
		REFLECT_ENUM_VALUE(InputCode::Key_Period, "Key_Period")
		REFLECT_ENUM_VALUE(InputCode::Key_Slash, "Key_Slash")
		REFLECT_ENUM_VALUE(InputCode::Key_Alpha0, "Key_Alpha0")
		REFLECT_ENUM_VALUE(InputCode::Key_Alpha1, "Key_Alpha1")
		REFLECT_ENUM_VALUE(InputCode::Key_Alpha2, "Key_Alpha2")
		REFLECT_ENUM_VALUE(InputCode::Key_Alpha3, "Key_Alpha3")
		REFLECT_ENUM_VALUE(InputCode::Key_Alpha4, "Key_Alpha4")
		REFLECT_ENUM_VALUE(InputCode::Key_Alpha5, "Key_Alpha5")
		REFLECT_ENUM_VALUE(InputCode::Key_Alpha6, "Key_Alpha6")
		REFLECT_ENUM_VALUE(InputCode::Key_Alpha7, "Key_Alpha7")
		REFLECT_ENUM_VALUE(InputCode::Key_Alpha8, "Key_Alpha8")
		REFLECT_ENUM_VALUE(InputCode::Key_Alpha9, "Key_Alpha9")
		REFLECT_ENUM_VALUE(InputCode::Key_Semicolon, "Key_Semicolon")
		REFLECT_ENUM_VALUE(InputCode::Key_Equal, "Key_Equal")

		REFLECT_ENUM_VALUE(InputCode::Key_A, "Key_A")
		REFLECT_ENUM_VALUE(InputCode::Key_B, "Key_B")
		REFLECT_ENUM_VALUE(InputCode::Key_C, "Key_C")
		REFLECT_ENUM_VALUE(InputCode::Key_D, "Key_D")
		REFLECT_ENUM_VALUE(InputCode::Key_E, "Key_E")
		REFLECT_ENUM_VALUE(InputCode::Key_F, "Key_F")
		REFLECT_ENUM_VALUE(InputCode::Key_G, "Key_G")
		REFLECT_ENUM_VALUE(InputCode::Key_H, "Key_H")
		REFLECT_ENUM_VALUE(InputCode::Key_I, "Key_I")
		REFLECT_ENUM_VALUE(InputCode::Key_J, "Key_J")
		REFLECT_ENUM_VALUE(InputCode::Key_K, "Key_K")
		REFLECT_ENUM_VALUE(InputCode::Key_L, "Key_L")
		REFLECT_ENUM_VALUE(InputCode::Key_M, "Key_M")
		REFLECT_ENUM_VALUE(InputCode::Key_N, "Key_N")
		REFLECT_ENUM_VALUE(InputCode::Key_O, "Key_O")
		REFLECT_ENUM_VALUE(InputCode::Key_P, "Key_P")
		REFLECT_ENUM_VALUE(InputCode::Key_Q, "Key_Q")
		REFLECT_ENUM_VALUE(InputCode::Key_R, "Key_R")
		REFLECT_ENUM_VALUE(InputCode::Key_S, "Key_S")
		REFLECT_ENUM_VALUE(InputCode::Key_T, "Key_T")
		REFLECT_ENUM_VALUE(InputCode::Key_U, "Key_U")
		REFLECT_ENUM_VALUE(InputCode::Key_V, "Key_V")
		REFLECT_ENUM_VALUE(InputCode::Key_W, "Key_W")
		REFLECT_ENUM_VALUE(InputCode::Key_X, "Key_X")
		REFLECT_ENUM_VALUE(InputCode::Key_Y, "Key_Y")
		REFLECT_ENUM_VALUE(InputCode::Key_Z, "Key_Z")

		REFLECT_ENUM_VALUE(InputCode::Key_LeftBracket, "Key_LeftBracket")
		REFLECT_ENUM_VALUE(InputCode::Key_Backslash, "Key_Backslash")
		REFLECT_ENUM_VALUE(InputCode::Key_RightBracket, "Key_RightBracket")
		REFLECT_ENUM_VALUE(InputCode::Key_Grave, "Key_Grave")
		REFLECT_ENUM_VALUE(InputCode::Key_Escape, "Key_Escape")
		REFLECT_ENUM_VALUE(InputCode::Key_Enter, "Key_Enter")
		REFLECT_ENUM_VALUE(InputCode::Key_Tab, "Key_Tab")
		REFLECT_ENUM_VALUE(InputCode::Key_Backspace, "Key_Backspace")
		REFLECT_ENUM_VALUE(InputCode::Key_Delete, "Key_Delete")

		REFLECT_ENUM_VALUE(InputCode::Key_RightArrow, "Key_RightArrow")
		REFLECT_ENUM_VALUE(InputCode::Key_LeftArrow, "Key_LeftArrow")
		REFLECT_ENUM_VALUE(InputCode::Key_DownArrow, "Key_DownArrow")
		REFLECT_ENUM_VALUE(InputCode::Key_UpArrow, "Key_UpArrow")

		// Mouse

		REFLECT_ENUM_VALUE(InputCode::Mouse_LeftButton, "Mouse_LeftButton")
		REFLECT_ENUM_VALUE(InputCode::Mouse_RightButton, "Mouse_RightButton")

		// Controller

		REFLECT_ENUM_VALUE(InputCode::Controller_DPadUp, "Controller_DPadUp")
		REFLECT_ENUM_VALUE(InputCode::Controller_DPadDown, "Controller_DPadDown")
		REFLECT_ENUM_VALUE(InputCode::Controller_DPadLeft, "Controller_DPadLeft")
		REFLECT_ENUM_VALUE(InputCode::Controller_DPadRight, "Controller_DPadRight")
		REFLECT_ENUM_VALUE(InputCode::Controller_Start, "Controller_Start")
		REFLECT_ENUM_VALUE(InputCode::Controller_Back, "Controller_Back")
		REFLECT_ENUM_VALUE(InputCode::Controller_LeftStickDown, "Controller_LeftStickDown")
		REFLECT_ENUM_VALUE(InputCode::Controller_RightStickDown, "Controller_RightStickDown")
		REFLECT_ENUM_VALUE(InputCode::Controller_LeftShoulder, "Controller_LeftShoulder")
		REFLECT_ENUM_VALUE(InputCode::Controller_RightShoulder, "Controller_RightShoulder")
		REFLECT_ENUM_VALUE(InputCode::Controller_A, "Controller_A")
		REFLECT_ENUM_VALUE(InputCode::Controller_B, "Controller_B")
		REFLECT_ENUM_VALUE(InputCode::Controller_X, "Controller_X")
		REFLECT_ENUM_VALUE(InputCode::Controller_Y, "Controller_Y")

		REFLECT_ENUM_VALUE(InputCode::Controller_LeftStickX, "Controller_LeftStickX")
		REFLECT_ENUM_VALUE(InputCode::Controller_LeftStickY, "Controller_LeftStickY")
		REFLECT_ENUM_VALUE(InputCode::Controller_RightStickX, "Controller_RightStickX")
		REFLECT_ENUM_VALUE(InputCode::Controller_RightStickY, "Controller_RightStickY")

		REFLECT_ENUM_VALUE(InputCode::Controller_LeftTrigger, "Controller_LeftTrigger")
		REFLECT_ENUM_VALUE(InputCode::Controller_RightTrigger, "Controller_RightTrigger")
	REFLECT_ENUM_END(InputCode)

	// Codes
} // namespace nabi::Input

/*
Archive 2 reflection

// Core
namespace InputType
{
	REFLECT_ENUM_BEGIN(Enum, "InputType")
		REFLECT_ENUM_VALUE(InputType::Keyboard, "Keyboard")
		REFLECT_ENUM_VALUE(InputType::Mouse, "Mouse")
		REFLECT_ENUM_VALUE(InputType::Controller, "Controller")
	REFLECT_ENUM_END(Enum)
} // namespace InputType

// Keys

namespace Keys
{
	REFLECT_ENUM_BEGIN(Enum, "Keys")
		REFLECT_ENUM_VALUE(Keys::Space, "Space")
		REFLECT_ENUM_VALUE(Keys::Apostrophe, "Apostrophe")
		REFLECT_ENUM_VALUE(Keys::Comma, "Comma")
		REFLECT_ENUM_VALUE(Keys::Minus, "Minus")
		REFLECT_ENUM_VALUE(Keys::Period, "Period")
		REFLECT_ENUM_VALUE(Keys::Slash, "Slash")
		REFLECT_ENUM_VALUE(Keys::Alpha0, "Alpha0")
		REFLECT_ENUM_VALUE(Keys::Alpha1, "Alpha1")
		REFLECT_ENUM_VALUE(Keys::Alpha2, "Alpha2")
		REFLECT_ENUM_VALUE(Keys::Alpha3, "Alpha3")
		REFLECT_ENUM_VALUE(Keys::Alpha4, "Alpha4")
		REFLECT_ENUM_VALUE(Keys::Alpha5, "Alpha5")
		REFLECT_ENUM_VALUE(Keys::Alpha6, "Alpha6")
		REFLECT_ENUM_VALUE(Keys::Alpha7, "Alpha7")
		REFLECT_ENUM_VALUE(Keys::Alpha8, "Alpha8")
		REFLECT_ENUM_VALUE(Keys::Alpha9, "Alpha9")
		REFLECT_ENUM_VALUE(Keys::Semicolon, "Semicolon")
		REFLECT_ENUM_VALUE(Keys::Equal, "Equal")

		REFLECT_ENUM_VALUE(Keys::A, "A")
		REFLECT_ENUM_VALUE(Keys::B, "B")
		REFLECT_ENUM_VALUE(Keys::C, "C")
		REFLECT_ENUM_VALUE(Keys::D, "D")
		REFLECT_ENUM_VALUE(Keys::E, "E")
		REFLECT_ENUM_VALUE(Keys::F, "F")
		REFLECT_ENUM_VALUE(Keys::G, "G")
		REFLECT_ENUM_VALUE(Keys::H, "H")
		REFLECT_ENUM_VALUE(Keys::I, "I")
		REFLECT_ENUM_VALUE(Keys::J, "J")
		REFLECT_ENUM_VALUE(Keys::K, "K")
		REFLECT_ENUM_VALUE(Keys::L, "L")
		REFLECT_ENUM_VALUE(Keys::M, "M")
		REFLECT_ENUM_VALUE(Keys::N, "N")
		REFLECT_ENUM_VALUE(Keys::O, "O")
		REFLECT_ENUM_VALUE(Keys::P, "P")
		REFLECT_ENUM_VALUE(Keys::Q, "Q")
		REFLECT_ENUM_VALUE(Keys::R, "R")
		REFLECT_ENUM_VALUE(Keys::S, "S")
		REFLECT_ENUM_VALUE(Keys::T, "T")
		REFLECT_ENUM_VALUE(Keys::U, "U")
		REFLECT_ENUM_VALUE(Keys::V, "V")
		REFLECT_ENUM_VALUE(Keys::W, "W")
		REFLECT_ENUM_VALUE(Keys::X, "X")
		REFLECT_ENUM_VALUE(Keys::Y, "Y")
		REFLECT_ENUM_VALUE(Keys::Z, "Z")

		REFLECT_ENUM_VALUE(Keys::LeftBracket, "LeftBracket")
		REFLECT_ENUM_VALUE(Keys::Backslash, "Backslash")
		REFLECT_ENUM_VALUE(Keys::RightBracket, "RightBracket")
		REFLECT_ENUM_VALUE(Keys::Grave, "Grave")
		REFLECT_ENUM_VALUE(Keys::Escape, "Escape")
		REFLECT_ENUM_VALUE(Keys::Enter, "Enter")
		REFLECT_ENUM_VALUE(Keys::Tab, "Tab")
		REFLECT_ENUM_VALUE(Keys::Backspace, "Backspace")
		REFLECT_ENUM_VALUE(Keys::Delete, "Delete")

		REFLECT_ENUM_VALUE(Keys::RightArrow, "RightArrow")
		REFLECT_ENUM_VALUE(Keys::LeftArrow, "LeftArrow")
		REFLECT_ENUM_VALUE(Keys::DownArrow, "DownArrow")
		REFLECT_ENUM_VALUE(Keys::UpArrow, "UpArrow")
	REFLECT_ENUM_END(Enum)
} // namespace Keys

// Mouse

namespace Mouse
{
	REFLECT_ENUM_BEGIN(Enum, "Mouse")
		REFLECT_ENUM_VALUE(Mouse::LeftButton, "LeftButton")
		REFLECT_ENUM_VALUE(Mouse::RightButton, "RightButton")
	REFLECT_ENUM_END(Enum)
} // namespace Mouse

// Controller

namespace Controller
{
	REFLECT_ENUM_BEGIN(Enum, "Controller")
		REFLECT_ENUM_VALUE(Controller::ControllerOne, "ControllerOne")
		REFLECT_ENUM_VALUE(Controller::ControllerTwo, "ControllerTwo")
		REFLECT_ENUM_VALUE(Controller::ControllerThree, "ControllerThree")
		REFLECT_ENUM_VALUE(Controller::ControllerFour, "ControllerFour")
		REFLECT_ENUM_VALUE(Controller::MasterController, "MasterController")
	REFLECT_ENUM_END(Enum)
} // namespace Controller

namespace ControllerButtons
{
	REFLECT_ENUM_BEGIN(Enum, "ControllerButtons")
		REFLECT_ENUM_VALUE(ControllerButtons::DPadUp, "DPadUp")
		REFLECT_ENUM_VALUE(ControllerButtons::DPadDown, "DPadDown")
		REFLECT_ENUM_VALUE(ControllerButtons::DPadLeft, "DPadLeft")
		REFLECT_ENUM_VALUE(ControllerButtons::DPadRight, "DPadRight")
		REFLECT_ENUM_VALUE(ControllerButtons::Start, "Start")
		REFLECT_ENUM_VALUE(ControllerButtons::Back, "Back")
		REFLECT_ENUM_VALUE(ControllerButtons::LeftStick, "LeftStick")
		REFLECT_ENUM_VALUE(ControllerButtons::RightStick, "RightStick")
		REFLECT_ENUM_VALUE(ControllerButtons::LeftShoulder, "LeftShoulder")
		REFLECT_ENUM_VALUE(ControllerButtons::RightShoulder, "RightShoulder")
		REFLECT_ENUM_VALUE(ControllerButtons::A, "A")
		REFLECT_ENUM_VALUE(ControllerButtons::B, "B")
		REFLECT_ENUM_VALUE(ControllerButtons::X, "X")
		REFLECT_ENUM_VALUE(ControllerButtons::Y, "Y")
	REFLECT_ENUM_END(Enum)
} // namespace ControllerButtons

namespace ControllerAxes
{
	REFLECT_ENUM_BEGIN(Enum, "ControllerAxes")
		REFLECT_ENUM_VALUE(ControllerAxes::LeftStickX, "LeftStickX")
		REFLECT_ENUM_VALUE(ControllerAxes::LeftStickY, "LeftStickY")
		REFLECT_ENUM_VALUE(ControllerAxes::RightStickX, "RightStickX")
		REFLECT_ENUM_VALUE(ControllerAxes::RightStickY, "RightStickY")
		REFLECT_ENUM_VALUE(ControllerAxes::LeftTrigger, "LeftTrigger")
		REFLECT_ENUM_VALUE(ControllerAxes::RightTrigger, "RightTrigger")
	REFLECT_ENUM_END(Enum)
} // namespace ControllerAxes

namespace ControllerSticks
{
	REFLECT_ENUM_BEGIN(Enum, "ControllerSticks")
		REFLECT_ENUM_VALUE(ControllerSticks::Left, "Left")
		REFLECT_ENUM_VALUE(ControllerSticks::Right, "Right")
	REFLECT_ENUM_END(Enum)
} // namespace ControllerSticks
*/
