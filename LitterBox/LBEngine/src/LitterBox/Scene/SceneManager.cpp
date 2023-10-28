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
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Serialization/Serializer.h"

namespace LB
{
	SceneManager* SCENEMANAGER = nullptr;

	/*!***********************************************************************
	 \brief
	 Constructor for the SceneManager and creates a new scene
	*************************************************************************/
	SceneManager::SceneManager()
	{
		if (!SCENEMANAGER)
			SCENEMANAGER = this;
		else
			std::cerr << "SceneManager System already exist" << std::endl;
	}

	/*!***********************************************************************
	 \brief
	 Initializes the SceneManager, setting it's name and initialising the scene
	*************************************************************************/
	void SceneManager::Initialize()
	{
		SetSystemName("SceneManager System");

		// TODO: Lookup table for scene names, arranged where 0 index is loaded first!
		LoadScene("Scenetest");
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
		MEMORY->Deallocate(currentScene);
	}

	void SceneManager::LoadScene(std::string name)
	{
		Scene* newScene = MEMORY->Allocate<Scene>(name);
		LoadScene(newScene);
	}

	void SceneManager::LoadScene(Scene* newScene)
	{
		// Free current scene first
		if (currentScene) {
			currentScene->Destroy();
			MEMORY->Deallocate(currentScene);
		}
		currentScene = newScene;

		onNewSceneLoad.Invoke(currentScene);

		currentScene->Init();
	}
}