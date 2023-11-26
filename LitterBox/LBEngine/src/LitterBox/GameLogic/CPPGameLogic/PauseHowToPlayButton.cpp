/*!************************************************************************
 \file				PauseHowToPlayButton.cpp
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:		CSD2401A
 \date				26-11-2023
 \brief
 This file contains the functionality of the button for the how to play
 screen during gameplay

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "PauseHowToPlayButton.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	 Initializes the GameObjects needed for the script
	*************************************************************************/
	void PauseMenuHowToPlayButton::Start()
	{
		std::vector<GameObject*> GOs = GOMANAGER->GetGameObjects();
		for (GameObject* GO : GOs) {
			if (GO->GetName() == "PauseMenuHowToPlayTextureObject") 
			{
				HowToPlayTexture = GO;
			}
			if (GO->GetName() == "PauseMenuHowToPlayPreviousMenuButton")
			{
				HowToPlayBackButton = GO;
			}
			if (GO->GetName() == "PauseMenuTextureObject")
			{
				PauseMenuTexture = GO;
			}
			if (GO->GetName() == "PauseMenuResumeObject")
			{
				PauseMenuResumeButton = GO;
			}
			if (GO->GetName() == "PauseMenuHowToPlayObject") 
			{
				PauseMenuHowToPlayButton = GO;
			}
			if (GO->GetName() == "PauseMenuQuitObject")
			{
				PauseMenuExitButton = GO;
			}

		}
		
		mCollider = this->GameObj->GetComponent<CPCollider>();
	}

	/*!***********************************************************************
	\brief
	 Allows for button press and movement of other game objects into view
	*************************************************************************/
	void PauseMenuHowToPlayButton::Update()
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
				if (vec_colliders[i] == mCollider)
				{
					// If we found the block
					// Move all the Pause Menu Objects away
					PauseMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					PauseMenuResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					PauseMenuHowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					PauseMenuExitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

					// Move all the How To Play Objects into scene
					HowToPlayTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{948.88f, 545.65f});
					HowToPlayTexture->GetComponent<CPTransform>()->SetScale(Vec2<float>{19.80f, 10.90f});

					HowToPlayBackButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{161.75f, 72.12f});
					HowToPlayBackButton->GetComponent<CPCollider>()->SetWidthHeightRadius(265.f, 222.f, 50.f);
				}
			}
		}
	}

	/*!***********************************************************************
	\brief
	 Override destroy function
	*************************************************************************/
	void PauseMenuHowToPlayButton::Destroy()
	{

	}
}