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
#include "SceneEmpty.h"
#include "LitterBox/Engine/Time.h"

namespace LB
{
	SceneManager* SCENEMANAGER = nullptr;

	/*!***********************************************************************
	 \brief
	 Constructor for the SceneManager and creates a new scene
	*************************************************************************/
	SceneManager::SceneManager(Scene* firstScene)
	{
		if (!DEBUG)
			SCENEMANAGER = this;
		else
			std::cerr << "SceneManager System already exist" << std::endl;

		nextScene = firstScene;
	}

	/*!***********************************************************************
	 \brief
	 Initializes the SceneManager, setting it's name and initialising the scene
	*************************************************************************/
	void SceneManager::Initialize()
	{
		SetSystemName("SceneManager System");

		// Load an empty scene first
		LoadScene(nextScene);
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

	// TODO: REFACTOR TO USE JSON FILE 
	void SceneManager::LoadScene(Scene* newScene)
	{
		// Free current scene first
		if (currentScene) {
			currentScene->Destroy();
			delete currentScene;
		}

		currentScene = newScene;
		currentScene->Init();
	}
}