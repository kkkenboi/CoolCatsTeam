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
#include "LitterBox/Core/Core.h"

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

		// On play, serialize the scene. On !play, reload the scene
		CORE->onPlayingModeToggle.Subscribe(LB::SceneOnPlayToggle);

		// TODO: Lookup table for scene names, arranged where 0 index is loaded first!
		LoadScene("Scenetest");
	}

	/*!***********************************************************************
	 \brief
	 Updates all active scenes
	*************************************************************************/
	void SceneManager::Update()
	{
		if (!CORE->IsPlaying() && TIME->IsPaused()) return;
		m_currentScene->Update();
	}

	/*!***********************************************************************
	 \brief
	 Destroys all scenes 
	*************************************************************************/
	void SceneManager::Destroy()
	{
		if (!CORE->IsPlaying())
		{
			m_currentScene->Save();
		}

		m_currentScene->Destroy();
		MEMORY->Deallocate(m_currentScene);
	}

	void SceneManager::LoadScene(std::string name)
	{
		Scene* newScene = MEMORY->Allocate<Scene>(name);
		LoadScene(newScene);
	}

	void SceneManager::LoadScene(Scene* newScene)
	{
		if (CORE->IsPlaying())
		{
			DebuggerLogWarningFormat("Tried to load new scene %s while scene %s is running.", newScene->GetName().c_str(), m_currentScene->GetName().c_str());
			MEMORY->Deallocate(newScene);
			return;
		}

		// TODO: Confirm first before saving
		if (m_currentScene) {
			m_currentScene->Save();
		}

		// Free current scene first
		if (m_currentScene) {
			m_currentScene->Destroy();
			MEMORY->Deallocate(m_currentScene);
		}
		m_currentScene = newScene;

		onNewSceneLoad.Invoke(m_currentScene);

		m_currentScene->Init();
	}

	void SceneManager::SceneOnPlayToggle(bool isPlaying)
	{
		if (isPlaying)
		{
			m_currentScene->Save();
		}
		else
		{
			LoadScene(m_currentScene->GetName());
		}
	}

	// For event subscription
	void SceneOnPlayToggle(bool isPlaying)
	{
		SCENEMANAGER->SceneOnPlayToggle(isPlaying);
	}
}