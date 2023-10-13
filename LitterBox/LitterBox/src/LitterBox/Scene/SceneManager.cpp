/*!************************************************************************
 \file                SceneManager.cpp
 \author(s)           Ang Jiawei Jarrett
 \par DP email(s):    a.jiaweijarrett@digipen.edu
 \par Course:         CSD2401A
 \date                27-09-2023
 \brief
 This file contains the function definitions for the Scene Manager

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "SceneManager.h"

// Test scene, to be refactored in M2
#include "../../../LitterBoxEngine/src/SceneTestMain.h"

namespace LB
{
	SceneManager* SCENEMANAGER = nullptr;

	/*!***********************************************************************
	 \brief
	 Constructor for the SceneManager and creates a new scene
	*************************************************************************/
	SceneManager::SceneManager()
	{
		if (!DEBUG)
			SCENEMANAGER = this;
		else
			std::cerr << "SceneManager System already exist" << std::endl;

		// For now create a test scene
		currentScene = new SceneTestMain;
	}

	/*!***********************************************************************
	 \brief
	 Initializes the SceneManager, setting it's name and initialising the scene
	*************************************************************************/
	void SceneManager::Initialize()
	{
		SetSystemName("SceneManager System");
		currentScene->Init();
	}

	/*!***********************************************************************
	 \brief
	 Updates all active scenes
	*************************************************************************/
	void SceneManager::Update()
	{
		if (TIME->IsPaused()) return;
		currentScene->Update();
	}

	/*!***********************************************************************
	 \brief
	 Destroys all scenes 
	*************************************************************************/
	void SceneManager::Destroy()
	{
		currentScene->Destroy();
		delete currentScene;
	}

	//void SceneManager::LoadScene(int index)
	//{

	//}

	//void SceneManager::LoadScene(std::string name)
	//{

	//}
}