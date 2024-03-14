#pragma once

#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Serialization/AssetManager.h"

namespace LB
{
	class CPPSTutorialGrandpa : public CPPBehaviour
	{
	public:
		void Start();
		void Update();
		void Destroy();

		float timerRemaining{ 45.f };

		GameObject* PlayerObj;

		GameObject* BubbleWASD;
		GameObject* BubbleMouse;
		GameObject* BubbleEnemies;
		GameObject* BubbleItems;
		GameObject* BubbleScore;
		GameObject* BubblePortal;

		GameObject* Tutorial1;
		GameObject* Tutorial2;
		GameObject* Tutorial3;
	};
	REGISTER_SCRIPT(CPPSTutorialGrandpa)
}