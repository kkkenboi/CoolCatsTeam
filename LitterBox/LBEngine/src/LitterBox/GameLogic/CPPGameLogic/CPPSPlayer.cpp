/*!************************************************************************
 \file				CPPSPlayer.cpp
 \author(s)			Vanessa Chua Siew Jin, Ryan Tan Jian Hao
 \par DP email(s):	vanessasiewjin@digipen.edu, ryanjianhao.tan\@digipen.edu
 \par Course:		CSD2401A
 \date				09-02-2024
 \brief
 This file contains the CPPSPlayer class and all its functionalities,

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSPlayer.h"

#include "CPPSPlayerGolfBall.h"
#include "CPPSUpgradeManager.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Engine/Time.h"
#include <array>
#include <random>
#include "CPPSPlayerHUD.h"
#include "CPPSUpgradeManager.h"
#include "LitterBox/Renderer/Camera.h"
#include "CPPGameManager.h"
#include "CPPSCameraFollow.h"
#include "CPPSBramble.h"

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
		m_GameManager = GOMANAGER->FindGameObjectWithName("GameManager");
		m_CameraFollow = GOMANAGER->FindGameObjectWithName("CameraFollow");

		m_stepSoundInterval = 0.2f;
		m_stepSoundCurrent = 0.0f;

		m_shootForce = 4500.0f;
		m_shootRadius = 120.0f;

		// 1 seconds of invincibility
		mGotAttackedCooldown = 0;
		mGotAttacked = 1.f;

		// 0.5 seconds of stun
		mIsStunned = false;
		mStunTimer = 0.5f;
		mStunRemaining = mStunTimer;

		// So that balls don't spawn on top each other
		rb->addForce(Vec2<float>{10.0f, 0.0f} * TIME->GetDeltaTime());

		//---------------------------getting the uvs for the run------------------------
		//if (LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["walking_cat"]) != LB::ASSETMANAGER->Textures.end()) {
		//	//int img_width{ LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["walking_cat"])->second.first->width }; NOTREFERENCED

		//	float x_inc{ 1.f / 10.f };

		//	for (int x{ 0 }; x < 10; ++x) {
		//		frames[x].at(0) = { x * x_inc, 0.f };//bottom left
		//		frames[x].at(1) = { (x + 1) * x_inc, 0.f };//bottom right
		//		frames[x].at(2) = { (x + 1) * x_inc, 1.f };//top right
		//		frames[x].at(3) = { x * x_inc, 1.f };//top left
		//	}
		//}
		//else DebuggerLogWarning("Can't find!");
		//Renderer::GRAPHICS->init_anim("player_walk", frames.data(), 0.1f, 10);
		//Renderer::GRAPHICS->init_anim("player_idle", frames.data(), 1.f, 1);

		//rend->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("walking_cat"), static_cast<int>(rend->w), static_cast<int>(rend->h));
		//rend->play_repeat("player_idle");

		onTakingDamage.Subscribe(DecreaseHealth);
		onPlacingBall.Subscribe(IncreaseBalls);
		onPlayerDeathEvent.Subscribe(ShowGameOver);	//GameManager event
	}

	/*!***********************************************************************
	\brief
	Update function (Updates the keypresses and checks keypresses)
	*************************************************************************/
	void CPPSPlayer::Update()
	{
		if (TIME->IsPaused()) return;
		if (m_GameManager->GetComponent<CPPSGameManager>()->isGameOver) return;

		
		if (mGotAttackedCooldown > 0.0f) {
			mGotAttackedCooldown -= static_cast<float>(TIME->GetDeltaTime());
		}

		/*!***********************************************************************
		\brief
		Movement animation of the player
		*************************************************************************/
		//------------------Walking animation------------------
		static bool isWalkingAnim{ false };
		if (INPUT->IsKeyTriggered(KeyCode::KEY_W) && !mIsStunned)
		{
			//rend->stop_anim();
			//rend->play_repeat("player_walk");

			GetComponent<CPAnimator>()->PlayRepeat("FelixWalk");

			isWalkingAnim = true;
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_A) && !mIsStunned)
		{
			//rend->stop_anim();
			//rend->play_repeat("player_walk");

			GetComponent<CPAnimator>()->PlayRepeat("FelixWalk");

			isWalkingAnim = true;
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_D) && !mIsStunned)
		{
			//rend->stop_anim();
			//rend->play_repeat("player_walk");

			GetComponent<CPAnimator>()->PlayRepeat("FelixWalk");

			isWalkingAnim = true;
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_S) && !mIsStunned)
		{
			//rend->stop_anim();
			//rend->play_repeat("player_walk");

			GetComponent<CPAnimator>()->PlayRepeat("FelixWalk");

			isWalkingAnim = true;
		}


		/*!***********************************************************************
		\brief
		Movement of the player
		*************************************************************************/
		//------------------Movement WASD------------------
		bool isMoving{ false };
		Vec2<float> movement{ 0.f,0.f };
		Vec2<float> externalForce = rb->mVelocity;
		if (INPUT->IsKeyPressed(KeyCode::KEY_W) && !mIsStunned)
		{
			//rb->addForce(Vec2<float>{0.f, m_walkSpeed} * TIME->GetDeltaTime());
			//rb->mVelocity += Vec2<float>{0.f, m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed} *TIME->GetDeltaTime(); 
			//movement += Vec2<float>{ 0.f, m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed };
			movement.y += 1.f;
			isMoving = true;
		}
		if (INPUT->IsKeyPressed(KeyCode::KEY_S) && !mIsStunned)
		{
			//rb->addForce(Vec2<float>{0.f, -m_walkSpeed} * TIME->GetDeltaTime());
			//rb->mVelocity -= Vec2<float>{0.f, m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed} *TIME->GetDeltaTime();
			//movement += Vec2<float>{ 0.f, -m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed };
			movement.y += -1.f;
			isMoving = true;
		}
		if (INPUT->IsKeyPressed(KeyCode::KEY_A) && !mIsStunned)
		{
			//rb->addForce(Vec2<float>{-m_walkSpeed, 0.f} * TIME->GetDeltaTime());
			//rb->mVelocity += Vec2<float>{-m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed, 0.f} *TIME->GetDeltaTime();
			//movement += Vec2<float>{ -m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed, 0.f };
			movement.x += -1.f;
			isMoving = true;
		}
		if (INPUT->IsKeyPressed(KeyCode::KEY_D) && !mIsStunned)
		{
			//rb->addForce(Vec2<float>{m_walkSpeed, 0.f} * TIME->GetDeltaTime());
			//rb->mVelocity += Vec2<float>{m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed, 0.f} *TIME->GetDeltaTime();
			//movement += Vec2<float>{ m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed, 0.f };
			movement.x += 1.f;
			isMoving = true;
		}

		if (movement.x != 0.f || movement.y != 0.f) {
			movement = Normalise(movement);
		}

		if (!mIsStunned) {
			//rb->mVelocity = movement * m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed;
			Vec2<float> AddedVelocity = movement * m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed;
			rb->mVelocity += (AddedVelocity  - rb->mVelocity) * 10.f * static_cast<float>(TIME->GetDeltaTime());
		}

		//clamping of the speed of the player movement
		rb->mVelocity.x = Clamp<float>(rb->mVelocity.x, -m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxSpeed, m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxSpeed);
		rb->mVelocity.y = Clamp<float>(rb->mVelocity.y, -m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxSpeed, m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxSpeed);

		if (!isMoving)
		{
			//rb->addForce(-rb->mVelocity * 5.0f * TIME->GetDeltaTime());
			rb->mVelocity *= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerArbitraryFriction;
			 if (isWalkingAnim)
			 {
			 	//rend->stop_anim();
			 	//rend->play_repeat("player_idle");
				 if (!mIsStunned) {
					 GetComponent<CPAnimator>()->Stop();
				 }
			 	isWalkingAnim = false;
			 }
			GameObj->GetComponent<CPTransform>()->GetChild()->gameObj->GetComponent<CPParticle>()->mIsActive = false;
		}
		if (isMoving) GameObj->GetComponent<CPTransform>()->GetChild()->gameObj->GetComponent<CPParticle>()->mIsActive = true;
		//------------------Play step sound------------------
		if (isMoving && m_stepSoundCurrent > m_stepSoundInterval)
		{
			m_stepSoundCurrent = 0.0f;
			AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->PlayerFootStepsSounds,0.5f);
		}
		m_stepSoundCurrent += static_cast<float>(TIME->GetDeltaTime());
		
		/*!***********************************************************************
		\brief
		Shooting and spawnining of the golf balls
		*************************************************************************/
		//------------------Pushes balls away from the player in a circle------------------
		if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_2))
		{
			hasPlayedHitSound = false;
			// Play hit sound
			AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->PlayerSlashSounds,0.3f);

			// Pushes the ball
			Vec2<float> current_pos = GameObj->GetComponent<CPTransform>()->GetPosition();

			DEBUG->DrawCircle(current_pos, m_shootRadius, Vec4<float>{0.f, 0.f, 0.5f, 1.0f});
			std::vector<CPCollider*> vec_colliders = COLLIDERS->OverlapCircle(current_pos, m_shootRadius);

			Vec2<float> mouse_pos = INPUT->GetMousePos();
			mouse_pos.x += m_CameraFollow->GetComponent<CPPSCameraFollow>()->cameraPos.x;
			mouse_pos.y -= m_CameraFollow->GetComponent<CPPSCameraFollow>()->cameraPos.y;
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
						if (vec_colliders[i]->gameObj->GetName() == "ball")
						{
							vec_colliders[i]->gameObj->GetComponent<CPPSPlayerGolfBall>()->Split(force_to_apply);
							if (!hasPlayedHitSound)
							{
								AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->PlayerHitBallSounds, 0.5f);
								hasPlayedHitSound = true;
							}
						}
					}
				}
			}
		}

		//------------------Spawn a golf ball----------------------
		if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
		{
			if (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentBalls >= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls) return;
			
			onPlacingBall.Invoke();
			
			//Spawn Game Object
			GameObject* ballObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("ball", *ballObject);
			int Channel = AUDIOMANAGER->PlaySound("Ball into hole");
			AUDIOMANAGER->SetChannelVolume(Channel, 0.7f);
			Vec2<float> playerPos = GameObj->GetComponent<CPTransform>()->GetPosition();
			playerPos.x += m_isFacingLeft ? -50.0f : 50.0f;

			ballObject->GetComponent<CPTransform>()->SetPosition(playerPos);
			ballObject->GetComponent<CPPSPlayerGolfBall>()->SetBallUpgrade(GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->GetBallUpgrades());
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

		// Update the Stunned timer
		if (mIsStunned) {
			mStunRemaining -= TIME->GetDeltaTime();
			if (mStunRemaining <= 0.f) {
				mIsStunned = false;
				mStunRemaining = mStunTimer;
			}
		}
	} // End of Update

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
		if (m_GameManager->GetComponent<CPPSGameManager>()->isGameOver) return;
		if (colData.colliderOther->m_gameobj->GetName() == "Projectile" ||
			colData.colliderOther->m_gameobj->GetName() == "Mage" ||
			colData.colliderOther->m_gameobj->GetName() == "EnemyChaser1" ||
			colData.colliderOther->m_gameobj->GetName() == "Charger_Shield" ||
			colData.colliderOther->m_gameobj->GetName() == "Bramble" )
		{
			if (mGotAttackedCooldown > 0) return;
			//shake the cam
			if (mGotAttackedCooldown <= 0.0f) {
				Renderer::GRAPHICS->shake_camera(20.f, .1f);
				if (colData.colliderOther->m_gameobj->GetName() == "EnemyChaser1")
				{
					AUDIOMANAGER->ChanceToPlayGroupSound(AUDIOMANAGER->ChaserAttackSounds);
				}
				if (!mIsStunned) {
					//rb->mVelocity *= 10.f;
					Vec2<float> otherPos = colData.colliderOther->transform->GetPosition();
					Vec2<float> knockBack = colData.colliderThis->transform->GetPosition() - otherPos;
					if (colData.colliderOther->m_gameobj->GetName() != "Projectile") 
					{
						colData.colliderThis->rigidbody->mVelocity += knockBack * 15.f;
					}
					else 
					{
						colData.colliderThis->rigidbody->mVelocity += knockBack * 100.f;
					}
					mStunTimer = 0.5f;
					mIsStunned = true;
				}
			}
			mGotAttackedCooldown = mGotAttacked;

			GetComponent<CPAnimator>()->Play("FelixHurt");

			AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->PlayerHurtSounds, 0.4f);
			// Update the HUD as well
			onTakingDamage.Invoke();
			//if the player is dead
			if (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth <= 0)
			{
				//We send a COPY of the gameobj that killed the player because I think it doesn't
				//have to be the actual thing
				onPlayerDeathEvent.Invoke(*colData.colliderOther->gameObj);
			}
		}
		if (colData.colliderOther->m_gameobj->GetName() == "Mushroom") 
		{
			if (!mIsStunned) {
				//rb->mVelocity *= 10.f;
				Vec2<float> otherPos = colData.colliderOther->transform->GetPosition();
				Vec2<float> knockBack = colData.colliderThis->transform->GetPosition() - otherPos;
				if (colData.colliderOther->m_gameobj->GetName() != "Projectile")
				{
					colData.colliderThis->rigidbody->mVelocity += knockBack * 15.f;
				}
				else
				{
					colData.colliderThis->rigidbody->mVelocity += knockBack * 100.f;
				}
				mStunTimer = 0.15f;
				mIsStunned = true;
			}
		}
	}
}