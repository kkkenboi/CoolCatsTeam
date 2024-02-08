#include "CPPSUpgrade.h"
#include "CPPSPlayerGolfBall.h"
#include "CPPSUpgradeManager.h"
#include "CPPSPlayerHUD.h"
#include "CPPSPlayer.h"

namespace LB
{
	void CPPSUpgrade::Start()
	{
		//Grabbing the ball reference
		//BallGameObj = GOMANAGER->FindGameObjectWithName("ball");
		//assignedUpgradeType = (std::rand() % 3) + 1;
		
	}
	void CPPSUpgrade::Update()
	{
		if(canDestroy) GOMANAGER->RemoveGameObject(this->GameObj);
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
			//If upgrade has been hit, it tells the UpgradeManager by calling set ball upgrade		

			//CPPSUpgradeManager::Instance()->SetBallUpgrade(assignedUpgradeType);
			GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->SetBallUpgrade(assignedUpgradeType);
			switch (assignedUpgradeType)
			{
			case MOREBALL:
				GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->IncreaseMaxHealth(3);
				break;
			case MOREHEALTH:
				GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->IncreaseMaxBalls(3);
				break;
			case MOVESPEED:
				GOMANAGER->FindGameObjectWithName("MainChar")->GetComponent<CPPSPlayer>()->m_maxSpeed *= 2;
				break;
			default:
				//do nothing
				break;
			}
		/*	BallGameObj = GOMANAGER->FindGameObjectWithName("ball");
			BallGameObj->GetComponent<CPPSPlayerGolfBall>()->SetBallUpgrade(1);*/
			canDestroy = true;
			GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->HideUpgrades(assignedUpgradeType);
			/*this->GameObj->SetActive(false);*/
			//std::cout << this->GameObj->GetName() << '\n';
			//GameObj->GetComponent<CPTransform>()->SetPosition({ 10000.f,10000.f });
			//GOMANAGER->RemoveGameObject(this->GameObj);
		}
	}

	//Gets called by the upgrade manager, to assign the current type of upgrade to the ball
	void CPPSUpgrade::AssignUpgradeID(int upgradeType)
	{
		//Called by the upgrade 
		assignedUpgradeType = upgradeType;
	}
}