#pragma once

//#include "pch.h"
//#include "System.h"
//#include "LitterBox/Core/Message.h"


enum class InputKey
{
	KEY_A, KEY_B, KEY_C,
	KEY_D, KEY_E, KEY_F, 
	KEY_G, KEY_H, KEY_I,
	KEY_J, KEY_K, KEY_L,
	KEY_M, KEY_N, KEY_O,
	KEY_P, KEY_Q, KEY_R,
	KEY_S, KEY_T, KEY_U,
	KEY_V, KEY_W, KEY_X,
	KEY_Y, KEY_Z,

	LMouse,
	RMouse
};

void CCPInputKeyTrigger(InputKey inputkey);
void CCPInputKeyCallBack(GLFWwindow* pwin, int key, int scancode, int action, int mod);
void CCPInputMouseCallBack(GLFWwindow* pwin, int button, int action, int mod);

