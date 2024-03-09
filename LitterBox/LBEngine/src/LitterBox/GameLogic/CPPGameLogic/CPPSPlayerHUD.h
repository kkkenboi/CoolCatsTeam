/*!************************************************************************
 \file				CPPSPlayerHUD.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:		CSD2451A
 \date				09-02-2024
 \brief
 This file contains the CPPSPlayerHUD class and all its functionalities.

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "pch.h"
#include "CPPBehaviour.h"
#include "LitterBox/Components/CPPScriptComponent.h"
#include "CPPSUpgradeManager.h"
#include <tuple>


namespace LB
{
	/*!***********************************************************************
	\brief
	CPPSPlayerHUD displays various HUD objects onto the screen to let the player
	know it's stats, such as health, balls and possibly upgrades in the future.
	*************************************************************************/
	class CPPSPlayerHUD : public CPPBehaviour
	{
	public:

		/*!***********************************************************************
		\brief
		Start function to initialise the heart and ball display variables based on
		the current player's variables.
		*************************************************************************/
		void Start() override;

		/*!***********************************************************************
		\brief
		Update will reference the player and update the HUD if there are any changes
		such as the health and the balls.
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		\brief
		Inherited Destroy currently doesn't need to do anything.
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		 \brief
		 Decreases the member variable for current health in the PlayerHUD
		*************************************************************************/
		void DecreaseHealth();

		/*!***********************************************************************
		 \brief
		 Decreases the member variable for current balls in the PlayerHUD
		*************************************************************************/
		void DecreaseBalls();

		/*!***********************************************************************
		 \brief
		 Increases the member variable for current health in the PlayerHUD
		*************************************************************************/
		void IncreaseHealth();

		/*!***********************************************************************
		 \brief
		 Increases the member variable for current balls in the PlayerHUD
		*************************************************************************/
		void IncreaseBalls();

		/*!***********************************************************************
		 \brief
		 Increases the member variable for max health in the PlayerHUD
		*************************************************************************/
		void IncreaseMaxHealth(int amount);

		/*!***********************************************************************
		 \brief
		 Increases the member variable for max balls in the PlayerHUD
		*************************************************************************/
		void IncreaseMaxBalls(int amount);

		/*!***********************************************************************
		 \brief
		 Adds a new upgrade to the display in PlayerHUD
		*************************************************************************/
		void AddNewUpgrade(UpgradeType upgrade);

		bool m_mouseHoverUI{ false };
		bool m_mouseHoverWorld{ false };
		GameObject* m_UpgradeObject{ nullptr };

		UpgradeType m_currentPopUpIndex{ UpgradeType::SPLIT };
		std::vector<std::pair<GameObject*, GameObject*>> m_totalUpgradePopUps;
	private:
		bool m_displayUpgrade{false};
		bool m_decreaseHealth{ false };
		bool m_decreaseBalls{ false };
		Vec2<float> m_heartDisplayOffset = { 110.f, 110.f };
		Vec2<float> m_ballDisplayOffset = { 68.f, 68.f };
		Vec2<float> m_upgradeDisplayOffset = { 106.f, 106.f };

		GameObject* m_GameManager{ nullptr };
		GameObject* m_UpgradeManager{ nullptr };
		GameObject* m_Mouse{ nullptr };
		GameObject* m_CameraFollow{ nullptr };


		std::vector<GameObject*> m_totalHeartDisplay;
		std::vector<GameObject*> m_totalBallsDisplay;
		std::vector<GameObject*> m_totalUpgradeDisplay;
	};
	REGISTER_SCRIPT(CPPSPlayerHUD)

	/*!***********************************************************************
	 \brief
	 For event subscription to decrease health when the player takes damage
	*************************************************************************/
	void DecreaseHealth();
	/*!***********************************************************************
	 \brief
	 For event subscription to decrease ball when the player places one
	*************************************************************************/
	void DecreaseBalls();

	/*!***********************************************************************
	 \brief
	 For event subscription to increase health when the player regens health
	*************************************************************************/
	void IncreaseHealth();

	/*!***********************************************************************
	 \brief
	 For event subscription to increase ball count when the ball expires
	*************************************************************************/
	void IncreaseBalls();

	/*!***********************************************************************
	 \brief
	 For event subscription to increase the max balls in PlayerHUD
	*************************************************************************/
	void IncreaseMaxHealth(int amount);

	/*!***********************************************************************
	 \brief
	 For event subscription to increase the max health in PlayerHUD
	*************************************************************************/
	void IncreaseMaxBalls(int amount);

	/*!***********************************************************************
	 \brief
	 For event subscription to add a new upgrade in PlayerHUD
	*************************************************************************/
	void AddNewUpgrade(UpgradeType upgrade);
}

