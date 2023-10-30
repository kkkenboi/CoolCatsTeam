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

		//---------------------------getting the uvs for the run------------------------
		if (LB::ASSETMANAGER->Textures.find("run") != LB::ASSETMANAGER->Textures.end()) {
			int img_width{ LB::ASSETMANAGER->Textures.find("run")->second.first->width };
			int img_height{ LB::ASSETMANAGER->Textures.find("run")->second.first->height };

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

		rend->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("run"));
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

		//if (INPUT->IsKeyPressed(KeyCode::KEY_W))
		//{
		//	rb->addForce(Vec2<float>{0.f, 5000.f});
		//}
		//else if (INPUT->IsKeyPressed(KeyCode::KEY_S))
		//{
		//	rb->addForce(Vec2<float>{-5000.f, 0.f});
		//}
		//else if (INPUT->IsKeyPressed(KeyCode::KEY_A))
		//{
		//	rb->addForce(Vec2<float>{-5000.f, 0.f});
		//}
		//else if (INPUT->IsKeyPressed(KeyCode::KEY_D))
		//{
		//	rb->addForce(Vec2<float>{5000.f, 0.f});
		//}
	}

	void CPPSPlayer::Destroy()
	{

	}
}