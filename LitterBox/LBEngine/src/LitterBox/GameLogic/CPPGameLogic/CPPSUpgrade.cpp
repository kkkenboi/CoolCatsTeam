#include "CPPSUpgrade.h"
#include "CPPSPlayerGolfBall.h"
#include "CPPSUpgradeManager.h"
namespace LB
{
	void CPPSUpgrade::Start()
	{
		//Grabbing the ball reference
		//BallGameObj = GOMANAGER->FindGameObjectWithName("ball");

	}
	void CPPSUpgrade::Update()
	{
	}
	void CPPSUpgrade::Destroy()
	{
	}

	void CPPSUpgrade::OnCollisionEnter(CollisionData colData)
	{
		//make collision layer that only contains ball and upgrade interaction
		//If the upgrade collides with a ball
		if (colData.colliderOther->m_gameobj->GetName() == "ball")
		{
			std::cout << "Hit upgrade\n";

			CPPSUpgradeManager::Instance()->SetBallUpgrade(assignedUpgradeType);
		/*	BallGameObj = GOMANAGER->FindGameObjectWithName("ball");
			BallGameObj->GetComponent<CPPSPlayerGolfBall>()->SetBallUpgrade(1);*/
			this->GameObj->SetActive(false);
		}
	}
	void CPPSUpgrade::AssignUpgradeID(int upgradeType)
	{
		//Called by the upgrade 
		assignedUpgradeType = upgradeType;
	}
}