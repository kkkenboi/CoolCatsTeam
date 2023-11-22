/*!************************************************************************
 \file				Input.h
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

#pragma once

#include <GLFW/glfw3.h>
#include "LitterBox/Core/System.h"
#include "Events.h"
#include <map>

namespace LB {
	#define KEY_EVENT_PRIORITY 100

	/*!***********************************************************************
	 \brief
	 enum class for all keycodes as GLFW keys increments.
	*************************************************************************/
	enum class KeyCode
	{
		//1 to 1 mapping to GLFW enums
		KEY_MOUSE_1 = GLFW_MOUSE_BUTTON_1,
		KEY_MOUSE_2, KEY_MOUSE_3, KEY_MOUSE_4,
		KEY_MOUSE_5,

		KEY_0 = GLFW_KEY_0,
		KEY_1, KEY_2, KEY_3,
		KEY_4, KEY_5, KEY_6,
		KEY_7, KEY_8, KEY_9,

		KEY_A = GLFW_KEY_A,
		KEY_B, KEY_C, KEY_D,
		KEY_E, KEY_F, KEY_G,
		KEY_H, KEY_I, KEY_J,
		KEY_K, KEY_L, KEY_M,
		KEY_N, KEY_O, KEY_P,
		KEY_Q, KEY_R, KEY_S,
		KEY_T, KEY_U, KEY_V,
		KEY_W, KEY_X, KEY_Y,
		KEY_Z,

		KEY_ESCAPE = GLFW_KEY_ESCAPE,
		KEY_ENTER, KEY_TAB, KEY_BACKSPACE,
		KEY_INSERT, KEY_DELETE, KEY_RIGHT,
		KEY_LEFT, KEY_DOWN, KEY_UP,

		KEY_F1 = GLFW_KEY_F1, 
		KEY_F2, KEY_F3, KEY_F4, 
		KEY_F5, KEY_F6, KEY_F7, 
		KEY_F8, KEY_F9, KEY_F10, 
		KEY_F11, KEY_F12,

		KEY_LEFTSHIFT = GLFW_KEY_LEFT_SHIFT,
		KEY_LEFTCONTROL, KEY_LEFTALT
	};

	/*!***********************************************************************
	 \brief
	 enum class for whether the key press broadcast should pause when it's
	 paused.
	*************************************************************************/
	enum class KeyTriggerType 
	{
		PAUSABLE,
		NONPAUSABLE
	};

	/*!***********************************************************************
	 \brief
	 3 types of KeyEvent, Trigger, pressed and released
	*************************************************************************/
	enum class KeyEvent
	{
		TRIGGERED,
		PRESSED,
		RELEASED
	};

	/*!***********************************************************************
	 \brief
	 Class InputSystem that inherits from ISystem
	*************************************************************************/
	class InputSystem : public ISystem
	{
	public:
		/*!***********************************************************************
		 \brief
		 Constructor for InputSystem
		*************************************************************************/
		InputSystem();

		/*!***********************************************************************
		 \brief
		 Initialize function to set the input system name
		*************************************************************************/
		void Initialize() override { SetSystemName("Input System"); }

		/*!***********************************************************************
		 \brief
		 Update function that updates the input
		*************************************************************************/
		virtual void Update();

		/*!***********************************************************************
		 \brief
		 Invoke Key overload press to notify all subscribers for all keycode presses
		*************************************************************************/
		void GLFWKeyPressed(GLFWwindow* pwin, int key, int scancode, int action, int mod);

		/*!***********************************************************************
		 \brief
		 Invoke Key overload press to notify all subscribers for all keycode presses
		*************************************************************************/
		void GLFWKeyPressed(GLFWwindow* pwin, int button, int action, int mod);

		/*!***********************************************************************
		 \brief
		 Updates the mouse position based on GLFW
		*************************************************************************/
		void GLFWMousePos(GLFWwindow* window, double xpos, double ypos);

		/*!***********************************************************************
		 \brief
		 Returns the mouse position based on the whole main window
		*************************************************************************/
		Vec2<float> const& GetMousePos();

		/*!***********************************************************************
		 \brief
		 Subscribes the key event to register/notify when the event occurs
		*************************************************************************/
		void SubscribeToKey(Event<>::func_ptr function, KeyCode key, KeyEvent keyEvent, KeyTriggerType triggerType = KeyTriggerType::PAUSABLE);

		/*!***********************************************************************
		 \brief
		 Unsubscribes the key where it stop its interest in listening to the the key input.
		*************************************************************************/
		void UnsubscribeFromKey(Event<>::func_ptr function, KeyCode key, KeyEvent keyEvent, KeyTriggerType triggerType = KeyTriggerType::PAUSABLE);

		/*!***********************************************************************
		 \brief
		 Was the key given pressed (held) this frame
		*************************************************************************/
		bool IsKeyPressed(KeyCode key);

		/*!***********************************************************************
		 \brief
		 Was the key given just pressed (triggered) this frame
		*************************************************************************/
		bool IsKeyTriggered(KeyCode key);

		/*!***********************************************************************
		 \brief
		 Was the key released this frame
		*************************************************************************/
		bool IsKeyReleased(KeyCode key);

	private:
		/*!***********************************************************************
		 \brief
		 Getting Events for onTrigger onPressed and onReleased
		*************************************************************************/
		struct Key
		{
			Event<> onTrigger;
			Event<> onPressed;
			Event<> onReleased;
		};

		/*!***********************************************************************
		 \brief
		 Map create a new pair a keycode to key when it doesnt exist
		*************************************************************************/
		std::map<KeyCode, Key> inputKeys, inputKeysPausable;

		Vec2<float> mousePos;

		bool inputKeysLast[GLFW_KEY_LAST] { false }, inputKeysCurr[GLFW_KEY_LAST]{ false };
	};

	/*!***********************************************************************
	\brief
	 So others can use the INPUT
	*************************************************************************/
	extern InputSystem* INPUT; //can reference input from anywhere
	//if they want to use INPUT they just type INPUT

	// For GLFW
	/*!***********************************************************************
	\brief
	 Sets the state of the key given from GLFW
	*************************************************************************/
	void GLFWKeyPressed(GLFWwindow* pwin, int key, int scancode, int action, int mod);

	/*!***********************************************************************
	 \brief
	 Sets the state of the key given from GLFW
	*************************************************************************/
	void GLFWKeyPressed(GLFWwindow* pwin, int button, int action, int mod);

	/*!***********************************************************************
	 \brief
	 Sets the mouse position given from GLFW
	*************************************************************************/
	void GLFWMousePos(GLFWwindow* window, double xpos, double ypos);
}
