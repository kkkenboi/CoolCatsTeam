#include "Input.h"

namespace LB
{
	InputSystem* INPUT = nullptr;

	InputSystem::InputSystem()
	{
		if (!INPUT) //when theres no input instance, assign this new input system object as input
			INPUT = this;
		else
			std::cerr << "Input System already exist" << std::endl;
	}
	
	void InputSystem::Update()
	{
		glfwPollEvents();
	}

	void InputSystem::InvokeKeyPressed(GLFWwindow* pwin, int key, int scancode, int action, int mod)
	{
		UNREFERENCED_PARAMETER(pwin);
		UNREFERENCED_PARAMETER(scancode);
		UNREFERENCED_PARAMETER(mod);

		//all the functions subscribe
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

	void InputSystem::InvokeKeyPressed(GLFWwindow* pwin, int button, int action, int mod) //overload function due to  glfw mouscallback have different sets of parameters
	{
		InvokeKeyPressed(pwin, button, 0, action, mod);
	}

	void InputSystem::SubscribeToKey(Event<>::func_ptr function,  KeyCode key, KeyEvent keyEvent)
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

	void InputSystem::UnsubscribeToKey(Event<>::func_ptr function, KeyCode key, KeyEvent keyEvent)
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

	// For GLFW
	void InvokeKeyPressed(GLFWwindow* pwin, int key, int scancode, int action, int mod) {
		INPUT->InvokeKeyPressed(pwin, key, scancode, action, mod);
	}
	void InvokeKeyPressed(GLFWwindow* pwin, int button, int action, int mod) {
		INPUT->InvokeKeyPressed(pwin, button, action, mod);
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


