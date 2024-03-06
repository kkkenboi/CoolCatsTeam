#include "CPPSPortal.h"
#include "CPPGameManager.h"
#include "CPPSPlayer.h"

void LB::CPPSPortal::Start()
{
	mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
	mGameManager = GOMANAGER->FindGameObjectWithName("GameManager");
}

void LB::CPPSPortal::Update()
{
	if (isTransitioning)
	{
		timer += TIME->GetDeltaTime();
		//Lerp the player to that pos
		if (timer <= 1.f) //I think the lerp is implemented a bit wrongly
		{
		Vec2<float> lerpedPos = Lerp(mCachedPlayerPos, GetComponent<CPTransform>()->GetPosition(), timer);
		mPlayer->GetComponent<CPTransform>()->SetPosition(lerpedPos);

		}
		if (timer >= 1.f) //this means player should be in centre now so we can spin and scale now
		{
			//By right I think we couldddd probably use an anim state for this but oh well
			rotTimer += TIME->GetDeltaTime();
			mPlayer->GetComponent<CPTransform>()->SetRotation(rotTimer * rotAnglePerSec);
			Vec2<float> lerpedScale = Lerp(Vec2<float>(1, 1), Vec2<float>(0, 0), rotTimer / 2.f);
			mPlayer->GetComponent<CPTransform>()->SetScale(lerpedScale);
			if (rotTimer >= 2.f)
			{
				isTransitioning = false;
				finishTransition = true;
			}
		}
		
	}
	else
	{
		timer = 0;
		rotTimer = 0;
	}
	if (finishTransition)
	{
		if (!temp)//temp fix so that it's only called once
		{//will not be needed once cols have their setactive done
		this->GameObj->SetActive(false);
		std::cout << "Spawn next wave!\n";
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

	}
}
