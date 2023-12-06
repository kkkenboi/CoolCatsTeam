/*!************************************************************************
 \file				CPPSPlayer.cpp
 \author(s)			Vanessa Chua Siew Jin, Ryan Tan Jian Hao
 \par DP email(s):	vanessasiewjin@digipen.edu, ryanjianhao.tan\@digipen.edu
 \par Course:		CSD2401A
 \date				03-11-2023
 \brief
 This file contains the CPPSPlayer class and all its functionalities,

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSPlayer.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Engine/Time.h"
#include <array>
#include <random>

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
		col = GameObj->GetComponent<CPCollider>();

		right_face = trans->GetScale();
		left_face = trans->GetScale();
		left_face.x = -left_face.x;

		//--------------------------Variables initializaiton----------------------------
		m_maxSpeed = 30000.0f;
		m_walkSpeed = 150000.0f;
		m_stepSoundInterval = 0.2f;
		m_stepSoundCurrent = 0.0f;

		m_shootForce = 4500.0f;
		m_shootRadius = 120.0f;

		m_maxBalls = 3;
		m_currentBalls = 0;

		m_health = 3;

		// 0.5 seconds of invincibility
		mGotAttackedCooldown = 0;
		mGotAttacked = 0.5f;

		// So that balls don't spawn on top each other
		rb->addForce(Vec2<float>{10.0f, 0.0f} * TIME->GetDeltaTime());

		//---------------------------getting the uvs for the run------------------------
		if (LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["walking_cat"]) != LB::ASSETMANAGER->Textures.end()) {
			//int img_width{ LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["walking_cat"])->second.first->width }; NOTREFERENCED

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

		rend->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("walking_cat"), static_cast<int>(rend->w), static_cast<int>(rend->h));
		rend->play_repeat("player_idle");
	}

	/*!***********************************************************************
	\brief
	Update function (Updates the keypresses and checks keypresses)
	*************************************************************************/
	void CPPSPlayer::Update()
	{
		if (TIME->IsPaused()) return;
		/*!***********************************************************************
		\brief
		When "8" is pressed on the key, it will spawn a mage
		*************************************************************************/
		//-----------------TESTING SPAWN-----------------------
		//Spawn Mage
		if (INPUT->IsKeyTriggered(KeyCode::KEY_8))
		{
			Vec2<float> mouse_pos = INPUT->GetMousePos();
			mouse_pos.y = mouse_pos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();
			mouse_pos.y *= 1080.f / (float)WINDOWSSYSTEM->GetHeight();
			mouse_pos.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();

			GameObject* mageObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("Mage", *mageObject);
			mageObject->GetComponent<CPTransform>()->SetPosition(mouse_pos);
		}
		/*!***********************************************************************
		\brief
		When "9" is pressed on the key, it will spawn a Chaser
		*************************************************************************/
		//Spawn Chaser
		if (INPUT->IsKeyTriggered(KeyCode::KEY_9))
		{
			Vec2<float> mouse_pos = INPUT->GetMousePos();
			mouse_pos.y = mouse_pos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();
			mouse_pos.y *= 1080.f / (float)WINDOWSSYSTEM->GetHeight();
			mouse_pos.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();

			GameObject* chaserObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("EnemyChaser1", *chaserObject);
			chaserObject->GetComponent<CPTransform>()->SetPosition(mouse_pos);
		}

		if (mGotAttackedCooldown > 0.0f) {
			mGotAttackedCooldown -= static_cast<float>(TIME->GetDeltaTime());
		}

		/*!***********************************************************************
		\brief
		Movement animation of the player
		*************************************************************************/
		//------------------Walking animation------------------
		static bool isWalkingAnim{ false };
		if (INPUT->IsKeyTriggered(KeyCode::KEY_W))
		{
			rend->stop_anim();
			rend->play_repeat("player_walk");

			isWalkingAnim = true;
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_A))
		{
			rend->stop_anim();
			rend->play_repeat("player_walk");

			isWalkingAnim = true;
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_D))
		{
			rend->stop_anim();
			rend->play_repeat("player_walk");

			isWalkingAnim = true;
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_S))
		{
			rend->stop_anim();
			rend->play_repeat("player_walk");

			isWalkingAnim = true;
		}

		/*!***********************************************************************
		\brief
		Movement of the player
		*************************************************************************/
		//------------------Movement WASD------------------
		bool isMoving{ false };
		if (INPUT->IsKeyPressed(KeyCode::KEY_W))
		{
			rb->addForce(Vec2<float>{0.f, m_walkSpeed} * TIME->GetDeltaTime());
			isMoving = true;
		}
		if (INPUT->IsKeyPressed(KeyCode::KEY_S))
		{
			rb->addForce(Vec2<float>{0.f, -m_walkSpeed} * TIME->GetDeltaTime());
			isMoving = true;
		}
		if (INPUT->IsKeyPressed(KeyCode::KEY_A))
		{
			rb->addForce(Vec2<float>{-m_walkSpeed, 0.f} * TIME->GetDeltaTime());
			isMoving = true;
		}
		if (INPUT->IsKeyPressed(KeyCode::KEY_D))
		{
			rb->addForce(Vec2<float>{m_walkSpeed, 0.f} * TIME->GetDeltaTime());
			isMoving = true;
		}
		//clamping of the speed of the player movement
		rb->mVelocity.x = Clamp<float>(rb->mVelocity.x, -m_maxSpeed, m_maxSpeed);
		rb->mVelocity.y = Clamp<float>(rb->mVelocity.y, -m_maxSpeed, m_maxSpeed);

		if (!isMoving)
		{
			rb->addForce(-rb->mVelocity * 5.0f * TIME->GetDeltaTime());

			if (isWalkingAnim)
			{
				rend->stop_anim();
				rend->play_repeat("player_idle");
				isWalkingAnim = false;
			}
		}

		//------------------Play step sound------------------
		if (isMoving && m_stepSoundCurrent > m_stepSoundInterval)
		{
			m_stepSoundCurrent = 0.0f;
			switch (std::rand() % 4)
			{
			case 0:
				AUDIOMANAGER->PlaySound("Footsteps-Grass-Far-Small_1");
				break;
			case 1:
				AUDIOMANAGER->PlaySound("Footsteps-Grass-Far-Small_2");
				break;
			case 2:
				AUDIOMANAGER->PlaySound("Footsteps-Grass-Far-Small_3");
				break;
			case 3:
				AUDIOMANAGER->PlaySound("Footsteps-Grass-Far-Small_4");
				break;
			}
		}
		m_stepSoundCurrent += static_cast<float>(TIME->GetDeltaTime());
		
		/*!***********************************************************************
		\brief
		Shooting and spawnining of the golf balls
		*************************************************************************/
		//------------------Pushes balls away from the player in a circle------------------
		if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_2))
		{
			// Play hit sound
			int Channel = AUDIOMANAGER->PlaySound("Sward-Whoosh_1");
			AUDIOMANAGER->SetChannelVolume(Channel, 0.3f);

			// Pushes the ball
			Vec2<float> current_pos = GameObj->GetComponent<CPTransform>()->GetPosition();

			DEBUG->DrawCircle(current_pos, m_shootRadius, Vec4<float>{0.f, 0.f, 0.5f, 1.0f});
			std::vector<CPCollider*> vec_colliders = COLLIDERS->OverlapCircle(current_pos, m_shootRadius);

			Vec2<float> mouse_pos = INPUT->GetMousePos();
			mouse_pos.y = mouse_pos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();
			mouse_pos.y *= 1080.f / (float)WINDOWSSYSTEM->GetHeight();
			mouse_pos.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();

			for (size_t i = 0; i < vec_colliders.size(); ++i) {
				Vec2<float> force_to_apply = mouse_pos - vec_colliders[i]->m_pos;
				force_to_apply = Normalise(force_to_apply) * m_shootForce;

				if (vec_colliders[i]->rigidbody != nullptr)
				{
					if (vec_colliders[i] == GameObj->GetComponent<CPCollider>() || vec_colliders[i]->GetLayerName() != "PlayerBall")
					{
						continue;
					}
					if (!TIME->IsPaused())
					{
						vec_colliders[i]->rigidbody->addImpulse(force_to_apply); //* TIME->GetDeltaTime());
					}
				}
			}
		}

		//------------------Spawn a golf ball----------------------
		if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
		{
			if (m_currentBalls >= m_maxBalls) return;
			++m_currentBalls;

			//Spawn Game Object
			GameObject* ballObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("ball", *ballObject);

			Vec2<float> playerPos = GameObj->GetComponent<CPTransform>()->GetPosition();
			playerPos.x += m_isFacingLeft ? -50.0f : 50.0f;

			ballObject->GetComponent<CPTransform>()->SetPosition(playerPos);
		}

		/*!***********************************************************************
		\brief
		Getting direction of where the mous is and making the player face to that direction
		*************************************************************************/
		//------------------Player face mouse pos------------------
		Vec2<float> playerPos = GameObj->GetComponent<CPTransform>()->GetPosition();
		Vec2<float> mousePos = INPUT->GetMousePos();
		
		mousePos.y = mousePos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();
		mousePos.y *= 1080.f / (float)WINDOWSSYSTEM->GetHeight();
		mousePos.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();
		
		Vec2<float> playerToMouseDir = mousePos - playerPos;
		Vec2<float> TransformRight{ 1,0 };

		m_isFacingLeft = DotProduct(playerToMouseDir.Normalise(), TransformRight) < 0.0f;

		if (m_isFacingLeft)
		{
			trans->SetScale(left_face);
		} else trans->SetScale(right_face);
	}

	/*!***********************************************************************
	\brief
	Destroy function (will delete the states)
	*************************************************************************/
	void CPPSPlayer::Destroy(){ }

	/*!***********************************************************************
	\brief
	When on collision  with either Projectile, Mage, Chase, it will get hurt and --m_health,
	*************************************************************************/
	void CPPSPlayer::OnCollisionEnter(CollisionData colData) 
	{
		if (colData.colliderOther->m_gameobj->GetName() == "Projectile" ||
			colData.colliderOther->m_gameobj->GetName() == "Mage" ||
			colData.colliderOther->m_gameobj->GetName() == "EnemyChaser1")
		{
			if (mGotAttackedCooldown > 0.0f) {
				return;
			}
			mGotAttackedCooldown = mGotAttacked;
			int Channel{ 0 };
			switch (std::rand() % 3)
			{
			case 0:
				Channel = AUDIOMANAGER->PlaySound("playerhurt1");
				break;
			case 1:
				Channel = AUDIOMANAGER->PlaySound("playerhurt2");
				break;
			case 2:
				Channel = AUDIOMANAGER->PlaySound("playerhurt3");
				break;
			}
			AUDIOMANAGER->SetChannelVolume(Channel, 0.5f);
			AUDIOMANAGER->SetChannelPitch(Channel, 1.1f);

			--m_health;

			if (m_health < 0)
			{
				//GOMANAGER->RemoveGameObject(this->GameObj);
			}

		}
	}
}