/*!************************************************************************
 \file				PauseQuitNoButton.cpp
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:		CSD2401A
 \date				26-11-2023
 \brief
 This file contains the functionality of the No button for the Quit
 confirmation pop up

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "Platform/Windows/Windows.h"
#include "PauseQuitNoButton.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Core/Core.h"

namespace LB
{
	void PauseQuitNoButton::Start()
	{

		// Grab All The GOs associated with the Pause Menu
		for (GameObject* GO : GOMANAGER->GetGameObjects())
		{
			// Confirm Menu
			if (GO->GetName() == "PauseMenuQuitConfirmTexture")
			{
				ConfirmMenuTexture = GO;
			}
			if (GO->GetName() == "PauseMenuQuitConfirmYesObject")
			{
				ConfirmMenuYesButton = GO;
			}
			if (GO->GetName() == "PauseMenuQuitConfirmNoObject")
			{
				ConfirmMenuNoButton = GO;
			}

			// Pause Menu
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

		if (GameObj->HasComponent<CPCollider>())
		{
			mCollider = GameObj->GetComponent<CPCollider>();
		}
		else
		{
			mCollider = nullptr;
			return;
		}
	}

	void PauseQuitNoButton::Update() 
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
				if (vec_colliders[i] == mCollider)
				{
					if(PauseMenuTexture)
						PauseMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.f, 529.f});
					if (PauseMenuResumeButton)
						PauseMenuResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.66f, 585.23f});
					if (PauseMenuHowToPlayButton)
						PauseMenuHowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 487.13f});
					if (PauseMenuExitButton)
						PauseMenuExitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.22f, 310.78f});


					ConfirmMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					ConfirmMenuYesButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					ConfirmMenuNoButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
				}
			}
		}
	}

	void PauseQuitNoButton::Destroy()
	{
	
	}
}