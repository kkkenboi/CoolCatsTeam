/*!************************************************************************
 \file				CPPSFPS.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:		CSD2451A
 \date				09-02-2024
 \brief
 This file contains the CPPSFPS class and all its functionalities.

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSFPS.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Renderer/Renderer.h"
#include "LitterBox/Components/TextComponent.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	Start function to initialise a FPS object and set it's scale
	*************************************************************************/
	void CPPSFPS::Start()
	{
		// Based on the FPS prefab, create the object, position is already set in the prefab
		m_FPSObject = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("FPS", *m_FPSObject);

		// Set it to 0.f when not active ( wanted to do ToggleActive but didn't work )
		m_FPSObject->GetComponent<CPText>()->get_msg().scale = 0.0f;
		//m_FPSObject->GetComponent<CPText>()->ToggleActive(false);
	}


	/*!***********************************************************************
	\brief
	Update function to check for the current game state and also if the user
	wants to hide the FPS using key 'J'. 
	*************************************************************************/
	void CPPSFPS::Update()
	{
		// Update game paused state
		if (!TIME->IsPaused())
		{
			if (INPUT->IsKeyTriggered(KeyCode::KEY_ESCAPE))
			{
				m_GamePaused = true;
			}

		}
		else
		{
			if (INPUT->IsKeyTriggered(KeyCode::KEY_ESCAPE))
			{
				m_GamePaused = false;
			}
		}

		if (INPUT->IsKeyTriggered(KeyCode::KEY_0))
		{
			m_ShowFPS = !m_ShowFPS;
		}

		// If game is not paused, update the text shown based on the actual FPS
		if (!m_GamePaused)
		{
			std::stringstream truncatedFps;
			truncatedFps << std::fixed << std::setprecision(2) << (1.0 / TIME->GetUnscaledDeltaTime());
			fpsBuffer = "FPS: " + truncatedFps.str();
			strcpy_s(finalFps, sizeof(finalFps), fpsBuffer.c_str());

			m_FPSObject->GetComponent<CPText>()->set_msg(finalFps);

			// Follow the camera to ensure that it's always on the top right
			//m_FPSObject->GetComponent<CPTransform>()->SetPosition(m_CameraFollow->GetComponent<CPTransform>()->GetPosition() + Vec2<float>(750.f, 500.f));
			//DebuggerLogFormat("m_FPSObject Position: %f, %f", m_FPSObject->GetComponent<CPTransform>()->GetPosition().x, m_FPSObject->GetComponent<CPTransform>()->GetPosition().y);
		}

		// If key J is pressed, show the FPS.
		if (m_ShowFPS)
		{
			m_FPSObject->GetComponent<CPText>()->get_msg().scale = 1.0f;
		}
		else if (!m_ShowFPS)
		{
			m_FPSObject->GetComponent<CPText>()->get_msg().scale = 0.0f;
		}

	}


	/*!***********************************************************************
	\brief
	Inherited Destroy function to destroy any future variables we new'ed.
	*************************************************************************/
	void CPPSFPS::Destroy()
	{
		
	}
}