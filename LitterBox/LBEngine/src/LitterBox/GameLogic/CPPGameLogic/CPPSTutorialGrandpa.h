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

		float timerRemaining{ 30.f };

		GameObject* BubbleWASD;
		GameObject* BubbleMouse;
		GameObject* BubbleEnemies;
		GameObject* BubbleItems;
		GameObject* BubbleScore;
		GameObject* BubblePortal;
	};
	REGISTER_SCRIPT(CPPSTutorialGrandpa)
}