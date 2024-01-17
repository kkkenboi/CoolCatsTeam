#include "CPPSPlayerHUD.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Components/CPPScriptComponent.h"
#include "LitterBox/GameLogic/CPPGameLogic/CPPSPlayer.h"

namespace LB {

	Vec2 heartDisplayStartingPos   { 126.f, 960.f };
	Vec2 ballDisplayStartingPos    { 126.f, 830.f };
	Vec2 upgradeDisplayStartingPos { 1150.f, 110.f };

	//Vec2 heartDisplayScale  { 0.3f, 0.3f };
	//Vec2 ballDisplayScale   { 0.3f, 0.3f };
	//Vec2 upgradeDisplayScale{ 0.3f, 0.3f };
	Vec2 displayScale{ 0.3f, 0.3f };

	//Vec2 heartDisplayOffset{ 130.f, 130.f };
	//Vec2 ballDisplayOffset{ 130.f, 130.f };
	//Vec2 upgradeDisplayOffset{ 130.f, 130.f };
	Vec2 displayOffset { 130.f, 130.f };

	void CPPSPlayerHUD::Start()
	{
		// Create no. of hearts and balls based on the player's info
		for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		{
			// Find out which object is the player
			if (gameObj->GetName() == "MainChar")
			{
				mainChar = gameObj;
			}

			mainChar->GetComponent<CPScriptCPP>()->GetInstance();

			//else if (gameObj->GetName() == "BallDisplay")
			//{
			//	m_BallsDisplay = gameObj;
			//}

			//else if (gameObj->GetName() == "UpgradeDisplay")
			//{
			//	m_UpgradeDisplay = gameObj;
			//}
		}
		
		//// Setting positions and scale for the different objects
		//m_HeartDisplay->GetComponent<CPTransform>()->SetPosition();
		//m_HeartDisplay->GetComponent<CPTransform>()->SetScale();
		//m_BallsDisplay->GetComponent<CPTransform>()->SetPosition();
		//m_BallsDisplay->GetComponent<CPTransform>()->SetScale();
		//m_UpgradeDisplay->GetComponent<CPTransform>()->SetPosition();
		//m_UpgradeDisplay->GetComponent<CPTransform>()->SetScale();
	}

	void CPPSPlayerHUD::Update()
	{

	}

	void CPPSPlayerHUD::Destroy()
	{

	}
}