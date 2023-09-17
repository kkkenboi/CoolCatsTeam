#pragma once

#include <GLFW/glfw3.h>
#include "Events.h"

enum class KeyCode
{
	KEY_MOUSE_1 = GLFW_MOUSE_BUTTON_1,
	KEY_MOUSE_2,

	KEY_0 = GLFW_KEY_0,
	KEY_1, KEY_2, KEY_3,
	KEY_4, KEY_5, KEY_6,
	KEY_7, KEY_8, KEY_9,

	KEY_A = GLFW_KEY_A,
	KEY_B, KEY_C,
	KEY_D, KEY_E, KEY_F,
	KEY_G, KEY_H, KEY_I,
	KEY_J, KEY_K, KEY_L,
	KEY_M, KEY_N, KEY_O,
	KEY_P, KEY_Q, KEY_R,
	KEY_S, KEY_T, KEY_U,
	KEY_V, KEY_W, KEY_X,
	KEY_Y, KEY_Z,
};

enum class KeyEvent
{
	TRIGGERED,
	PRESSED,
	RELEASED

};

namespace LBInput
{
	#define KEY_EVENT_PRIORITY 100

	struct Key
	{
		Event<> onTrigger;
		Event<> onPressed;
		Event<> onReleased;
	};

	void InvokeKeyPressed(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	void InvokeKeyPressed(GLFWwindow* pwin, int button, int action, int mod);
	void SubscribeToKey(Event<>::func_ptr function, KeyCode key, KeyEvent keyEvent);
	void UnsubscribeToKey(Event<>::func_ptr function, KeyCode key, KeyEvent keyEvent);

}

