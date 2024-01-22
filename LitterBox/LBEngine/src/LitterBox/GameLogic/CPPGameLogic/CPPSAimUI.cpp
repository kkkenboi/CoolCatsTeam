#include "CPPSAimUI.h"

namespace LB
{
	REGISTER_SCRIPT(CPPSAimUI);
	void CPPSAimUI::Start()
	{
		//Grabbing the current obj's transform
		mTransform = GameObj->GetComponent<CPTransform>();
		//Grabbing the player reference
		std::vector<GameObject*> const& GOs = GOMANAGER->GetGameObjects();
		for (GameObject* GO : GOs) {
			if (GO->GetName() == "MainChar") {
				mPlayer = GO;
				break;
			}
		}
	}

	void CPPSAimUI::Update()
	{
		//Setting the position of the aim script to the player
		mTransform->SetPosition(mPlayer->GetComponent<CPTransform>()->GetPosition());

		MousePos = INPUT->GetMousePos();
		MousePos.y = MousePos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();
		MousePos.y *= 1080.f / (float)WINDOWSSYSTEM->GetHeight();
		MousePos.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();
		//Getting the direction from player to mouse
		DirToMouse = MousePos - mPlayer->GetComponent<CPTransform>()->GetPosition();
		//DirToMouse.Normalise();	//normalise for nicer calculations
		//Set the rotation of the AimUI to the direction
		mTransform->SetRotation(RadToDeg(atan2f(DirToMouse.y, DirToMouse.x)));

	}

	void CPPSAimUI::Destroy()
	{
		//Should be empty
	}

}
