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
#include "CPPSMouseWorld.h"
#include "LitterBox/Core/Core.h"

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
		anim = GameObj->GetComponent<CPAnimator>();
		particle = trans->GetChild()->GetComponent<CPParticle>();

		right_face = trans->GetScale();
		left_face = trans->GetScale();
		left_face.x = -left_face.x;

		//--------------------------Variables initializaiton----------------------------
		m_GameManager = GOMANAGER->FindGameObjectWithName("GameManager");
		m_CameraFollow = GOMANAGER->FindGameObjectWithName("CameraFollow");
		m_MouseWorld = GOMANAGER->FindGameObjectWithName("MouseWorld");

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

		m_particleEmitRate = particle->mEmitterRate;

		// So that balls don't spawn on top each other
		rb->addForce(Vec2<float>{10.0f, 0.0f} * TIME->GetDeltaTime());

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
		if (m_GameManager->GetComponent<CPPSGameManager>()->isMovementDisabled)
		{
			//We reset the velocity first just incase
			rb->mVelocity = Vec2<float>(0, 0);
			return;
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
		if (INPUT->IsKeyTriggered(KeyCode::KEY_W) && !mIsStunned)
		{
			anim->PlayRepeat("Felix_Walk");

			isWalkingAnim = true;
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_A) && !mIsStunned)
		{
			anim->PlayRepeat("Felix_Walk");

			isWalkingAnim = true;
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_D) && !mIsStunned)
		{
			anim->PlayRepeat("Felix_Walk");

			isWalkingAnim = true;
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_S) && !mIsStunned)
		{
			anim->PlayRepeat("Felix_Walk");

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
			movement.y += 1.f;
			isMoving = true;
		}
		if (INPUT->IsKeyPressed(KeyCode::KEY_S) && !mIsStunned)
		{
			movement.y += -1.f;
			isMoving = true;
		}
		if (INPUT->IsKeyPressed(KeyCode::KEY_A) && !mIsStunned)
		{
			movement.x += -1.f;
			isMoving = true;
		}
		if (INPUT->IsKeyPressed(KeyCode::KEY_D) && !mIsStunned)
		{
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
				 //if (!mIsStunned) {
				anim->StopAndReset();
				 //}
			 	isWalkingAnim = false;
			 }
			 particle->mIsActive = false;
		}
		if (isMoving) particle->mIsActive = true;
		//------------------Play step sound------------------
		if (isMoving && m_stepSoundCurrent > m_stepSoundInterval)
		{
			m_stepSoundCurrent = 0.0f;
			AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->PlayerFootStepsSounds,0.3f);
		}
		m_stepSoundCurrent += static_cast<float>(TIME->GetDeltaTime()) * anim->m_playSpeed;
		
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
			ballObject->GetComponent<CPTransform>()->SetScale(Vec2<float>{0.8f, 0.8f});
			ballObject->GetComponent<CPPSPlayerGolfBall>()->SetBallUpgrade(GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->GetBallUpgrades());
		}

		/*!***********************************************************************
		\brief
		Getting direction of where the mous is and making the player face to that direction
		*************************************************************************/
		//------------------Player face mouse pos------------------
		Vec2<float> playerPos = GameObj->GetComponent<CPTransform>()->GetPosition();
		Vec2<float> mousePos = m_MouseWorld->GetComponent<CPPSMouseWorld>()->GetComponent<CPTransform>()->GetPosition();
		
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
			colData.colliderOther->m_gameobj->GetName() == "Shield" ||
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

			anim->PlayAndReset("Felix_Hurt");

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

	void CPPSPlayer::OnCollisionStay(CollisionData colData)
	{
		if (colData.colliderOther->gameObj->GetName() == "Sandpit")
		{
			anim->m_playSpeed = 0.6f;
			particle->mEmitterRate = m_particleEmitRate * 0.6f;
		}
	}

	void CPPSPlayer::OnCollisionExit(CollisionData colData)
	{
		if (colData.colliderOther->gameObj->GetName() == "Sandpit")
		{
			anim->m_playSpeed = 1.0f;
			particle->mEmitterRate = m_particleEmitRate;
		}
	}
}