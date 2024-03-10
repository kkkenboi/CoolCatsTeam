#include "CPPSPortal.h"
#include "CPPGameManager.h"
#include "CPPSPlayer.h"

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
		rotAngle += TIME->GetDeltaTime() * 100;
		GetComponent<CPTransform>()->SetRotation(rotAngle);
	}
	//If the transition is happening, we want to increment all our timers and do the transition
	if (isTransitioning)
	{
		timer += TIME->GetDeltaTime();
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
			rotTimer += TIME->GetDeltaTime();
			//First we rotate the player
			mPlayer->GetComponent<CPTransform>()->SetRotation(rotTimer * rotAnglePerSec);
			//Then we scale him down till he's tiny
			Vec2<float> lerpedScale = Lerp(Vec2<float>(1, 1), Vec2<float>(0, 0), rotTimer / 2.f);
			mPlayer->GetComponent<CPTransform>()->SetScale(lerpedScale);

			//Once the player is tiny as all hell, we expand the portal center to fill the screen
			if (rotTimer >= 2.f)
			{
				mPortalCenter->SetActive(true);
				//At the start the expandout bool is true so the circle expands
				if(expandOut) circleTimer += TIME->GetDeltaTime();
				else circleTimer -= TIME->GetDeltaTime();
				if (circleTimer >= 1.f)
				{	//Once the 1 second time has been reached, it should be max size
					expandOut = false;
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
					mPlayer->GetComponent<CPTransform>()->SetScale(Vec2<float>(1, 1));
					mPlayer->GetComponent<CPTransform>()->SetRotation(0);
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
	}

	//This handles what happens when the animations are all done, the circle has expanded and shrunk.
	//The player and camera will be teleported, and only after that animation is done does the wave start.
	if (finishTransition)
	{
		if (!temp)//temp fix so that it's only called once
		{//will not be needed once cols have their setactive done
			this->GameObj->SetActive(false);
			//Next wave just handles starting the wave... 
			mGameManager->GetComponent<CPPSGameManager>()->NextWave();
		
			temp = true;
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
