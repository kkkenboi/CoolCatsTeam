/*!************************************************************************
 \file
 \author(s)
 \par DP email(s):
 \par Course:		CSD2401A
 \date
 \brief

**************************************************************************/

#include "Input.h"
#include "LitterBox/Engine/Time.h"

namespace LB
{
	InputSystem* INPUT = nullptr;

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	InputSystem::InputSystem()
	{
		if (!INPUT) //when theres no input instance, assign this new input system object as input
			INPUT = this;
		else
			std::cerr << "Input System already exist" << std::endl;
	}
	
	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/	
	void InputSystem::Update()
	{
		glfwPollEvents();
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void InputSystem::InvokeKeyPressed(GLFWwindow* pwin, int key, int scancode, int action, int mod)
	{
		UNREFERENCED_PARAMETER(pwin);
		UNREFERENCED_PARAMETER(scancode);
		UNREFERENCED_PARAMETER(mod);

		if (TIME->IsPaused() && (KeyCode)key != KeyCode::KEY_U) return;

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

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void InputSystem::InvokeKeyPressed(GLFWwindow* pwin, int button, int action, int mod) //overload function due to  glfw mouscallback have different sets of parameters
	{
		InvokeKeyPressed(pwin, button, 0, action, mod);
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
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

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
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
	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void InvokeKeyPressed(GLFWwindow* pwin, int key, int scancode, int action, int mod) {
		INPUT->InvokeKeyPressed(pwin, key, scancode, action, mod);
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void InvokeKeyPressed(GLFWwindow* pwin, int button, int action, int mod) {
		INPUT->InvokeKeyPressed(pwin, button, action, mod);
	}
}


