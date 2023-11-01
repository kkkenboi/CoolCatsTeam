/*!************************************************************************
 \file				CPPSPlayer.cpp
 \author(s)			
 \par DP email(s):	
 \par Course:		CSD2401A
 \date				28-09-2023
 \brief
 This file contains 

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "Platform/Editor/EditorGameView.h"
#include "CPPSPlayer.h"
#include "LitterBox/Engine/Input.h"
#include <array>

namespace LB
{
	std::array<std::array<LB::Vec2<float>, 4>, 12> frames[4];

	void CPPSPlayer::Start()
	{
		rb = GameObj->GetComponent<CPRigidBody>();

		rend = GameObj->GetComponent<CPRender>();

		DebuggerLogWarningFormat("Found run! %d", LB::ASSETMANAGER->Textures.find("anim") != LB::ASSETMANAGER->Textures.end());

		//---------------------------getting the uvs for the run------------------------
		if (LB::ASSETMANAGER->Textures.find("anim") != LB::ASSETMANAGER->Textures.end()) {
			int img_width{ LB::ASSETMANAGER->Textures.find("anim")->second.first->width };
			int img_height{ LB::ASSETMANAGER->Textures.find("anim")->second.first->height };

			float x_inc{ (float)img_width / (12.f * (float)img_width) };
			float y_inc{ (float)img_height / (4.f * (float)img_height) };

			for (int y{ 0 }; y < 4; ++y)
				for (int x{ 0 }; x < 12; ++x) {
					frames[y][x].at(0) = { x * x_inc, y * y_inc };//bottom left
					frames[y][x].at(1) = { (x + 1) * x_inc, y * y_inc };//bottom right
					frames[y][x].at(2) = { (x + 1) * x_inc, (y + 1) * y_inc };//top right
					frames[y][x].at(3) = { x * x_inc, (y + 1) * y_inc };//top left
				}
		}
		Renderer::GRAPHICS->init_anim("up_walk", frames[0].data(), 0.5f, 12);
		Renderer::GRAPHICS->init_anim("right_walk", frames[1].data(), 0.5f, 12);
		Renderer::GRAPHICS->init_anim("left_walk", frames[2].data(), 0.5f, 12);
		Renderer::GRAPHICS->init_anim("down_walk", frames[3].data(), 0.5f, 12);

		rend->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("anim"));
		rend->play_repeat("up_walk");
	}

	void CPPSPlayer::Update()
	{
		if (INPUT->IsKeyTriggered(KeyCode::KEY_W))
		{
			rend->stop_anim();
			rend->play_repeat("up_walk");
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_A))
		{
			rend->stop_anim();
			rend->play_repeat("left_walk");
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_D))
		{
			rend->stop_anim();
			rend->play_repeat("right_walk");
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_S))
		{
			rend->stop_anim();
			rend->play_repeat("down_walk");
		}

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
		
		if (INPUT->IsKeyPressed(KeyCode::KEY_F))
		{
			Vec2<float> current_pos = GameObj->GetComponent<CPTransform>()->GetPosition();
			float effect_radius = 200.f;
			float effect_magnitude = 1000.f;

			DEBUG->DrawCircle(current_pos, effect_radius, Vec4<float>{0.f, 0.f, 0.5f, 1.0f});

			std::vector<CPCollider*> vec_colliders = COLLIDERS->OverlapCircle(current_pos, effect_radius);

			Vec2<float> mouse_pos = EDITORGAMEVIEW->GetMousePos();

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

	void CPPSPlayer::Destroy()
	{

	}
}