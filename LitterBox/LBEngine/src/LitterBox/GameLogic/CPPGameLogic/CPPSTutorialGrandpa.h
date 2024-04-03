#pragma once
/*!************************************************************************
 \file                CPPSTutorialGrandpa.h
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

#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Serialization/AssetManager.h"

namespace LB
{
	class CPPSTutorialGrandpa : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		 CPPBehaviour base class virtual function override for start
		*************************************************************************/
		void Start();
		/*!***********************************************************************
		\brief
		 CPPBehaviour base class virtual function override for Update
		*************************************************************************/
		void Update();
		/*!***********************************************************************
		\brief
		 CPPBehaviour base class virtual function override for Destroy
		*************************************************************************/
		void Destroy();

		float timerRemaining{ 45.f };

		GameObject* PlayerObj;

		GameObject* BubbleWASD;
		GameObject* BubbleMouse;
		GameObject* BubbleEnemies;
		GameObject* BubbleItems;
		GameObject* BubbleScore;
		GameObject* BubblePortal;
		GameObject* BubbleSandpit;
		GameObject* BubbleObstacles;

		GameObject* Tutorial1;
		GameObject* Tutorial2;
		GameObject* Tutorial3;
		GameObject* Tutorial4;
		GameObject* Tutorial5;
	};
	REGISTER_SCRIPT(CPPSTutorialGrandpa)
}