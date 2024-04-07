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
#include "CPPSMouse.h"
#include "CPPSMouseWorld.h"
#include "LitterBox/Core/Core.h"
#include "CPPAudioManager.h"
#include "CPPVFXManager.h"

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
		trans = GameObj->GetComponent<CPTransform>();
		m_clubTrans = trans->GetChild()->GetChild()->GetChild();
		rend = trans->GetChild()->GetChild()->GetComponent<CPRender>();
		anim = trans->GetChild()->GetChild()->GetComponent<CPAnimator>();
		m_clubAnim = trans->GetChild()->GetChild()->GetChild()->GetChild()->GetComponent<CPAnimator>();
		m_moveAnim = trans->GetChild()->GetComponent<CPAnimator>();
		rb = GameObj->GetComponent<CPRigidBody>();
		col = GameObj->GetComponent<CPCollider>();
		particle = trans->GetChild(1)->GetComponent<CPParticle>();

		right_face = trans->GetScale();
		left_face = trans->GetScale();
		left_face.x = -left_face.x;

		//--------------------------Variables initializaiton----------------------------
		m_GameManager = GOMANAGER->FindGameObjectWithName("GameManager");
		m_AudioManager = GOMANAGER->FindGameObjectWithName("AudioManager");
		m_CameraFollow = GOMANAGER->FindGameObjectWithName("CameraFollow");
		m_MouseWorld = GOMANAGER->FindGameObjectWithName("MouseWorld");

		m_stepSoundInterval = 0.2f;
		m_stepSoundCurrent = 0.0f;

		m_shootForce = 3500.0f;
		m_shootRadius = 120.0f;

		// 1 seconds of invincibility
		mGotAttackedCooldown = 0;
		mGotAttacked = 1.f;

		// 0.5 seconds of stun
		mIsStunned = false;
		mStunTimer = 0.5f;
		mStunRemaining = mStunTimer;

		// Hand position from body
		m_handOffset = 5.0f;

		m_particleEmitRate = particle->mEmitterRate;

		// So that balls don't spawn on top each other
		//rb->addForce(Vec2<float>{10.0f, 0.0f} * TIME->GetDeltaTime());

		//Play some spawn sounds like "it's meow or never" [removed because doesn't fit]
		//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->PlayerPositiveSounds, 0.3f);

		// Gamepad anim bug fix
		m_moveAnim->PlayAndReset("Action_BigMove");
		m_moveAnim->StopAndReset();

		// Gamepad ref point
		m_Mouse = GOMANAGER->FindGameObjectWithName("MouseCursor");

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
		if (m_GameManager->GetComponent<CPPSGameManager>()->isMovementDisabled) return;
	
		if (mGotAttackedCooldown > 0.0f) {
			mGotAttackedCooldown -= static_cast<float>(TIME->GetDeltaTime());
		}

		//Anim is the body anim
		if (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth == 1 && !anim->IsPlaying())
		{
			anim->PlayRepeat("Felix_Low");
		}
		
		/*!***********************************************************************
		\brief
		Movement animation of the player
		*************************************************************************/
		//------------------Walking animation------------------
		static bool isWalkingAnim{ false };
		if (INPUT->IsKeyTriggered(KeyCode::KEY_W) && !mIsStunned)
		{
			m_moveAnim->PlayRepeat("Action_BigMove");
			//anim->PlayRepeat("Felix_Walk");
			isWalkingAnim = true;
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_A) && !mIsStunned)
		{
			m_moveAnim->PlayRepeat("Action_BigMove");
			//anim->PlayRepeat("Felix_Walk");
			isWalkingAnim = true;
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_D) && !mIsStunned)
		{
			m_moveAnim->PlayRepeat("Action_BigMove");
			//anim->PlayRepeat("Felix_Walk");
			isWalkingAnim = true;
		}
		else if (INPUT->IsKeyTriggered(KeyCode::KEY_S) && !mIsStunned)
		{
			m_moveAnim->PlayRepeat("Action_BigMove");
			//anim->PlayRepeat("Felix_Walk");
			isWalkingAnim = true;
		}

		/*!***********************************************************************
		\brief
		Movement of the player
		*************************************************************************/
		//------------------Movement WASD------------------
		isMoving = false;
		movement = Vec2<float>{ 0.f,0.f };
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

		m_Mouse->GetComponent<CPPSMouse>()->m_GamePadRef = trans->GetPosition();
		// GAMEPAD MOVEMENT
		if (INPUT->IsGamepadConnected() && !mIsStunned)
		{
			// Movement controls
			Vec2<float> const& joystickPos = INPUT->GetLeftJoystickPos();
			if (joystickPos.x != 0.f || joystickPos.y != 0.f) 
			{
				movement.x += joystickPos.x;
				movement.y += joystickPos.y;
				isMoving = true;
			}
		}

		if (movement.x != 0.f || movement.y != 0.f) {
			movement = Normalise(movement);
		}

		if (!mIsStunned) 
		{
			//rb->mVelocity = movement * m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed;
		/*	Vec2<float> AddedVelocity = movement * m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed;
			rb->mVelocity += (AddedVelocity  - rb->mVelocity) * 10.f * static_cast<float>(TIME->GetDeltaTime());*/

			// If move anim is not playing when it should, e.g. after stun or damage
			if (isMoving && !m_moveAnim->IsPlaying())
			{
				m_moveAnim->PlayRepeat("Action_BigMove");
				//anim->PlayRepeat("Felix_Walk");
				isWalkingAnim = true;
			}
		}

		////clamping of the speed of the player movement
		//rb->mVelocity.x = Clamp<float>(rb->mVelocity.x, -m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxSpeed, m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxSpeed);
		//rb->mVelocity.y = Clamp<float>(rb->mVelocity.y, -m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxSpeed, m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxSpeed);

		if (!isMoving)
		{
			//rb->addForce(-rb->mVelocity * 5.0f * TIME->GetDeltaTime());
			//rb->mVelocity *= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerArbitraryFriction;
			 if (isWalkingAnim)
			 {
				 //if (!mIsStunned) {
				//if (anim->IsPlaying("Felix_Walk"))
				if (m_moveAnim->IsPlaying("Action_BigMove"))
					m_moveAnim->StopAndReset();
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
			if(isOnSand)
				m_AudioManager->GetComponent<CPPSAudioManager>()->Play2DSound(m_AudioManager->GetComponent<CPPSAudioManager>()->PlayerSandStepSounds,false, 0.2f);
				//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->PlayerSandStepSounds, 0.3f);
			else
				m_AudioManager->GetComponent<CPPSAudioManager>()->Play2DSound(m_AudioManager->GetComponent<CPPSAudioManager>()->PlayerFootStepsSounds,false, 0.15f);

				//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->PlayerFootStepsSounds, 0.2f);
			//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->PlayerFootStepsSounds,0.2f);
		}
		m_stepSoundCurrent += static_cast<float>(TIME->GetDeltaTime()) * anim->m_playSpeed;
		
		/*!***********************************************************************
		\brief
		Shooting and spawnining of the golf balls
		*************************************************************************/
		//------------------Pushes balls away from the player in a circle------------------
		if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_2) || INPUT->IsKeyTriggered(KeyCode::KEY_GAMEPAD_RIGHT_BUMPER))
		{
			if (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth > 1 && !anim->IsPlaying())
			{
				anim->PlayAndReset("Felix_Attack");
			}
			hasPlayedHitSound = false;
			// Play hit sound
			//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->PlayerSlashSounds,0.3f);
			m_AudioManager->GetComponent<CPPSAudioManager>()->Play2DSound(m_AudioManager->GetComponent<CPPSAudioManager>()->PlayerSlashSounds, false, 0.3f);
			
			// Pushes the ball
			Vec2<float> current_pos = GameObj->GetComponent<CPTransform>()->GetPosition();

			// Play the swing anim
			if (m_isFacingLeft)
			{
				m_clubAnim->PlayAndReset("Felix_SwingLeft");
			}
			else
			{
				m_clubAnim->PlayAndReset("Felix_SwingRight");
			}

			DEBUG->DrawCircle(current_pos, m_shootRadius, Vec4<float>{0.f, 0.f, 0.5f, 1.0f});
			std::vector<CPCollider*> vec_colliders = COLLIDERS->OverlapCircle(current_pos, m_shootRadius);

			Vec2<float> mouse_pos;
			// GAMEPAD AIMING
			Vec2<float> const& rightJoystickPos = INPUT->GetRightJoystickPos();
			if (rightJoystickPos.x != 0.f || rightJoystickPos.y != 0.f)
			{
				mouse_pos = current_pos + rightJoystickPos * 1000.f;
			}
			else
			{
				mouse_pos = m_MouseWorld->GetComponent<CPPSMouseWorld>()->GetComponent<CPTransform>()->GetPosition();
			}

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
								//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->PlayerHitBallSounds, 0.5f);
								m_AudioManager->GetComponent<CPPSAudioManager>()->Play2DSound(m_AudioManager->GetComponent<CPPSAudioManager>()->PlayerHitBallSounds, false, 0.5f);
								hasPlayedHitSound = true;
							}
						}
					}
				}
			}
		}

		//------------------Spawn a golf ball----------------------
		if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1) || INPUT->IsKeyTriggered(KeyCode::KEY_GAMEPAD_LEFT_BUMPER))
		{

			//GOMANAGER->FindGameObjectWithName("MouseCursor")->GetComponent<CPPSMou
			if (!m_GameManager->GetComponent<CPPSGameManager>()->isInfiniteAmmo && m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentBalls >= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls) return;
			if(!m_GameManager->GetComponent<CPPSGameManager>()->isInfiniteAmmo)
			onPlacingBall.Invoke();
			
			//Spawn Game Object
			GameObject* ballObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("ball", *ballObject);
			m_AudioManager->GetComponent<CPPSAudioManager>()->Play2DSound("Ball into hole", false, 0.7f);
	/*		int Channel = AUDIOMANAGER->PlaySound("Ball into hole");
			AUDIOMANAGER->SetChannelVolume(Channel, 0.7f);*/
			Vec2<float> playerPos = GameObj->GetComponent<CPTransform>()->GetPosition();
			playerPos.x += m_isFacingLeft ? -50.0f : 50.0f;
			if(isSlippery)
				ballObject->GetComponent<CPRigidBody>()->mFriction = 0.99f;
			ballObject->GetComponent<CPTransform>()->SetPosition(playerPos);
			//ballObject->GetComponent<CPTransform>()->SetScale(Vec2<float>{0.8f, 0.8f});
			ballObject->GetComponent<CPPSPlayerGolfBall>()->SetBallUpgrade(GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->GetBallUpgrades());
		}

		/*!***********************************************************************
		\brief
		Getting direction of where the mous is and making the player face to that direction
		*************************************************************************/
		//------------------Player face mouse pos------------------
		Vec2<float> playerPos = trans->GetPosition();
		Vec2<float> mousePos = m_MouseWorld->GetComponent<CPPSMouseWorld>()->GetComponent<CPTransform>()->GetPosition();
		
		Vec2<float> playerToMouseDir = mousePos - playerPos;
		playerToMouseDir = playerToMouseDir.Normalise();

		Vec2<float> TransformRight{ 1,0 };
		m_isFacingLeft = DotProduct(playerToMouseDir, TransformRight) < 0.0f;

		// Set the club to face the mouse pos
		float angle = atan2f(playerToMouseDir.y, playerToMouseDir.x);
		m_clubTrans->SetRotation(RadToDeg(angle));

		playerToMouseDir *= m_handOffset;
		playerToMouseDir.y -= 40.0f;
		m_clubTrans->SetPosition(playerToMouseDir);

		if (m_isFacingLeft)
		{
			trans->SetScale(left_face);
			m_clubTrans->SetScale({-1.0f, -1.0f});
		} 
		else
		{
			trans->SetScale(right_face);
			m_clubTrans->SetScale(right_face);
		}

		// Update the Stunned timer
		if (mIsStunned) {
			mStunRemaining -= static_cast<float>(TIME->GetDeltaTime());
			if (mStunRemaining <= 0.f) {
				mIsStunned = false;
				mStunRemaining = mStunTimer;
			}
		}

	/*	if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
		{
			AUDIOMANAGER->Play3DSound("Ball into hole",mousePos);

		}*/
	} // End of Update

	/*!***********************************************************************
	\brief
	Fixed Update for the player for movement
	*************************************************************************/
	void CPPSPlayer::FixedUpdate()
	{
		if (m_GameManager->GetComponent<CPPSGameManager>()->isMovementDisabled)
		{
			//We reset the velocity first just incase
			rb->mVelocity = Vec2<float>(0, 0);
			return;
		}

		if (!mIsStunned) {
			Vec2<float> AddedVelocity = movement * m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed;
			rb->mVelocity += (AddedVelocity - rb->mVelocity) * 10.f * static_cast<float>(TIME->GetFixedDeltaTime());
		}

		//clamping of the speed of the player movement
		rb->mVelocity.x = Clamp<float>(rb->mVelocity.x, -m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxSpeed, m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxSpeed);
		rb->mVelocity.y = Clamp<float>(rb->mVelocity.y, -m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxSpeed, m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxSpeed);

		if (!isMoving)
		{
			//rb->addForce(-rb->mVelocity * 5.0f * TIME->GetDeltaTime());
			rb->mVelocity *= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerArbitraryFriction;
		}
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
		if (m_GameManager->GetComponent<CPPSGameManager>()->isGameOver) return;
		if (colData.colliderOther->m_gameobj->GetName() == "Projectile" ||
			colData.colliderOther->m_gameobj->GetName() == "Mage" ||
			colData.colliderOther->m_gameobj->GetName() == "EnemyChaser1" ||
			colData.colliderOther->m_gameobj->GetName() == "Charger_Shield" ||
			colData.colliderOther->m_gameobj->GetName() == "Shield" ||
			colData.colliderOther->m_gameobj->GetName() == "Bramble" )
		{
			//GODMODE!! 
			if (isGod) return;


			if (mGotAttackedCooldown > 0) return;
			//shake the cam
			if (mGotAttackedCooldown <= 0.0f) {
				Renderer::GRAPHICS->shake_camera(20.f, .1f);
				if (colData.colliderOther->m_gameobj->GetName() == "EnemyChaser1")
				{
					//AUDIOMANAGER->ChanceToPlayGroupSound(AUDIOMANAGER->ChaserAttackSounds,0.2f);
					m_AudioManager->GetComponent<CPPSAudioManager>()->Play3DSound(m_AudioManager->GetComponent<CPPSAudioManager>()->ChaserAttackSounds,
						colData.colliderOther->transform->GetPosition(),false, 0.2f);
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
			if (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth > 1)
			{
				m_AudioManager->GetComponent<CPPSAudioManager>()->Play2DSound(m_AudioManager->GetComponent<CPPSAudioManager>()->PlayerHurtSounds,false, 0.3f);
				m_AudioManager->GetComponent<CPPSAudioManager>()->Play2DSound(m_AudioManager->GetComponent<CPPSAudioManager>()->CrowdNegativeSounds, false, 0.3f);
			}
				//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->PlayerHurtSounds, 0.3f);
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

		if (colData.colliderOther->m_gameobj->GetName() == "ChaserClub")
		{
			GOMANAGER->FindGameObjectWithName("VFXManager")->GetComponent<CPPSVFXManager>()->SpawnHitAnim(trans->GetPosition());
		}
	}

	/*!***********************************************************************
	\brief
	When on collision stay with when colliding with sandpit
	*************************************************************************/
	void CPPSPlayer::OnCollisionStay(CollisionData colData)
	{
		if (colData.colliderOther->gameObj->GetName() == "Sandpit")
		{
			m_moveAnim->m_playSpeed = 0.6f;
			particle->mEmitterRate = m_particleEmitRate * 0.6f;
			//isOnSand = true;
		}
	}

	/*!***********************************************************************
	\brief
	When on collision exit, exits from stay
	*************************************************************************/
	void CPPSPlayer::OnCollisionExit(CollisionData colData)
	{
		if (colData.colliderOther->gameObj->GetName() == "Sandpit")
		{
			m_moveAnim->m_playSpeed = 1.0f;
			particle->mEmitterRate = m_particleEmitRate;
			//isOnSand = false;
		}
	}
}