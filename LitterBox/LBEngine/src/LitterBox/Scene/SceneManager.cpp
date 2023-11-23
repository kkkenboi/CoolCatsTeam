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
		JSONSerializer::DeserializeFromFile("SceneOrder", *this);

		LoadScene(0);
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
		// Save scene order
		JSONSerializer::SerializeToFile("SceneOrder", *this);

		// Auto-save current loaded scene if game is not playing
		if (!CORE->IsPlaying())
		{
			m_currentScene->Save();
		}

		m_currentScene->Destroy();
		MEMORY->Deallocate(m_currentScene);
	}

	/*!***********************************************************************
	 \brief
	 Loads a scene based on index in scene order
	*************************************************************************/
	void SceneManager::LoadScene(int index)
	{
		if (index < 0 || index >= m_sceneOrder.size())
		{
			DebuggerLogErrorFormat("Tried to load scene index out of range: %d!", index);
			return;
		}
		LoadScene(m_sceneOrder[index]);
	}

	/*!***********************************************************************
	 \brief
	 Loads a scene based on the given name
	*************************************************************************/
	void SceneManager::LoadScene(std::string name)
	{
		Scene* newScene = MEMORY->Allocate<Scene>(name);
		LoadScene(newScene);
	}

	/*!***********************************************************************
	 \brief
	 Loads a scene based on the pointer given
	*************************************************************************/
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
			if (m_isReloading)
				m_isReloading = false;
			else
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

	/*!***********************************************************************
	 \brief
	 Loads the same scene again without saving
	*************************************************************************/
	void SceneManager::ReloadScene()
	{
		m_isReloading = true;
		LoadScene(m_currentScene->GetName());
	}

	/*!***********************************************************************
	 \brief
	 Gets the current scene loaded in SceneManager
	*************************************************************************/
	Scene* SceneManager::GetCurrentScene()
	{
		return m_currentScene;
	}

	/*!***********************************************************************
	 \brief
	 Loads or saves the scene when the play button is pressed
	*************************************************************************/
	void SceneManager::SceneOnPlayToggle(bool isPlaying)
	{
		if (isPlaying)
			m_currentScene->Save();
		else
			ReloadScene();
	}

	// For event subscription
	void SceneOnPlayToggle(bool isPlaying)
	{
		SCENEMANAGER->SceneOnPlayToggle(isPlaying);
	}

	/*!***********************************************************************
	\brief
	Serializes the scene order to the JSON file
	*************************************************************************/
	bool SceneManager::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		Value sceneArray(rapidjson::kArrayType);
		for (const auto& scene : m_sceneOrder)
		{
			Value sceneName(scene.c_str(), alloc);
			sceneArray.PushBack(sceneName, alloc);
		}
		data.AddMember("SceneOrder", sceneArray, alloc);

		return true;
	}

	/*!***********************************************************************
	 \brief
	 Loads the scene order from the JSON file
	*************************************************************************/
	bool SceneManager::Deserialize(const Value& data)
	{
		DebuggerLogWarning("Ran!!");

		bool HasScene = data.HasMember("SceneOrder");
		if (data.IsObject())
		{
			if (HasScene)
			{
				const Value& scenesValue = data["SceneOrder"].GetArray();
				for (rapidjson::SizeType i{}; i < scenesValue.Size(); ++i)
				{
					m_sceneOrder.push_back(scenesValue[i].GetString());
				}

				DebuggerLogWarningFormat("Scene Order size: %d", m_sceneOrder.size());
				return true;
			}
		}

		return false;
	}
}