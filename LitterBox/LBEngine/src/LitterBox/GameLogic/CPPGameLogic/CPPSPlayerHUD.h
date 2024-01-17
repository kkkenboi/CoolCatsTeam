#pragma once

#include "pch.h"
#include "CPPBehaviour.h"

namespace LB
{
	class CPPSPlayerHUD : public CPPBehaviour
	{
	public:
		void Start() override;
		void Update() override;
		void Destroy() override;

		//// Assuming that balls are max every time the player enters a new level
		//void InitialiseHUD(int maxHearts, int currentHearts, int maxBalls);

	private:
		bool displayUpgrade;

		GameObject* mainChar;

		std::stack<GameObject*> m_TotalHeartDisplay;
		std::stack<GameObject*> m_TotalBallsDisplay;
		std::stack<GameObject*> m_TotalUpgradeDisplay;
	};
}

