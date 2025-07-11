/*!************************************************************************
 \file				Input.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin.chua@digipen.edu
 \par Course:		CSD2401A
 \date				15-09-2023
 \brief
 The functions in the Input class include:

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "Platform/Windows/Windows.h"
#include "Input.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Renderer/Renderer.h"

namespace LB
{
	InputSystem* INPUT = nullptr;

	/*!***********************************************************************
	 \brief
	 Constructor for InputSystem to check if input system exist
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
	 In this update function, when is pause prevent all keys to activate
	 \return
	 void
	*************************************************************************/	
	void InputSystem::Update()
	{
		for (int i = 0; i < GLFW_KEY_LAST; ++i)
		{
			inputKeysLast[i] = inputKeysCurr[i];
		}

		glfwPollEvents();

		// Trigger non-pausable key press events
		for (auto key = inputKeys.begin(); key != inputKeys.end(); ++key)
		{
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

		// Check for gamepad input
		GLFWPollGamepad();

		// Trigger pausable key press events
		if (TIME->IsPaused()) return;
		for (auto key = inputKeysPausable.begin(); key != inputKeysPausable.end(); ++key)
		{
			if (inputKeysCurr[(int)key->first] && !inputKeysLast[(int)key->first])
			{
				inputKeysPausable[key->first].onTrigger.Invoke(); //invoke, notifying all subscribers for the keycode on Trigger
				inputKeysPausable[key->first].onPressed.Invoke(); //invoke, notifying all subscribers for the keycode on Press
			}
			else if (inputKeysCurr[(int)key->first] && inputKeysLast[(int)key->first])
			{
				inputKeysPausable[key->first].onPressed.Invoke(); //invoke, notifying all subscribers for the keycode on Press
			}
			else if (!inputKeysCurr[(int)key->first] && inputKeysLast[(int)key->first])
			{
				inputKeysPausable[key->first].onReleased.Invoke(); //invoke, notifying all subscribers for the keycode on Release
			}
		}
	}

	/*!***********************************************************************
	 \brief
	 Check if key is pressed or not (Held down in last frame and current frame)
	*************************************************************************/
	bool InputSystem::IsKeyPressed(KeyCode key)
	{
		return inputKeysCurr[(int)key];
	}

	/*!***********************************************************************
	 \brief
	 Check if key is triggered or not (ONLY held down in current frame)
	*************************************************************************/
	bool InputSystem::IsKeyTriggered(KeyCode key)
	{
		return inputKeysCurr[(int)key] && !inputKeysLast[(int)key];
	}

	/*!***********************************************************************
	 \brief
	 Check if key is released or not (Release current frame)
	*************************************************************************/
	bool InputSystem::IsKeyReleased(KeyCode key)
	{
		return !inputKeysCurr[(int)key] && inputKeysLast[(int)key];
	}

	/*!***********************************************************************
	 \brief
	 Check if gamepad is connected and update the gamepad state
	*************************************************************************/
	void InputSystem::GLFWPollGamepad()
	{
		// 1 .
		// This should be done in joycallback ?
		// glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepadState) => set current state for both the button and axis and last

		// 2.
		// Need something to continously check for incoming connection => glfwJoystickPresent(GLFW_JOYSTICK_1); then set the first bool (probably done event style to start up the second bool which is to run the update loop)

		// 3. 
		// This is done here
		// put a bool here to only run if there is a controller connected
		// TO DO add portion for non-pausable button and axis 
			// TO DO else if for controller ontrigger&onpressed
			// TO DO else if for controller onpressed
			// else if for controller onreleased
		// this uses the curr and last to check to do the 3 options above or not

		// 4. upon disconnection, turn bools off and stuff
		m_gamepadConnected = glfwJoystickPresent(GLFW_JOYSTICK_1);
		if (m_gamepadConnected)
		{
			int axisCount;
			const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);

			// Check if joystick is moved beyond the deadzone
			if (axisCount >= 4)
			{
				if ((fabsf(axes[0]) > JOYSTICK_DEADZONE) || (fabsf(axes[1]) > JOYSTICK_DEADZONE)) 
				{
					leftJoystickPos.x = axes[0];
					leftJoystickPos.y = -axes[1];
					leftJoystickPos.Normalise();
				}
				else
				{
					leftJoystickPos.x = 0.0f;
					leftJoystickPos.y = 0.0f;
				}

				if ((fabsf(axes[2]) > JOYSTICK_DEADZONE) || (fabsf(axes[3]) > JOYSTICK_DEADZONE))
				{
					rightJoytickPos.x = axes[2];
					rightJoytickPos.y = -axes[3];
					rightJoytickPos.Normalise();
				}
				else
				{
					rightJoytickPos.x = 0.0f;
					rightJoytickPos.y = 0.0f;
				}
			}

			int buttonCount;
			const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
			// Check if buttons are pressed
			if (buttonCount >= 8)
			{
				for (int button{ 0 }; button < buttonCount; ++button)
				{
					if (buttons[button] == GLFW_RELEASE)
					{
						inputKeysCurr[(int)KeyCode::KEY_GAMEPAD_A + button] = false;
					}
					else
					{
						inputKeysCurr[(int)KeyCode::KEY_GAMEPAD_A + button] = true;
					}
				}
			}
		}
	}
	/*!***********************************************************************
	 \brief
	 Returns the current left joystick position (between -1 and 1)
	*************************************************************************/
	Vec2<float> const& InputSystem::GetLeftJoystickPos()
	{
		return leftJoystickPos;
	}
	/*!***********************************************************************
	 \brief
	 Returns the current right joystick position (between -1 and 1)
	*************************************************************************/
	Vec2<float> const& InputSystem::GetRightJoystickPos()
	{
		return rightJoytickPos;
	}
	/*!***********************************************************************
	 \brief
	 Retuns true if a gamepad is connected
	*************************************************************************/
	bool InputSystem::IsGamepadConnected()
	{
		return m_gamepadConnected;
	}

	/*!***********************************************************************
	 \brief
	 Invoke Key overload press to notify all subscribers for all keycode presses
	 \return
	 void
	*************************************************************************/
	void InputSystem::GLFWKeyPressed(GLFWwindow* pwin, int key, int scancode, int action, int mod)
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
	 Invoke Key overload press to notify all subscribers for all keycode presses
	 \return
	 void
	*************************************************************************/
	void InputSystem::GLFWKeyPressed(GLFWwindow* pwin, int button, int action, int mod) //overload function due to  glfw mouscallback have different sets of parameters
	{
		GLFWKeyPressed(pwin, button, 0, action, mod);
	}

	/*!***********************************************************************
	 \brief
	 Updates the mouse position based on GLFW
	*************************************************************************/
	void InputSystem::GLFWMousePos(GLFWwindow* window, double xpos, double ypos) 
	{
		// If mouse position is being controlled externally (like editor), don't use GLFW's
		if (mousePosOverwritten) 
		{
			return;
		}

		UNREFERENCED_PARAMETER(window);
		mousePos.x = (float)xpos;
		mousePos.y = (float)ypos;
	}

	/*!***********************************************************************
	 \brief
	 Returns the mouse position based on the whole main window
	*************************************************************************/
	Vec2<float> const& InputSystem::GetMousePos()
	{
		return mousePos;
	}

	/*!***********************************************************************
	 \brief
	 Overwrite the mouse position, mainly used by Editor to map the mousePos
	 to the GameView instead of the full screen
	*************************************************************************/
	void InputSystem::OverwriteMousePos(Vec2<float> const& newPos)
	{
		mousePos.x = newPos.x;
		mousePos.y = newPos.y;
		mousePosOverwritten = true;
	}

	/*!***********************************************************************
	 \brief
	 Subscribes the key event to register/notify when the event occurs
	 \return
	 void
	*************************************************************************/
	void InputSystem::SubscribeToKey(Event<>::func_ptr function,  KeyCode key, KeyEvent keyEvent, KeyTriggerType triggerType)
	{
		if (keyEvent == KeyEvent::TRIGGERED)
		{
			if (triggerType == KeyTriggerType::PAUSABLE) 
			{
				inputKeysPausable[key].onTrigger.Subscribe(function, KEY_EVENT_PRIORITY);
			}
			else 
			{
				inputKeys[key].onTrigger.Subscribe(function, KEY_EVENT_PRIORITY);
			}
		}
		else if (keyEvent == KeyEvent::PRESSED)
		{
			if (triggerType == KeyTriggerType::PAUSABLE)
			{
				inputKeysPausable[key].onPressed.Subscribe(function, KEY_EVENT_PRIORITY);
			}
			else
			{
				inputKeys[key].onPressed.Subscribe(function, KEY_EVENT_PRIORITY);
			}
		}
		else if (keyEvent == KeyEvent::RELEASED)
		{
			if (triggerType == KeyTriggerType::PAUSABLE)
			{
				inputKeysPausable[key].onReleased.Subscribe(function, KEY_EVENT_PRIORITY);
			}
			else
			{
				inputKeys[key].onReleased.Subscribe(function, KEY_EVENT_PRIORITY);
			}
		}
	}

	/*!***********************************************************************
	 \brief
	 Unsubscribes the key where it stop its interest in listening to the the key input.
	 \return
	 void
	*************************************************************************/
	void InputSystem::UnsubscribeFromKey(Event<>::func_ptr function, KeyCode key, KeyEvent keyEvent, KeyTriggerType triggerType)
	{
		UNREFERENCED_PARAMETER(triggerType);
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
	 Sets the state of the key given from GLFW
	*************************************************************************/
	void GLFWKeyPressed(GLFWwindow* pwin, int key, int scancode, int action, int mod) 
	{
		INPUT->GLFWKeyPressed(pwin, key, scancode, action, mod);
	}

	/*!***********************************************************************
	 \brief
	 Sets the state of the key given from GLFW
	*************************************************************************/
	void GLFWKeyPressed(GLFWwindow* pwin, int button, int action, int mod) 
	{
		INPUT->GLFWKeyPressed(pwin, button, action, mod);
	}
	
	/*!***********************************************************************
	 \brief
	 Sets the mouse position given from GLFW
	*************************************************************************/
	void GLFWMousePos(GLFWwindow* window, double xpos, double ypos) 
	{
		INPUT->GLFWMousePos(window, xpos, ypos);
	}

}


