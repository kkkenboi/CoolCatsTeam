/*!************************************************************************
 \file				CPPSMouse.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:		CSD2451A
 \date				06-03-2024
 \brief
 This file contains the CPPSMouse class and all its functionalities.

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/


#include "CPPSMouse.h"
#include "CPPSCameraFollow.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Core/Core.h"
#include "Platform/Windows/Windows.h"


namespace LB
{
	/*!***********************************************************************
	\brief
	Start function where variables will be initialised
	*************************************************************************/
	void CPPSMouse::Start()
	{
		// Initialise pointers to specific game objects
		m_MouseUI = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("MouseUI", *m_MouseUI);

		m_MouseWorld = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("MouseWorld", *m_MouseWorld);

		m_CameraFollow = GOMANAGER->FindGameObjectWithName("CameraFollow");

		m_mHeight = static_cast<float>(WINDOWSSYSTEM->GetMonitorHeight());
		m_mWidth = static_cast<float>(WINDOWSSYSTEM->GetMonitorWidth());
		m_BorderHeight = (m_mHeight - static_cast<float>(WINDOWSSYSTEM->GetHeight())) / 2.f;
	}

	/*!***********************************************************************
	\brief
	Update function where the mouse's position is being updated based on
	whether it is of screen or world space
	*************************************************************************/
	void CPPSMouse::Update()
	{
		if (CORE->IsEditorMode())
		{
			// Mouse will be following the camera and adjusting to the native resolution if in full screen mode
			Vec2<float> mousePos = INPUT->GetMousePos();
			// Gamepad cursor
			Vec2<float> gamePadPos = INPUT->GetRightJoystickPos();
			if (gamePadPos.x != 0.f || gamePadPos.y != 0.f)
			{
				mousePos = m_GamePadRef + gamePadPos * 300.f;
			}
			else
			{
				mousePos = INPUT->GetMousePos();

				mousePos.x += m_CameraFollow->GetComponent<CPPSCameraFollow>()->cameraPos.x;
				mousePos.y -= m_CameraFollow->GetComponent<CPPSCameraFollow>()->cameraPos.y;
			
				mousePos.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();
				mousePos.y = (mousePos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight()) * 1080.f / (float)WINDOWSSYSTEM->GetHeight();
			}
			m_MouseWorld->GetComponent<CPTransform>()->SetPosition(mousePos);

			if (gamePadPos.x != 0.f || gamePadPos.y != 0.f)
			{
				Vec2<float> camPos = m_CameraFollow->GetComponent<CPPSCameraFollow>()->currentPos - m_GamePadRef;
				camPos.x += 960.f;
				camPos.y += 540.f;

				mousePos = gamePadPos * 300.f;
				mousePos.x += WINDOWSSYSTEM->GetWidth() / 2.0f;
				mousePos.y = WINDOWSSYSTEM->GetHeight() / 2.0f - mousePos.y;

				mousePos.x -= camPos.x;
				mousePos.y += camPos.y;

				glfwSetCursorPos(WINDOWSSYSTEM->GetWindow(), mousePos.x, mousePos.y);
			}
			else
			{
				mousePos = INPUT->GetMousePos();
				mousePos.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();
				mousePos.y = (mousePos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight()) * 1080.f / (float)WINDOWSSYSTEM->GetHeight();
			}
			m_MouseUI->GetComponent<CPTransform>()->SetPosition(mousePos);
		}
		else
		{
			// Mouse will be following the camera and adjusting to the native resolution if in full screen mode
			Vec2<float> mousePos = INPUT->GetMousePos();
			// Gamepad cursor
			Vec2<float> gamePadPos = INPUT->GetRightJoystickPos();
			if (gamePadPos.x != 0.f || gamePadPos.y != 0.f)
			{
				mousePos = m_GamePadRef + gamePadPos * 300.f;
			}
			else
			{
				mousePos = INPUT->GetMousePos();

				mousePos.x *= WINDOWSSYSTEM->getViewPortConversion().x;
				mousePos.x += m_CameraFollow->GetComponent<CPPSCameraFollow>()->cameraPos.x;

				mousePos.y = ((float)WINDOWSSYSTEM->GetHeight() - mousePos.y - WINDOWSSYSTEM->getBorderHeight()) * WINDOWSSYSTEM->getViewPortConversion().y;
				mousePos.y += m_CameraFollow->GetComponent<CPPSCameraFollow>()->cameraPos.y;
			}

			// Add offset if the scene is not mainmenu or in pause menu
			if (WINDOWSSYSTEM->GetSceneName() != "SceneMainMenu")
			{
				mousePos.x += 24.f;
				mousePos.y -= 24.f;
			}

			m_MouseWorld->GetComponent<CPTransform>()->SetPosition(mousePos);

			if (gamePadPos.x != 0.f || gamePadPos.y != 0.f)
			{
				Vec2<float> camPos = m_CameraFollow->GetComponent<CPPSCameraFollow>()->currentPos - m_GamePadRef;
				camPos.x += 960.f;
				camPos.y += 540.f;

				mousePos = gamePadPos * 300.f;
				mousePos.x += WINDOWSSYSTEM->GetWidth() / 2.0f;
				mousePos.y = WINDOWSSYSTEM->GetHeight() / 2.0f - mousePos.y;

				mousePos.x -= camPos.x;
				mousePos.y += camPos.y;

				glfwSetCursorPos(WINDOWSSYSTEM->GetWindow(), mousePos.x, mousePos.y);
			}
			else
			{
				mousePos = INPUT->GetMousePos();
				mousePos.x *= WINDOWSSYSTEM->getViewPortConversion().x;
				mousePos.y = ((float)WINDOWSSYSTEM->GetHeight() - mousePos.y - WINDOWSSYSTEM->getBorderHeight()) * WINDOWSSYSTEM->getViewPortConversion().y;
			}

			// Add offset if the scene is not mainmenu or in pause menu
			if (WINDOWSSYSTEM->GetSceneName() != "SceneMainMenu")
			{
				mousePos.x += 24.f;
				mousePos.y -= 24.f;
			}

			m_MouseUI->GetComponent<CPTransform>()->SetPosition(mousePos);
		}
	}

	/*!***********************************************************************
	\brief
	Overriden destroy function because of inheritance
	*************************************************************************/
	void CPPSMouse::Destroy()
	{

	}
}