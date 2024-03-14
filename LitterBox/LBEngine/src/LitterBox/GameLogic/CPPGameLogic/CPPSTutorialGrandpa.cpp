#include "CPPSTutorialGrandpa.h"

namespace LB 
{
	void CPPSTutorialGrandpa::Start()
	{
		GameObj = this->GameObj;
		BubbleWASD = GOMANAGER->FindGameObjectWithName("BubbleWASD");
		BubbleMouse = GOMANAGER->FindGameObjectWithName("BubbleMouse");
		BubbleEnemies = GOMANAGER->FindGameObjectWithName("BubbleEnemies");
		BubbleItems = GOMANAGER->FindGameObjectWithName("BubbleItems");
		BubbleScore = GOMANAGER->FindGameObjectWithName("BubbleScore");
		BubblePortal = GOMANAGER->FindGameObjectWithName("BubblePortal");

		BubbleWASD->GetComponent<CPRender>()->ToggleActiveFlag(true);
		BubbleMouse->GetComponent<CPRender>()->ToggleActiveFlag(true);
		BubbleEnemies->GetComponent<CPRender>()->ToggleActiveFlag(true);
		BubbleItems->GetComponent<CPRender>()->ToggleActiveFlag(true);
		BubbleScore->GetComponent<CPRender>()->ToggleActiveFlag(true);
		BubblePortal->GetComponent<CPRender>()->ToggleActiveFlag(true);
	}

	void CPPSTutorialGrandpa::Update()
	{
		if (timerRemaining <= 25.f) 
		{
			BubbleWASD->GetComponent<CPRender>()->ToggleActiveFlag(false);
		}
		if (timerRemaining <= 20.0f)
		{
			BubbleMouse->GetComponent<CPRender>()->ToggleActiveFlag(false);
		}
		if (timerRemaining <= 15.0f)
		{
			BubbleEnemies->GetComponent<CPRender>()->ToggleActiveFlag(false);
		}
		if (timerRemaining <= 10.0f)
		{
			BubbleItems->GetComponent<CPRender>()->ToggleActiveFlag(false);
		}
		if (timerRemaining <= 5.0f)
		{
			BubbleScore->GetComponent<CPRender>()->ToggleActiveFlag(false);
		}
		
		if (timerRemaining > 0.f)
		{
			timerRemaining -= TIME->GetDeltaTime();
		}
	}

	void CPPSTutorialGrandpa::Destroy()
	{

	}

}