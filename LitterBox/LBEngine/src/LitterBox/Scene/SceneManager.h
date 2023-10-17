/*!************************************************************************
 \file                SceneManager.h
 \author(s)           Ang Jiawei Jarrett
 \par DP email(s):    a.jiaweijarrett@digipen.edu
 \par Course:         CSD2401A
 \date                27-09-2023
 \brief
 This file contains functions to manage the scenes. It also contains a 
 pointer to the main active scene

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <string>
#include <vector>
#include "LitterBox/Core/System.h"
#include "LitterBox/Scene/Scene.h"

namespace LB
{
	/*!***********************************************************************
	 \brief
	 This class manages all the scenes in the game
	*************************************************************************/
	class SceneManager : public ISystem
	{
	public:
		/*!***********************************************************************
		 \brief
		 Constructor for the Scenemanager (Initialises its singleton)
		*************************************************************************/
		SceneManager();

		/*!***********************************************************************
		 \brief
		 Intialise function for the SceneManager (called by ISystem)
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		 \brief
		 Update function for the SceneManager (updated by ISystem)
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		 \brief
		 Destroy function for the SceneManager (called by ISystem)
		*************************************************************************/
		void Destroy() override;

		//void LoadScene(int index);
		//void LoadScene(std::string name);

	private:
		//std::vector<Scene> scenes;
		//TODO : Implementation for multiple scenes in the future
		/*!***********************************************************************
		 \brief
		 Pointer the current scene (This is the main scene)
		*************************************************************************/
		Scene* currentScene;
	};

	/*!***********************************************************************
	 \brief
	 Global pointer for the Scene Manager (Singleton)
	*************************************************************************/
	extern SceneManager* SCENEMANAGER;
}
