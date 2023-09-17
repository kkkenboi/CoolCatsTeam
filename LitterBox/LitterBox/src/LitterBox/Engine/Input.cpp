#include "Input.h"
#include <map>

#include <iostream>

namespace LBInput
{
	std::map<KeyCode, Key> inputKeys;

	void InvokeKeyPressed(GLFWwindow* pwin, int key, int scancode, int action, int mod)
	{
		if (action == GLFW_PRESS)
		{
			inputKeys[(KeyCode)key].onTrigger.Invoke(); //invoke, notifying all subscribers for the keycode on Trigger
			inputKeys[(KeyCode)key].onPressed.Invoke(); //invoke, notifying all subscribers for the keycode on Press
		}
		else if(action == GLFW_REPEAT) 
		{
			inputKeys[(KeyCode)key].onPressed.Invoke(); //invoke, notifying all subscribers for the keycode on Press
		}
		else if (action == GLFW_RELEASE)
		{
			inputKeys[(KeyCode)key].onReleased.Invoke(); //invoke, notifying all subscribers for the keycode on Release
		}
	}

	void InvokeKeyPressed(GLFWwindow* pwin, int button, int action, int mod) {
		InvokeKeyPressed(pwin, button, 0, action, mod);
	}

	void SubscribeToKey(Event<>::func_ptr function,  KeyCode key, KeyEvent keyEvent)
	{
		if (keyEvent == KeyEvent::TRIGGERED)
		{
			inputKeys[key].onTrigger.Subscribe(function, KEY_EVENT_PRIORITY);
		}
		else if (keyEvent == KeyEvent::PRESSED)
		{
			inputKeys[key].onPressed.Subscribe(function, KEY_EVENT_PRIORITY);
		}
		else if (keyEvent == KeyEvent::RELEASED)
		{
			inputKeys[key].onReleased.Subscribe(function, KEY_EVENT_PRIORITY);
		}
	}

	void UnsubscribeToKey(Event<>::func_ptr function, KeyCode key, KeyEvent keyEvent)
	{
		if (keyEvent == KeyEvent::TRIGGERED)
		{
			inputKeys[key].onTrigger.Unsubscribe(function);
		}
		else if (keyEvent == KeyEvent::PRESSED)
		{
			inputKeys[key].onPressed.Unsubscribe(function);
		}
		else if (keyEvent == KeyEvent::RELEASED)
		{
			inputKeys[key].onReleased.Unsubscribe(function);
		}
	}
}

//void CCPInputMouseCallBack(GLFWwindow* pwin, int button, int action, int mod)
//{
//	if (action == GLFW_PRESS)
//	{
//		switch (button)
//		{
//			case GLFW_MOUSE_BUTTON_LEFT:
//				CCPInputKeyTrigger(KeyCode::LMouse);
//				break;
//			case GLFW_MOUSE_BUTTON_RIGHT:
//				CCPInputKeyTrigger(KeyCode::RMouse);
//		}
//	}
//}


