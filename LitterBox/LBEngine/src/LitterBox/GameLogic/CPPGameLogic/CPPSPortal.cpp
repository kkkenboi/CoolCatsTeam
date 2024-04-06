/*!************************************************************************
 * \file				CPPPortal.cpp
 * \author(s)			Amadeus Chia
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2451A
 * \date				09/02/2024
 * \brief 				This header file contains the definitions for the
 * 						portal
 of the game
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "CPPSPortal.h"
#include "CPPGameManager.h"
#include "CPPSPlayer.h"
#include "LitterBox/Scene/SceneManager.h"
#include "LitterBox/Renderer/Renderer.h"
#include "LitterBox/Renderer/ForwardDeclerators.h"

/*!************************************************************************
* \brief
* Start function for the portal script
**************************************************************************/
void LB::CPPSPortal::Start()
{
	mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
	mGameManager = GOMANAGER->FindGameObjectWithName("GameManager");
	mPortalCenter = GOMANAGER->FindGameObjectWithName("PortalCenter");
	mLevelBoard = GOMANAGER->FindGameObjectWithName("LevelBoard");
	mLevelBoard->SetActive(false);

	// Particle Emitter for the Portal
	/*
	mPortalEmitter = FACTORY->SpawnGameObject();
	GameObject* portalblackcircle = GOMANAGER->FindGameObjectWithName("blackcircle");
	mPortalEmitter->GetComponent<CPTransform>()->SetParent(portalblackcircle->GetComponent<CPTransform>());
	mPortalEmitter->GetComponent<CPTransform>()->SetPosition(Vec2<float>{0.f,0.f});
	mPortalEmitter->AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
	mPortalEmitter->GetComponent<CPRender>()->Initialise();
	
	if (mPortalEmitter->GetComponent<CPRender>()->spriteIndex < 0)
	{	//We have to do the standard grabbing of texture from the asset manager stuff
		int textureID = mPortalEmitter->GetComponent<CPRender>()->texture;
		std::string textureName = ASSETMANAGER->GetTextureName(textureID);
		mPortalEmitter->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second, static_cast<int>(mPortalEmitter->GetComponent<CPRender>()->w), static_cast<int>(mPortalEmitter->GetComponent<CPRender>()->h));
	}
	else
	{	//if we do happen to have a sprite index, this means we're using the sprite sheet so
		//we just use the CPRender function to set sprite texture
		mPortalEmitter->GetComponent<CPRender>()->SetSpriteTexture(mPortalEmitter->GetComponent<CPRender>()->spriteSheetName, mPortalEmitter->GetComponent<CPRender>()->spriteIndex);
	}
	
	mPortalEmitter->GetComponent<CPRender>()->SetSpriteTexture("VFXSheet", 0);
	*/

	mPortalEmitter = GOMANAGER->FindGameObjectWithName("portalemitter");
	mPortalEmitter->GetComponent<CPParticle>()->mEmitterType = INVERSERADIAL;
	mPortalEmitter->GetComponent<CPParticle>()->mEmitterRate = 0.03f;

	mPortalEmitter->GetComponent<CPParticle>()->mInvRadDistanceMin = 125.f;
	mPortalEmitter->GetComponent<CPParticle>()->mInvRadDistanceMax = 350.f;

	mPortalEmitter->GetComponent<CPParticle>()->mRotationSpeedVariationMin = -100.f;
	mPortalEmitter->GetComponent<CPParticle>()->mRotationSpeedVariationMax = 100.f;

	mPortalEmitter->GetComponent<CPParticle>()->mEmitterSizeBegin = 1.25f;
	mPortalEmitter->GetComponent<CPParticle>()->mEmitterSizeEnd = 0.0f;

	mPortalEmitter->GetComponent<CPParticle>()->mParticleLifetime = 0.5f;
	mPortalEmitter->GetComponent<CPParticle>()->mParticleLifetimeRandomness = true;
	mPortalEmitter->GetComponent<CPParticle>()->mParticleLifetimeMin = 0.0f;
	mPortalEmitter->GetComponent<CPParticle>()->mParticleLifetimeMax = 1.5;


	mPortalEmitter->GetComponent<CPParticle>()->mIsActive = false;
	mPortalEmitter->GetComponent<CPParticle>()->mIsLooping = true;

	/*
	// Spawn fireworks
	mFireworksEmitter1 = FACTORY->SpawnGameObject();
	mFireworksEmitter1->GetComponent<CPTransform>()->SetPosition(Vec2<float>{500.f, 900.f});
	mFireworksEmitter1->AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
	mFireworksEmitter1->GetComponent<CPRender>()->Initialise();
	Renderer::GRAPHICS->swap_object_type(Renderer::Renderer_Types::RT_UI, mFireworksEmitter1->GetComponent<CPRender>());
	mFireworksEmitter1->SetName("JOEBIDEN");

	// Hard-coded texture index in sprite
	mFireworksEmitter1->GetComponent<CPRender>()->SetSpriteTexture("VFXSheet", 49);
	*/
}

/*!************************************************************************
* \brief
* Update function for the portal script
**************************************************************************/
void LB::CPPSPortal::Update()
{
	//we have to check this because CPPBehaviour runs things even though in active (for now)
	if (GameObj->IsActive())
	{
		rotAngle += static_cast<float>(TIME->GetDeltaTime()) * 100.f;
		GetComponent<CPTransform>()->SetRotation(rotAngle);
		GetComponent<CPTransform>()->GetChild(0)->gameObj->SetActive(true);
		GetComponent<CPTransform>()->GetChild(1)->gameObj->SetActive(true);
		mPortalEmitter->GetComponent<CPParticle>()->mIsActive = true;
	}
	else
	{
		GetComponent<CPTransform>()->GetChild(0)->gameObj->SetActive(false);
		GetComponent<CPTransform>()->GetChild(1)->gameObj->SetActive(false);
		mPortalEmitter->GetComponent<CPParticle>()->mIsActive = false;
	}
	//If the transition is happening, we want to increment all our timers and do the transition
	if (isTransitioning)
	{
		timer += static_cast<float>(TIME->GetDeltaTime());
		//Lerp the player to that pos
		if ((timer/0.75f) <= 1.f) //I think the lerp is implemented a bit wrongly
		{
			Vec2<float> lerpedPos = Lerp(mCachedPlayerPos, GetComponent<CPTransform>()->GetPosition(), timer/0.75f);
			mPlayer->GetComponent<CPTransform>()->SetPosition(lerpedPos);
		}

		//This handles the spinning "animation" of the player into the centre of the black hole
		if (timer >= 1.f) //this means player should be in centre now so we can spin and scale now
		{
			//By right I think we couldddd probably use an anim state for this but oh well
			rotTimer += static_cast<float>(TIME->GetDeltaTime());
			//First we rotate the player
			if (expandOut && circleTimer < 1.5f)
			{
				mPlayer->GetComponent<CPTransform>()->SetRotation(rotTimer * rotAnglePerSec);
				//Then we scale him down till he's tiny
				Vec2<float> lerpedScale = Lerp(Vec2<float>(1, 1), Vec2<float>(0, 0), rotTimer / 2.f);
				mPlayer->GetComponent<CPTransform>()->SetScale(lerpedScale);
			}

			if (!playPortalSound)
			{
				AUDIOMANAGER->PlaySound("Sucked In");
				playPortalSound = true;
			}
			//Once the player is tiny as all hell, we expand the portal center to fill the screen
			if (rotTimer >= 2.f)
			{
				mPortalCenter->SetActive(true);
		
				//At the start the expandout bool is true so the circle expands
				if(expandOut) circleTimer += static_cast<float>(TIME->GetDeltaTime());
				else circleTimer -= static_cast<float>(TIME->GetDeltaTime());

				if (circleTimer >= 1.5f)
				{	//Once the 1 second time has been reached, it should be max size
					circleTimer = 1.5f;
					//THIS IS WHERE YOU PUT THE INTERMISSION STUFF!!!
					intermissionTimer += static_cast<float>(TIME->GetDeltaTime());

					mLevelBoard->SetActive(true);

					// Plays the starting animation for the level transition
					if (playStartAnim)
					{
						mLevelBoard->SetActive(true);
						mLevelBoard->GetComponent<CPTransform>()->GetChild(0)->GetComponent<CPAnimator>()->Play("VFX_ExpandH");

						// Concatenate with the hole number
						std::stringstream holeNumber;
						char finalBuffer[256]{};
						holeNumber << mGameManager->GetComponent<CPPSGameManager>()->GetCurrentWave() + 1;
						std::string buffer = "Hole " + holeNumber.str();
						strcpy_s(finalBuffer, sizeof(finalBuffer), buffer.c_str());

						mLevelBoard->GetComponent<CPTransform>()->GetChild(1)->GetComponent<CPText>()->set_msg(finalBuffer);
						mLevelBoard->GetComponent<CPTransform>()->GetChild(2)->GetComponent<CPAnimator>()->Play("Flag_Appear");
						mLevelBoard->GetComponent<CPTransform>()->GetChild(2)->GetComponent<CPAnimator>()->PlayNext("Flag_Swaying");
						mLevelBoard->GetComponent<CPTransform>()->GetChild(2)->GetComponent<CPAnimator>()->PlayDelay("Flag_Disappear", 0.8f);
						playStartAnim = false;

						// Clear current map
						mGameManager->GetComponent<CPPSGameManager>()->DeleteOldMap();
					}

					if (intermissionTimer >= intermissionDuration)
					{
						expandOut = false;
						// Load the next level in the background
						mGameManager->GetComponent<CPPSGameManager>()->StartNewMap();
						mGameManager->GetComponent<CPPSGameManager>()->onNewMapStart.Invoke();

						mGameManager->GetComponent<CPPSGameManager>()->onNextLevel.Invoke();

						// Plays the ending animation for the level transition
						if (playEndAnim)
						{
							DebuggerLog("Playing end animation!");

							mLevelBoard->GetComponent<CPTransform>()->GetChild(0)->GetComponent<CPAnimator>()->Play("VFX_ExpandHReverse");

							// Reset the player scale and rotation
							mPlayer->GetComponent<CPTransform>()->SetScale(Vec2<float>(1, 1));
							mPlayer->GetComponent<CPTransform>()->SetRotation(0);

							playEndAnim = false;
						}

						DebuggerLogFormat("LEVEL %d COMPLETE! Now loading LEVEL %d!", mGameManager->GetComponent<CPPSGameManager>()->GetCurrentWave(),
							mGameManager->GetComponent<CPPSGameManager>()->GetCurrentWave() + 1);
					}
					if (mGameManager->GetComponent<CPPSGameManager>()->GetCurrentWave() == 0)
					{
						SCENEMANAGER->LoadScene("SceneMain");
					}
					//Once the player reaches the end of the 9th wave, we want to play the video
					else if (mGameManager->GetComponent<CPPSGameManager>()->GetCurrentWave() == 9)
					{
						mGameManager->GetComponent<CPPSGameManager>()->ShowGameWin();
					}
				}
				else if (circleTimer <= 0.f)
				{	//then it'll start shrinking, and at 0s, it means it's done and we can start the wave
					isTransitioning = false;
					mPortalCenter->SetActive(false);
					finishTransition = true;
				}

				//If the circle is shrinking and we assume the screen is already covered, we tp the player and camera
				if (!expandOut && circleTimer >= 0.8f)
				{
					mPlayer->GetComponent<CPTransform>()->SetPosition(mGameManager->GetComponent<CPPSGameManager>()->playerSpawnPoint);
					GOMANAGER->FindGameObjectWithName("CameraFollow")->GetComponent<CPTransform>()->SetPosition(mGameManager->GetComponent<CPPSGameManager>()->playerSpawnPoint);
				}

				//We lerp the scale, it follows the timer so it either shrinks or expands
				Vec2<float> portalScale = Lerp(Vec2<float>(1, 1), Vec2<float>(200, 200), circleTimer/1.5f);
				mPortalCenter->GetComponent<CPTransform>()->SetScale(portalScale);

				// Set text size to backboard's increasing/decreasing scale DO NOT CHANGE THESE VALUES PLEASE CONSULT ME
				endAnimDelay += static_cast<float>(TIME->GetDeltaTime());
				if (endAnimDelay > 3.43f)
				{
					mLevelBoard->GetComponent<CPTransform>()->GetChild(1)->GetComponent<CPText>()->update_msg_size(0.f);
				}
				else
				{
					float xPos = mLevelBoard->GetComponent<CPTransform>()->GetChild(1)->GetComponent<CPText>()->get_msg().xbound / 2.0f;
					Vec2<float> newPos{ xPos - xPos * mLevelBoard->GetComponent<CPTransform>()->GetChild(0)->GetComponent<CPTransform>()->GetScale().x / 2.0f - 220.f, -20.f };

					mLevelBoard->GetComponent<CPTransform>()->GetChild(1)->SetPosition(newPos);
					mLevelBoard->GetComponent<CPTransform>()->GetChild(1)->GetComponent<CPText>()->update_msg_size(mLevelBoard->GetComponent<CPTransform>()->GetChild(0)->GetComponent<CPTransform>()->GetScale().x);
				}
			}
		}		
	}
	else
	{
		timer = 0.f;
		endAnimDelay = 0.f;
		rotTimer = 0.f;
		circleTimer = 0.f;
		intermissionTimer = 0.f;
		mLevelBoard->GetComponent<CPTransform>()->GetChild(2)->GetComponent<CPAnimator>()->m_awakeDelay = 0.f;
		expandOut = true;
		playPortalSound = false;
		playStartAnim = true;
		playEndAnim = true;
	}

	//This handles what happens when the animations are all done, the circle has expanded and shrunk.
	//The camera will be teleported, and only after that animation is done does the wave start.
	if (finishTransition)
	{
		if (!temp)//temp fix so that it's only called once
		{//will not be needed once cols have their setactive done
			temp = true;

	
				this->GameObj->SetActive(false);
				//Next wave just handles starting the wave... 
				mGameManager->GetComponent<CPPSGameManager>()->NextWave();
		}
		finishTransition = false;
		//spawn the big black circle
	}
	if (!finishTransition) temp = false;
}

/*!************************************************************************
* \brief
* Destroy function for the portal script
**************************************************************************/
void LB::CPPSPortal::Destroy()
{
}
/*!************************************************************************
* \brief
* OnCollisioEnter function for the portal script
**************************************************************************/
void LB::CPPSPortal::OnCollisionEnter(CollisionData colData)
{	//Once the player touches the portal, we want to start the transition
	if (colData.colliderOther->gameObj->GetName() == "MainChar" && !isTransitioning)
	{
		isTransitioning = true;
		mCachedPlayerPos = mPlayer->GetComponent<CPTransform>()->GetPosition();
		//Disable players movement
		GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->isMovementDisabled = true;
	}
}
