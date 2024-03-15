#include "CPPSPortal.h"
#include "CPPGameManager.h"
#include "CPPSPlayer.h"
#include "LitterBox/Scene/SceneManager.h"

void LB::CPPSPortal::Start()
{
	mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
	mGameManager = GOMANAGER->FindGameObjectWithName("GameManager");
	mPortalCenter = GOMANAGER->FindGameObjectWithName("PortalCenter");
}

void LB::CPPSPortal::Update()
{
	//we have to check this because CPPBehaviour runs things even though in active (for now)
	if (GameObj->IsActive())
	{
		rotAngle += static_cast<float>(TIME->GetDeltaTime()) * 100.f;
		GetComponent<CPTransform>()->SetRotation(rotAngle);
		GetComponent<CPTransform>()->GetChild(0)->gameObj->SetActive(true);
		GetComponent<CPTransform>()->GetChild(1)->gameObj->SetActive(true);
	}
	else
	{
		GetComponent<CPTransform>()->GetChild(0)->gameObj->SetActive(false);
		GetComponent<CPTransform>()->GetChild(1)->gameObj->SetActive(false);
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
			mPlayer->GetComponent<CPTransform>()->SetRotation(rotTimer * rotAnglePerSec);
			//Then we scale him down till he's tiny
			Vec2<float> lerpedScale = Lerp(Vec2<float>(1, 1), Vec2<float>(0, 0), rotTimer / 2.f);
			mPlayer->GetComponent<CPTransform>()->SetScale(lerpedScale);

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

				if (circleTimer >= 1.f)
				{	//Once the 1 second time has been reached, it should be max size
					circleTimer = 1;
					//THIS IS WHERE YOU PUT THE INTERMISSION STUFF!!!
					intermissionTimer += static_cast<float>(TIME->GetDeltaTime());
					if (intermissionTimer >= intermissionDuration)
					{
						expandOut = false;
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
				Vec2<float> portalScale = Lerp(Vec2<float>(1, 1), Vec2<float>(100, 100), circleTimer);
				mPortalCenter->GetComponent<CPTransform>()->SetScale(portalScale);
			}
		}		
	}
	else
	{
		timer = 0;
		rotTimer = 0;
		circleTimer = 0;
		intermissionTimer = 0;
		expandOut = true;
		playPortalSound = false;
	}

	//This handles what happens when the animations are all done, the circle has expanded and shrunk.
	//The player and camera will be teleported, and only after that animation is done does the wave start.
	if (finishTransition)
	{
		if (!temp)//temp fix so that it's only called once
		{//will not be needed once cols have their setactive done
			temp = true;

	
				this->GameObj->SetActive(false);
				//Next wave just handles starting the wave... 
				mGameManager->GetComponent<CPPSGameManager>()->NextWave();
				mPlayer->GetComponent<CPTransform>()->SetScale(Vec2<float>(1, 1));
				mPlayer->GetComponent<CPTransform>()->SetRotation(0);
		}
		finishTransition = false;
		//spawn the big black circle
	}
	if (!finishTransition) temp = false;
}

void LB::CPPSPortal::Destroy()
{
}

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
