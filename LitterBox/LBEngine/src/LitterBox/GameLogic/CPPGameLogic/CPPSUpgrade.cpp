#include "CPPSUpgrade.h"
#include "CPPSPlayerGolfBall.h"
#include "CPPSUpgradeManager.h"
#include "CPPSPlayerHUD.h"

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
			if (assignedUpgradeType == MOREHEALTH) {
				GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->IncreaseMaxHealth(3);
			}if (assignedUpgradeType == MOREBALL) {

				GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->IncreaseMaxBalls(3);
			}
		/*	BallGameObj = GOMANAGER->FindGameObjectWithName("ball");
			BallGameObj->GetComponent<CPPSPlayerGolfBall>()->SetBallUpgrade(1);*/
			
			/*this->GameObj->SetActive(false);*/
			//std::cout << this->GameObj->GetName() << '\n';
			GameObj->GetComponent<CPTransform>()->SetPosition({ 10000.f,10000.f });
			//GOMANAGER->RemoveGameObject(this->GameObj);
		}
	}
	void CPPSUpgrade::AssignUpgradeID(int upgradeType)
	{
		//Called by the upgrade 
		assignedUpgradeType = upgradeType;
	}
}