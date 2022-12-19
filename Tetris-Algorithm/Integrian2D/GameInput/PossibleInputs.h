#pragma once

#ifdef max /* Undefine the max() macro since it's fucking the entire project somehow */
#undef max
#endif

#include <limits> /* looks safe */
#include <stdint.h> /* should be safe */
// #include <SDL.h> /* this is not safe */

namespace Integrian2D
{
	enum class MouseButton : uint8_t
	{
		//#define SDL_BUTTON(X)       (1 << ((X)-1))
		//#define SDL_BUTTON_LEFT     1
		//#define SDL_BUTTON_MIDDLE   2
		//#define SDL_BUTTON_RIGHT    3
		// 0000 0001 << ((1) - 1) ===> 0000 0001 << (0) ===> 0000 0001		// SDL_BUTTON(SDL_BUTTON_LEFT) == 1
		// 0000 0001 << ((2) - 1) ===> 0000 0001 << (1) ===> 0000 0010		// SDL_BUTTON(SDL_BUTTON_MIDDLE) == 2
		// 0000 0001 << ((3) - 1) ===> 0000 0001 << (2) ===> 0000 0100		// SDL_BUTTON(SDL_BUTTON_RIGHT) == 4
		INVALID = std::numeric_limits<uint8_t>::max(), // internal usage
		LMB = 0x0001, // SDL_BUTTON(SDL_BUTTON_LEFT), // 0x0001 
		MMB = 0x0002, // SDL_BUTTON(SDL_BUTTON_MIDDLE), // 0x0002
		RMB = 0x0004, // SDL_BUTTON(SDL_BUTTON_RIGHT), // 0x0004
		LMBAndMMB = 0x0003, // SDL_BUTTON(SDL_BUTTON_LEFT) | SDL_BUTTON(SDL_BUTTON_MIDDLE), // 0x0003
		LMBAndRMB = 0x0005, // SDL_BUTTON(SDL_BUTTON_LEFT) | SDL_BUTTON(SDL_BUTTON_RIGHT), // 0x0005
		RMBandMMB = 0x0006, // SDL_BUTTON(SDL_BUTTON_RIGHT) | SDL_BUTTON(SDL_BUTTON_MIDDLE), // 0x0006
	};

	enum class KeyboardInput : uint32_t
	{
		INVALID = std::numeric_limits<uint32_t>::max(), // internal usage
		A = 4, // SDL_SCANCODE_A,
		B = 5, // SDL_SCANCODE_B,
		C = 6, // SDL_SCANCODE_C,
		D = 7, // SDL_SCANCODE_D,
		E = 8, // SDL_SCANCODE_E,
		F = 9, // SDL_SCANCODE_F,
		G = 10, // SDL_SCANCODE_G,
		H = 11, // SDL_SCANCODE_H,
		I = 12, // SDL_SCANCODE_I,
		J = 13, // SDL_SCANCODE_J,
		K = 14, // SDL_SCANCODE_K,
		L = 15, // SDL_SCANCODE_L,
		M = 16, // SDL_SCANCODE_M,
		N = 17, // SDL_SCANCODE_N,
		O = 18, // SDL_SCANCODE_O,
		P = 19, // SDL_SCANCODE_P,
		Q = 20, // SDL_SCANCODE_Q,
		R = 21, // SDL_SCANCODE_R,
		S = 22, // SDL_SCANCODE_S,
		T = 23, // SDL_SCANCODE_T,
		U = 24, // SDL_SCANCODE_U,
		V = 25, // SDL_SCANCODE_V,
		W = 26, // SDL_SCANCODE_W,
		X = 27, // SDL_SCANCODE_X,
		Y = 28, // SDL_SCANCODE_Y,
		Z = 29, // SDL_SCANCODE_Z,
		Keyboard_1 = 30, // SDL_SCANCODE_1,
		Keyboard_2 = 31, // SDL_SCANCODE_2,
		Keyboard_3 = 32, // SDL_SCANCODE_3,
		Keyboard_4 = 33, // SDL_SCANCODE_4,
		Keyboard_5 = 34, // SDL_SCANCODE_5,
		Keyboard_6 = 35, // SDL_SCANCODE_6,
		Keyboard_7 = 36, // SDL_SCANCODE_7,
		Keyboard_8 = 37, // SDL_SCANCODE_8,
		Keyboard_9 = 38, // SDL_SCANCODE_9,
		Keyboard_0 = 39, // SDL_SCANCODE_0,
		Keypad_1 = 89, // SDL_SCANCODE_KP_1,
		Keypad_2 = 90, // SDL_SCANCODE_KP_2,
		Keypad_3 = 91, // SDL_SCANCODE_KP_3,
		Keypad_4 = 92, // SDL_SCANCODE_KP_4,
		Keypad_5 = 93, // SDL_SCANCODE_KP_5,
		Keypad_6 = 94, // SDL_SCANCODE_KP_6,
		Keypad_7 = 95, // SDL_SCANCODE_KP_7,
		Keypad_8 = 96, // SDL_SCANCODE_KP_8,
		Keypad_9 = 97, // SDL_SCANCODE_KP_9,
		Keypad_0 = 98, // SDL_SCANCODE_KP_0,
		F1 = 58, // SDL_SCANCODE_F1,
		F2 = 59, // SDL_SCANCODE_F2,
		F3 = 60, // SDL_SCANCODE_F3,
		F4 = 61, // SDL_SCANCODE_F4,
		F5 = 62, // SDL_SCANCODE_F5,
		F6 = 63, // SDL_SCANCODE_F6,
		F7 = 64, // SDL_SCANCODE_F7,
		F8 = 65, // SDL_SCANCODE_F8,
		F9 = 66, // SDL_SCANCODE_F9,
		F10 = 67, // SDL_SCANCODE_F10,
		F11 = 68, // SDL_SCANCODE_F11,
		F12 = 69, // SDL_SCANCODE_F12,
		Enter = 40, // SDL_SCANCODE_RETURN,
		Escape = 41, // SDL_SCANCODE_ESCAPE,
		Backspace = 42, // SDL_SCANCODE_BACKSPACE,
		Tab = 43, // SDL_SCANCODE_TAB,
		Space = 44, // SDL_SCANCODE_SPACE,
		Minus = 45, // SDL_SCANCODE_MINUS,
		Equals = 46, // SDL_SCANCODE_EQUALS,
		Leftbracket = 47, // SDL_SCANCODE_LEFTBRACKET,
		Rightbracket = 48, // SDL_SCANCODE_RIGHTBRACKET,
		Backslash = 49, // SDL_SCANCODE_BACKSLASH,
		Semicolon = 51, // SDL_SCANCODE_SEMICOLON,
		Apostrophe = 52, // SDL_SCANCODE_APOSTROPHE,
		Comma = 54, // SDL_SCANCODE_COMMA,
		Slash = 56, // SDL_SCANCODE_SLASH,
		Capslock = 57, // SDL_SCANCODE_CAPSLOCK,
		Printscreen = 70, // SDL_SCANCODE_PRINTSCREEN,
		Scrolllock = 71, // SDL_SCANCODE_SCROLLLOCK,
		Keypad_Enter = 88, // SDL_SCANCODE_KP_ENTER,
		ArrowRight = 79, // SDL_SCANCODE_RIGHT,
		ArrowLeft = 80, // SDL_SCANCODE_LEFT,
		ArrowDown = 81, // SDL_SCANCODE_DOWN,
		ArrowUp = 82, // SDL_SCANCODE_UP,
	};

	enum class ControllerInput : uint16_t
	{
		INVALID = std::numeric_limits<uint16_t>::max(), // internal usage
		ButtonA = 0, // SDL_CONTROLLER_BUTTON_A,
		ButtonB = 1, // SDL_CONTROLLER_BUTTON_B,
		ButtonX = 2, // SDL_CONTROLLER_BUTTON_X,
		ButtonY = 3, // SDL_CONTROLLER_BUTTON_Y,
		Back = 4, // SDL_CONTROLLER_BUTTON_BACK,
		Start = 6, // SDL_CONTROLLER_BUTTON_START,
		LeftStickPressed = 7, // SDL_CONTROLLER_BUTTON_LEFTSTICK,
		RightStickPressed = 8, // SDL_CONTROLLER_BUTTON_RIGHTSTICK,
		LeftShoulder = 9, // SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
		RightShoulder = 10, // SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
		DPAD_Up = 11, // SDL_CONTROLLER_BUTTON_DPAD_UP,
		DPAD_Down = 12, // SDL_CONTROLLER_BUTTON_DPAD_DOWN,
		DPAD_Left = 13, // SDL_CONTROLLER_BUTTON_DPAD_LEFT,
		DPAD_Right = 14, // SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
		JoystickLeftHorizontalAxis = 0, // SDL_CONTROLLER_AXIS_LEFTX,
		JoystickLeftVerticalAxis = 1, // SDL_CONTROLLER_AXIS_LEFTY,
		JoystickRightHorizontalAxis = 2, // SDL_CONTROLLER_AXIS_RIGHTX,
		JoystickRightVerticalAxis = 3, // SDL_CONTROLLER_AXIS_RIGHTY,
		LeftTrigger = 4, // SDL_CONTROLLER_AXIS_TRIGGERLEFT,
		RightTrigger = 5, // SDL_CONTROLLER_AXIS_TRIGGERRIGHT
	};
}

