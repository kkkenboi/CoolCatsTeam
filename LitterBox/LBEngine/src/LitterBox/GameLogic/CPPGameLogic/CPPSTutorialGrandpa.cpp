/*!************************************************************************
 \file                CPPSTutorialGrandpa.cpp
 \author(s)           Justine Carlo Villa Ilao
 \par DP email(s):    justine.c@digipen.edu
 \par Course:         CSD2401A
 \date                15-03-2024
 \brief

	This file contains the Tutorial scene's grandpa

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSTutorialGrandpa.h"
#include "CPPSTutorial1.h"
#include "CPPSTutorial2.h"
#include "CPPSTutorial3.h"

namespace LB 
{
	/*!***********************************************************************
	\brief
	 CPPBehaviour base class virtual function override for start
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	 CPPBehaviour base class virtual function override for Update
	*************************************************************************/
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
	/*!***********************************************************************
	\brief
	 CPPBehaviour base class virtual function override for Destroy
	*************************************************************************/
	void CPPSTutorialGrandpa::Destroy()
	{

	}

}