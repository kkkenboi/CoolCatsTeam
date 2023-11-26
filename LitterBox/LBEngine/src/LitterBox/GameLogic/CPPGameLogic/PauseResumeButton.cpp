/*!************************************************************************
 \file				PauseResumeButton.cpp
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:		CSD2401A
 \date				26-11-2023
 \brief
 This file contains the functionality of the back button for the how to play
 screen during gameplay

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "Platform/Windows/Windows.h"
#include "PauseResumeButton.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Physics/ColliderManager.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	 Initializes the GameObjects needed for the script
	*************************************************************************/
	void PauseResumeButton::Start()
	{
		for (GameObject* GO : GOMANAGER->GetGameObjects())
		{
			if (GO->GetName() == "PauseMenuTextureObject")
			{
				MenuTextureObject = GO;
			}
			if (GO->GetName() == "PauseMenuResumeObject")
			{
				ResumeButtonObject = GO;
			}
			if (GO->GetName() == "PauseMenuHowToPlayObject")
			{
				HowToPlayButtonObject = GO;
			}
			if (GO->GetName() == "PauseMenuQuitObject")
			{
				QuitButtonObject = GO;
			}
		}
	}

	/*!***********************************************************************
	\brief
	 Allows for button press and movement of other game objects into view
	*************************************************************************/
	void PauseResumeButton::Update()
	{
		if (TIME->IsPaused())
		{
			Vec2<float> mouse_pos = INPUT->GetMousePos();
			if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
			{
				Vec2<float> current_pos = GameObj->GetComponent<CPTransform>()->GetPosition();
				mouse_pos.y = mouse_pos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();

				mouse_pos.y *= 1080.f / (float)WINDOWSSYSTEM->GetHeight();
				mouse_pos.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();
				std::vector<CPCollider*> vec_colliders = COLLIDERS->OverlapCircle(mouse_pos, 1.0f);

				// Loop through the colliders found on mouse click
				for (size_t i = 0; i < vec_colliders.size(); ++i)
				{
					// If we found the block
					if (vec_colliders[i] == GameObj->GetComponent<CPCollider>())
					{
						TIME->Pause(false);

						MenuTextureObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						MenuTextureObject->GetComponent<CPTransform>()->SetScale(Vec2<float>{1.f, 1.f});

						ResumeButtonObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						ResumeButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

						HowToPlayButtonObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						HowToPlayButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

						QuitButtonObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						QuitButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

					}
				}
			}
		}
	}

	/*!***********************************************************************
	\brief
	 Override destroy function
	*************************************************************************/
	void PauseResumeButton::Destroy()
	{

	}
}