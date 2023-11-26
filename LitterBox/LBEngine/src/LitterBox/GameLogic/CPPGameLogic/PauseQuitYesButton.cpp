/*!************************************************************************
 \file				PauseQuitYesButton.cpp
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
#include "PauseQuitYesButton.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Core/Core.h"

namespace LB 
{
	/*!***********************************************************************
	\brief
	 Initializes the GameObjects needed for the script
	*************************************************************************/
	void PauseQuitYesButton::Start() 
	{

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

	/*!***********************************************************************
	\brief
	 Allows for button press and movement of other game objects into view
	*************************************************************************/
	void PauseQuitYesButton::Update()
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
					MessageQuit q;
					CORE->BroadcastMessage(&q);
				}
			}
		}
	}

	/*!***********************************************************************
	\brief
	 Override destroy function
	*************************************************************************/
	void PauseQuitYesButton::Destroy()
	{

	}
}