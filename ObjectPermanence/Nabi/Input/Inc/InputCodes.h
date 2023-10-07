#pragma once
#include "EngineCore.h" 

#include "TypeUtils.h"

// Contains keyboard, mouse and controller key/button codes

namespace nabi::Input
{
	// Types

	enum class InputType : int
	{
		Invalid,

		Keyboard,
		Mouse,
		Controller,

		ENUM_COUNT
	};

	enum class Controller : unsigned int
	{
		Invalid,

		ControllerOne = 0u,
		ControllerTwo = 1u,
		ControllerThree = 2u,
		ControllerFour = 3u,
		MasterController = ControllerOne,

		ENUM_COUNT
	};

	// Codes

	enum class InputCode : int
	{
		Invalid,

		// --- Keys ---
		// I got all the keys from here https://wiki.nexusmods.com/index.php/DirectX_Scancodes_And_How_To_Use_Them

		Key_Space = 32,
		Key_Apostrophe = 39,
		Key_Comma = 44,
		Key_Minus = 45,
		Key_Period = 46,
		Key_Slash = 47,
		Key_Alpha0 = 48,
		Key_Alpha1 = 49,
		Key_Alpha2 = 50,
		Key_Alpha3 = 51,
		Key_Alpha4 = 52,
		Key_Alpha5 = 53,
		Key_Alpha6 = 54,
		Key_Alpha7 = 55,
		Key_Alpha8 = 56,
		Key_Alpha9 = 57,
		Key_Semicolon = 59,
		Key_Equal = 61,

		Key_A = 65,
		Key_B = 66,
		Key_C = 67,
		Key_D = 68,
		Key_E = 69,
		Key_F = 70,
		Key_G = 71,
		Key_H = 72,
		Key_I = 73,
		Key_J = 74,
		Key_K = 75,
		Key_L = 76,
		Key_M = 77,
		Key_N = 78,
		Key_O = 79,
		Key_P = 80,
		Key_Q = 81,
		Key_R = 82,
		Key_S = 83,
		Key_T = 84,
		Key_U = 85,
		Key_V = 86,
		Key_W = 87,
		Key_X = 88,
		Key_Y = 89,
		Key_Z = 90,

		Key_LeftBracket = 91,
		Key_Backslash = 92,
		Key_RightBracket = 93,
		Key_Grave = 96,
		Key_Escape = 256,
		Key_Enter = 257,
		Key_Tab = 258,
		Key_Backspace = 259,
		Key_Delete = 261,

		Key_RightArrow = 0x27,
		Key_LeftArrow = 0x25,
		Key_DownArrow = 0x28,
		Key_UpArrow = 0x26,

		// --- Mouse ---

		Mouse_LeftButton = 1,
		Mouse_RightButton = 2,

		// --- Controller ---

		// Buttons
		Controller_DPadUp = 0x0001,
		Controller_DPadDown = 0x0002,
		Controller_DPadLeft = 0x0004,
		Controller_DPadRight = 0x0008,
		Controller_Start = 0x0010,
		Controller_Back = 0x0020,
		Controller_LeftStickDown = 0x0040,
		Controller_RightStickDown = 0x0080,
		Controller_LeftShoulder = 0x0100,
		Controller_RightShoulder = 0x0200,
		Controller_A = 0x1000,
		Controller_B = 0x2000,
		Controller_X = 0x4000,
		Controller_Y = 0x8000,

		// Axes
		Controller_LeftStickX = 0u,
		Controller_LeftStickY = 1u,
		Controller_RightStickX = 2u,
		Controller_RightStickY = 3u,
		Controller_LeftTrigger = 4u,
		Controller_RightTrigger = 5u,

		// Sticks
		Controller_LeftStick = 0u,
		Controller_RightStick = 2u,

		ENUM_COUNT
	};

	// This is real unfortunate.. but we can't use a regular enum for this ): (mainly because of the reflection system)
	// F

	constexpr inline int GetInputType(InputType const inputType) NABI_NOEXCEPT
	{
		return nabi::TypeUtils::ToUnderlying<InputType>(inputType);
	}
	constexpr inline unsigned int GetController(Controller const controller) NABI_NOEXCEPT
	{
		return nabi::TypeUtils::ToUnderlying<Controller>(controller);
	}
	constexpr inline int GetInputCode(InputCode const inputCode) NABI_NOEXCEPT
	{
		return nabi::TypeUtils::ToUnderlying<InputCode>(inputCode);
	}
} // namespace nabi::Input

// If someone in the future is looking at this code and wants to structure there code differently than me, these archives might help!
// ...or they might be useful to me if I change my mind again xD

/*
Archive 1

// Keyboard
namespace nabi::Input::Keys
{
	int constexpr Space = 32;
    int constexpr Apostrophe = 39;
	int constexpr Comma = 44;
	int constexpr Minus = 45;
	int constexpr Period = 46;
	int constexpr Slash = 47;
	int constexpr Alpha0 = 48;
	int constexpr Alpha1 = 49;
	int constexpr Alpha2 = 50;
	int constexpr Alpha3 = 51;
	int constexpr Alpha4 = 52;
	int constexpr Alpha5 = 53;
	int constexpr Alpha6 = 54;
	int constexpr Alpha7 = 55;
	int constexpr Alpha8 = 56;
	int constexpr Alpha9 = 57;
	int constexpr Semicolon = 59;
	int constexpr Equal = 61;

	int constexpr A = 65;
	int constexpr B = 66;
	int constexpr C = 67;
	int constexpr D = 68;
	int constexpr E = 69;
	int constexpr F = 70;
	int constexpr G = 71;
	int constexpr H = 72;
	int constexpr I = 73;
	int constexpr J = 74;
	int constexpr K = 75;
	int constexpr L = 76;
	int constexpr M = 77;
	int constexpr N = 78;
	int constexpr O = 79;
	int constexpr P = 80;
	int constexpr Q = 81;
	int constexpr R = 82;
	int constexpr S = 83;
	int constexpr T = 84;
	int constexpr U = 85;
	int constexpr V = 86;
	int constexpr W = 87;
	int constexpr X = 88;
	int constexpr Y = 89;
	int constexpr Z = 90;

	int constexpr LeftBracket = 91;
	int constexpr Backslash = 92;
	int constexpr RightBracket = 93;
	int constexpr Grave = 96;
	int constexpr Escape = 256;
	int constexpr Enter = 257;
	int constexpr Tab = 258;
	int constexpr Backspace = 259;
	int constexpr Delete = 261;

	int constexpr RightArrow = 0x27;
	int constexpr LeftArrow = 0x25;
	int constexpr DownArrow = 0x28;
	int constexpr UpArrow = 0x26;
} // namespace nabi::Input::Keys

// Mouse
namespace nabi::Input::Mouse
{
	//int constexpr LeftMouseButton   = 256;
	//int constexpr RightMouseButton  = 257;
	//int constexpr MiddleMouseButton = 258;
	//int constexpr MouseButtonThree  = 259;
	//int constexpr MouseButtonFour   = 260;
	//int constexpr MouseButtonFive   = 261;
	//int constexpr MouseButtonSix    = 262;
	//int constexpr MouseButtonSeven  = 263;
	//int constexpr MouseWheelUp      = 264;
	//int constexpr MouseWheelDown    = 265;

	int constexpr LeftButton  = 1;
	int constexpr RightButton = 2;
} // namespace nabi::Input::Mouse

// Controller
namespace nabi::Input::Controller
{
	unsigned int constexpr ControllerOne = 0u;
	unsigned int constexpr ControllerTwo = 1u;
	unsigned int constexpr ControllerThree = 2u;
	unsigned int constexpr ControllerFour = 3u;
	unsigned int constexpr MasterController = ControllerOne;

	namespace Buttons
	{
		int constexpr DPadUp = 0x0001;
		int constexpr DPadDown = 0x0002;
		int constexpr DPadLeft = 0x0004;
		int constexpr DPadRight = 0x0008;
		int constexpr Start = 0x0010;
		int constexpr Back = 0x0020;
		int constexpr LeftStick = 0x0040;
		int constexpr RightStick = 0x0080;
		int constexpr LeftShoulder = 0x0100;
		int constexpr RightShoulder = 0x0200;
		int constexpr A = 0x1000;
		int constexpr B = 0x2000;
		int constexpr X = 0x4000;
		int constexpr Y = 0x8000;
	} // namespace Buttons

	namespace Axes
	{
		unsigned int constexpr LeftStickX = 0u;
		unsigned int constexpr LeftStickY = 1u;
		unsigned int constexpr RightStickX = 2u;
		unsigned int constexpr RightStickY = 3u;
		unsigned int constexpr LeftTrigger = 4u;
		unsigned int constexpr RightTrigger = 5u;
	} // namespace Axes

	namespace Sticks
	{
		unsigned int constexpr Left = 0u;
		unsigned int constexpr Right = 2u;
	} // namespace Sticks
}
*/

/*
Archive 2

// enum rather than enum class so they can implicity converted to the underlying type

namespace InputType
{
	enum Enum : int
	{
		Invalid,

		Keyboard,
		Mouse,
		Controller,

		ENUM_COUNT
	};
} // namespace InputType

// Keyboard

namespace Keys
{
	enum Enum : int
	{
		// I got all the keys from here https://wiki.nexusmods.com/index.php/DirectX_Scancodes_And_How_To_Use_Them

		Space = 32,
		Apostrophe = 39,
		Comma = 44,
		Minus = 45,
		Period = 46,
		Slash = 47,
		Alpha0 = 48,
		Alpha1 = 49,
		Alpha2 = 50,
		Alpha3 = 51,
		Alpha4 = 52,
		Alpha5 = 53,
		Alpha6 = 54,
		Alpha7 = 55,
		Alpha8 = 56,
		Alpha9 = 57,
		Semicolon = 59,
		Equal = 61,

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,
		Backslash = 92,
		RightBracket = 93,
		Grave = 96,
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Delete = 261,

		RightArrow = 0x27,
		LeftArrow = 0x25,
		DownArrow = 0x28,
		UpArrow = 0x26
	};
} // namespace Keys

// Mouse

namespace Mouse
{
	enum Enum : int
	{
		LeftButton = 1,
		RightButton = 2
	};
} // namespace Mouse

// Controller

namespace Controller
{
	enum Enum : unsigned int
	{
		ControllerOne = 0u,
		ControllerTwo = 1u,
		ControllerThree = 2u,
		ControllerFour = 3u,
		MasterController = ControllerOne
	};
} // namespace Controller

namespace ControllerButtons
{
	enum Enum : int
	{
		DPadUp = 0x0001,
		DPadDown = 0x0002,
		DPadLeft = 0x0004,
		DPadRight = 0x0008,
		Start = 0x0010,
		Back = 0x0020,
		LeftStick = 0x0040,
		RightStick = 0x0080,
		LeftShoulder = 0x0100,
		RightShoulder = 0x0200,
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000
	};
} // namespace ControllerButtons

namespace ControllerAxes
{
	enum Enum : unsigned int
	{
		LeftStickX = 0u,
		LeftStickY = 1u,
		RightStickX = 2u,
		RightStickY = 3u,
		LeftTrigger = 4u,
		RightTrigger = 5u
	};
} // namespace ControllerAxes

namespace ControllerSticks
{
	enum Enum : unsigned int
	{
		Left = 0u,
		Right = 2u
	};
} // namespace ControllerSticks
*/
