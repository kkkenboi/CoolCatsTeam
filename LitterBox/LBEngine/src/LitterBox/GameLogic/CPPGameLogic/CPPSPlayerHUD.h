#pragma once

#include "pch.h"
#include "CPPBehaviour.h"
#include "LitterBox/Components/CPPScriptComponent.h"


namespace LB
{
	class CPPSPlayerHUD : public CPPBehaviour
	{
	public:
		void Start() override;
		void Update() override;
		void Destroy() override;

		void DecreaseHealth();
		void DecreaseBalls();
		void IncreaseHealth();
		void IncreaseBalls();
		//// Assuming that balls are max every time the player enters a new level
		//void InitialiseHUD(int maxHearts, int currentHearts, int maxBalls);

	private:
		int m_currentHealth;
		int m_maxHealth;
		int m_currentBalls;
		int m_maxBalls;
		bool m_displayUpgrade;

		GameObject* mainChar;

		std::vector<GameObject*> m_TotalHeartDisplay;
		std::vector<GameObject*> m_TotalBallsDisplay;
		std::stack<GameObject*> m_TotalUpgradeDisplay;
	};
	REGISTER_SCRIPT(CPPSPlayerHUD)

	void DecreaseHealth();
	void DecreaseBalls();
	void IncreaseHealth();
	void IncreaseBalls();
}

