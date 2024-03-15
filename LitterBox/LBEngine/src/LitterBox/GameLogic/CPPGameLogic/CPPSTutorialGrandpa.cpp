#include "CPPSTutorialGrandpa.h"
#include "CPPSTutorial1.h"
#include "CPPSTutorial2.h"
#include "CPPSTutorial3.h"

namespace LB 
{
	void CPPSTutorialGrandpa::Start()
	{
		GameObj = this->GameObj;

		PlayerObj = GOMANAGER->FindGameObjectWithName("MainChar");

		BubbleWASD = GOMANAGER->FindGameObjectWithName("BubbleWASD");
		BubbleMouse = GOMANAGER->FindGameObjectWithName("BubbleMouse");
		BubbleEnemies = GOMANAGER->FindGameObjectWithName("BubbleEnemies");
		BubbleItems = GOMANAGER->FindGameObjectWithName("BubbleItems");
		BubbleScore = GOMANAGER->FindGameObjectWithName("BubbleScore");
		BubblePortal = GOMANAGER->FindGameObjectWithName("BubblePortal");

		BubbleWASD->GetComponent<CPRender>()->ToggleActiveFlag(true);
		BubbleMouse->GetComponent<CPRender>()->ToggleActiveFlag(false);
		BubbleEnemies->GetComponent<CPRender>()->ToggleActiveFlag(false);
		BubbleItems->GetComponent<CPRender>()->ToggleActiveFlag(false);
		BubbleScore->GetComponent<CPRender>()->ToggleActiveFlag(false);
		BubblePortal->GetComponent<CPRender>()->ToggleActiveFlag(false);

		Tutorial1 = GOMANAGER->FindGameObjectWithName("Tutorial1");
		Tutorial2 = GOMANAGER->FindGameObjectWithName("Tutorial2");
		Tutorial3 = GOMANAGER->FindGameObjectWithName("Tutorial3");
	}

	void CPPSTutorialGrandpa::Update()
	{
		
		if (Tutorial1->GetComponent<CPPSTutorial1>()->collided == true)
		{
			BubbleWASD->GetComponent<CPRender>()->ToggleActiveFlag(false);
			BubbleMouse->GetComponent<CPRender>()->ToggleActiveFlag(true);
			BubbleEnemies->GetComponent<CPRender>()->ToggleActiveFlag(false);
			BubbleItems->GetComponent<CPRender>()->ToggleActiveFlag(false);
			BubbleScore->GetComponent<CPRender>()->ToggleActiveFlag(false);
			BubblePortal->GetComponent<CPRender>()->ToggleActiveFlag(false);
		}
		if (Tutorial2->GetComponent<CPPSTutorial2>()->collided == true)
		{
			BubbleWASD->GetComponent<CPRender>()->ToggleActiveFlag(false);
			BubbleMouse->GetComponent<CPRender>()->ToggleActiveFlag(false);
			BubbleEnemies->GetComponent<CPRender>()->ToggleActiveFlag(true);
			BubbleItems->GetComponent<CPRender>()->ToggleActiveFlag(false);
			BubbleScore->GetComponent<CPRender>()->ToggleActiveFlag(false);
			BubblePortal->GetComponent<CPRender>()->ToggleActiveFlag(false);
		}
		if (Tutorial3->GetComponent<CPPSTutorial3>()->collided == true)
		{
			BubbleWASD->GetComponent<CPRender>()->ToggleActiveFlag(false);
			BubbleMouse->GetComponent<CPRender>()->ToggleActiveFlag(false);
			BubbleEnemies->GetComponent<CPRender>()->ToggleActiveFlag(false);
			BubbleItems->GetComponent<CPRender>()->ToggleActiveFlag(true);
			BubbleScore->GetComponent<CPRender>()->ToggleActiveFlag(false);
			BubblePortal->GetComponent<CPRender>()->ToggleActiveFlag(false);
		}
		
	}

	void CPPSTutorialGrandpa::Destroy()
	{

	}

}