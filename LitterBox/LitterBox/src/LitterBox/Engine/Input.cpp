#include <GLFW/glfw3.h>
#include "linmath.h"
#include "Input.h"


void CCPInputKeyTrigger(InputKey inputkey)
{
	switch (inputkey)
	{
		//WASD IMPT FOR MOVEMENT
		case InputKey::KEY_W:
			std::cout << "W pressed\n";
			break;
		case InputKey::KEY_A:
			std::cout << "A pressed\n";
			break;
		case InputKey::KEY_S:
			std::cout << "S pressed\n";
			break;
		case InputKey::KEY_D:
			std::cout << "D pressed\n";
			break;
		//extra
		case InputKey::KEY_B:
			std::cout << "B pressed\n";
			break;
		case InputKey::KEY_C:
			std::cout << "C pressed\n";
			break;
		case InputKey::KEY_E:
			std::cout << "E pressed\n";
			break;
		case InputKey::KEY_F:
			std::cout << "F pressed\n";
			break;
		case InputKey::KEY_G:
			std::cout << "G pressed\n";
			break;
		case InputKey::KEY_H:
			std::cout << "H pressed\n";
			break;
		case InputKey::KEY_I:
			std::cout << "I pressed\n";
			break;
		case InputKey::KEY_J:
			std::cout << "J pressed\n";
			break;
		case InputKey::KEY_K:
			std::cout << "K pressed\n";
			break;
		case InputKey::KEY_L:
			std::cout << "L pressed\n";
			break;
		case InputKey::KEY_M:
			std::cout << "M pressed\n";
			break;
		case InputKey::KEY_N:
			std::cout << "N pressed\n";
			break;
		case InputKey::KEY_O:
			std::cout << "O pressed\n";
			break;
		case InputKey::KEY_P:
			std::cout << "P pressed\n";
			break;
		case InputKey::KEY_Q:
			std::cout << "Q pressed\n";
			break;
		case InputKey::KEY_R:
			std::cout << "R pressed\n";
			break;
		case InputKey::KEY_T:
			std::cout << "T pressed\n";
			break;
		case InputKey::KEY_U:
			std::cout << "U pressed\n";
			break;
		case InputKey::KEY_V:
			std::cout << "V pressed\n";
			break;
		case InputKey::KEY_X:
			std::cout << "X pressed\n";
			break;
		case InputKey::KEY_Y:
			std::cout << "Y pressed\n";
			break;
		case InputKey::KEY_Z:
			std::cout << "Z pressed\n";
			break;

		//Mouse input
		case InputKey::LMouse:
			std::cout << "Left Mouse pressed\n";
			break;
		case InputKey::RMouse:
			std::cout << "Right Mouse pressed\n";
			break;
	}

}

void CCPInputKeyCallBack(GLFWwindow* pwin, int key, int scancode, int action, int mod)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			//WASD
			case GLFW_KEY_W:
				CCPInputKeyTrigger(InputKey::KEY_W);
				break;
			case GLFW_KEY_A:
				CCPInputKeyTrigger(InputKey::KEY_A);
				break;
			case GLFW_KEY_S:
				CCPInputKeyTrigger(InputKey::KEY_S);
				break;
			case GLFW_KEY_D:
				CCPInputKeyTrigger(InputKey::KEY_D);
				break;

			//Extra just in case
			case GLFW_KEY_B:
				CCPInputKeyTrigger(InputKey::KEY_B);
				break;
			case GLFW_KEY_C:
				CCPInputKeyTrigger(InputKey::KEY_C);
				break;
			case GLFW_KEY_E:
				CCPInputKeyTrigger(InputKey::KEY_E);
				break;
			case GLFW_KEY_F:
				CCPInputKeyTrigger(InputKey::KEY_F);
				break;
			case GLFW_KEY_G:
				CCPInputKeyTrigger(InputKey::KEY_G);
				break;
			case GLFW_KEY_H:
				CCPInputKeyTrigger(InputKey::KEY_H);
				break;
			case GLFW_KEY_I:
				CCPInputKeyTrigger(InputKey::KEY_I);
				break;
			case GLFW_KEY_J:
				CCPInputKeyTrigger(InputKey::KEY_J);
				break;
			case GLFW_KEY_K:
				CCPInputKeyTrigger(InputKey::KEY_K);
				break;
			case GLFW_KEY_L:
				CCPInputKeyTrigger(InputKey::KEY_L);
				break;
			case GLFW_KEY_M:
				CCPInputKeyTrigger(InputKey::KEY_M);
				break;
			case GLFW_KEY_N:
				CCPInputKeyTrigger(InputKey::KEY_N);
				break;
			case GLFW_KEY_O:
				CCPInputKeyTrigger(InputKey::KEY_O);
				break;
			case GLFW_KEY_P:
				CCPInputKeyTrigger(InputKey::KEY_P);
				break;
			case GLFW_KEY_Q:
				CCPInputKeyTrigger(InputKey::KEY_Q);
				break;
			case GLFW_KEY_R:
				CCPInputKeyTrigger(InputKey::KEY_R);
				break;
			case GLFW_KEY_T:
				CCPInputKeyTrigger(InputKey::KEY_T);
				break;
			case GLFW_KEY_U:
				CCPInputKeyTrigger(InputKey::KEY_U);
				break;
			case GLFW_KEY_V:
				CCPInputKeyTrigger(InputKey::KEY_V);
				break;
			case GLFW_KEY_X:
				CCPInputKeyTrigger(InputKey::KEY_X);
				break;
			case GLFW_KEY_Y:
				CCPInputKeyTrigger(InputKey::KEY_Y);
				break;
			case GLFW_KEY_Z:
				CCPInputKeyTrigger(InputKey::KEY_Z);
				break;

		}
	}
}

void CCPInputMouseCallBack(GLFWwindow* pwin, int button, int action, int mod)
{
	if (action == GLFW_PRESS)
	{
		switch (button)
		{
			case GLFW_MOUSE_BUTTON_LEFT:
				CCPInputKeyTrigger(InputKey::LMouse);
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				CCPInputKeyTrigger(InputKey::RMouse);
		}
	}
}


