/*!************************************************************************
 \file				CPPSPlayer.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				03-11-2023
 \brief
 This file contains the CPPSPlayer class and all its functionalities,

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Physics/ColliderManager.h"
//#include "EditorGameView.h"
#include "CPPSPlayer.h"
#include "LitterBox/Engine/Input.h"
#include <array>

namespace LB
{
	//This array is for the animation frames
	std::array<std::array<LB::Vec2<float>, 4>, 10> frames;

	/*!***********************************************************************
	\brief
	Start function (Basically initializes everything)
	*************************************************************************/
	void CPPSPlayer::Start()
	{
		rb = GameObj->GetComponent<CPRigidBody>();

		rend = GameObj->GetComponent<CPRender>();

		trans = GameObj->GetComponent<CPTransform>();
		right_face = trans->GetScale();
		left_face = trans->GetScale();
		left_face.x = -left_face.x;

		DebuggerLogWarningFormat("Found run! %d", LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["run"]) != LB::ASSETMANAGER->Textures.end());

		//---------------------------getting the uvs for the run------------------------
		if (LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["walking_cat"]) != LB::ASSETMANAGER->Textures.end()) {
			int img_width{ LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["walking_cat"])->second.first->width };

			float x_inc{ 1.f / 10.f };

			for (int x{ 0 }; x < 10; ++x) {
				frames[x].at(0) = { x * x_inc, 0.f };//bottom left
				frames[x].at(1) = { (x + 1) * x_inc, 0.f };//bottom right
				frames[x].at(2) = { (x + 1) * x_inc, 1.f };//top right
				frames[x].at(3) = { x * x_inc, 1.f };//top left
			}
		}
		else DebuggerLogWarning("Can't find!");
		Renderer::GRAPHICS->init_anim("player_walk", frames.data(), 0.1f, 10);
		Renderer::GRAPHICS->init_anim("player_idle", frames.data(), 1.f, 1);

		rend->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("walking_cat"));
		rend->play_repeat("player_idle");
	}

	/*!***********************************************************************
	\brief
	Update function (Updates the keypresses and checks keypresses)
	*************************************************************************/
	void CPPSPlayer::Update()
	{
		if (INPUT->IsKeyTriggered(KeyCode::KEY_W))
		{
			AUDIOMANAGER->PlaySound("Footsteps-Grass-Far-Small_1");
			rend->stop_anim();
			rend->play_repeat("player_walk");
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_A))
		{
			trans->SetScale(left_face);
			AUDIOMANAGER->PlaySound("Footsteps-Grass-Far-Small_2");
			rend->stop_anim();
			rend->play_repeat("player_walk");
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_D))
		{
			trans->SetScale(right_face);
			AUDIOMANAGER->PlaySound("Footsteps-Grass-Far-Small_3");
			rend->stop_anim();
			rend->play_repeat("player_walk");
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_S))
		{
			AUDIOMANAGER->PlaySound("Footsteps-Grass-Far-Small_4");
			rend->stop_anim();
			rend->play_repeat("player_walk");
		}

		// Movement WASD
		if (INPUT->IsKeyPressed(KeyCode::KEY_W))
		{
			rb->addForce(Vec2<float>{0.f, 5000.f});
		}
		if (INPUT->IsKeyPressed(KeyCode::KEY_S))
		{
			rb->addForce(Vec2<float>{0.f, -5000.f});
		}
		if (INPUT->IsKeyPressed(KeyCode::KEY_A))
		{
			rb->addForce(Vec2<float>{-5000.f, 0.f});
		}
		if (INPUT->IsKeyPressed(KeyCode::KEY_D))
		{
			rb->addForce(Vec2<float>{5000.f, 0.f});
		}

		// Rotation QE
		if (INPUT->IsKeyPressed(KeyCode::KEY_Q))
		{
			rb->addRotation(0.15f);
		}
		if (INPUT->IsKeyPressed(KeyCode::KEY_E))
		{
			rb->addRotation(-0.15f);
		}
		
		// Click check
		if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
		{
			DebuggerLogWarning("Mouse 1 is pressed!");
		}

		// Pushes everything away from the player in a circle
		if (INPUT->IsKeyPressed(KeyCode::KEY_F))
		{
			Vec2<float> current_pos = GameObj->GetComponent<CPTransform>()->GetPosition();
			float effect_radius = 200.f;
			float effect_magnitude = 500.f;

			DEBUG->DrawCircle(current_pos, effect_radius, Vec4<float>{0.f, 0.f, 0.5f, 1.0f});

			std::vector<CPCollider*> vec_colliders = COLLIDERS->OverlapCircle(current_pos, effect_radius);

			Vec2<float> mouse_pos = INPUT->GetMousePos();
			mouse_pos.y = mouse_pos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();

			//std::cout << vec_colliders.size() << std::endl;
			for (size_t i = 0; i < vec_colliders.size(); ++i) {
				Vec2<float> force_to_apply = mouse_pos - vec_colliders[i]->m_pos;
				force_to_apply = Normalise(force_to_apply) * effect_magnitude;

				if (vec_colliders[i]->rigidbody != nullptr)
				{
					if (vec_colliders[i] == GameObj->GetComponent<CPCollider>())
					{
						continue;
					}
					vec_colliders[i]->rigidbody->addImpulse(force_to_apply);
				}

			}
		}
		
	}

	/*!***********************************************************************
	\brief
	Destroy function (will delete the states)
	*************************************************************************/
	void CPPSPlayer::Destroy()
	{

	}

	void CPPSPlayer::OnCollisionEnter(CollisionData colData) 
	{

	}
}