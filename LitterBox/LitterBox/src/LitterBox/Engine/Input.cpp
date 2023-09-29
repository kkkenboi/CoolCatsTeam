/*!************************************************************************
 \file
 \author(s)
 \par DP email(s):
 \par Course:		CSD2401A
 \date
 \brief

**************************************************************************/

#include "Platform/Windows/Windows.h"
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
		for (int i = 0; i < GLFW_KEY_LAST; ++i)
		{
			inputKeysLast[i] = inputKeysCurr[i];
		}

		glfwPollEvents();

		for (auto key = inputKeys.begin(); key != inputKeys.end(); ++key) 
		{
			// TODO: Refactor pausing input, have two maps, one affected by time, one not
			if (TIME->IsPaused() && !(key->first == KeyCode::KEY_U || key->first == KeyCode::KEY_J || key->first == KeyCode::KEY_I || key->first == KeyCode::KEY_L)) continue;

			if (inputKeysCurr[(int)key->first] && !inputKeysLast[(int)key->first])
			{

				inputKeys[key->first].onTrigger.Invoke(); //invoke, notifying all subscribers for the keycode on Trigger
				inputKeys[key->first].onPressed.Invoke(); //invoke, notifying all subscribers for the keycode on Press
			}
			else if (inputKeysCurr[(int)key->first] && inputKeysLast[(int)key->first])
			{
				inputKeys[key->first].onPressed.Invoke(); //invoke, notifying all subscribers for the keycode on Press
			}
			else if (!inputKeysCurr[(int)key->first] && inputKeysLast[(int)key->first])
			{
				inputKeys[key->first].onReleased.Invoke(); //invoke, notifying all subscribers for the keycode on Release
			}
		}
	}

	bool InputSystem::IsKeyPressed(KeyCode key)
	{
		return inputKeysCurr[(int)key];
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

		//all the functions subscribe
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			inputKeysCurr[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			inputKeysCurr[key] = false;
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


