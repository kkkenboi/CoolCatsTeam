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

		/*!***********************************************************************
		 \brief
		 Loads or saves the scene when the play button is pressed
		*************************************************************************/
		void SceneOnPlayToggle(bool isPlaying);

		void CheckSceneToLoad();

		/*!***********************************************************************
		 \brief
		 Loads a scene based on index in scene order
		*************************************************************************/
		void LoadScene(int index);

		/*!***********************************************************************
		 \brief
		 Loads a scene based on the given name
		*************************************************************************/
		void LoadScene(std::string name);

		/*!***********************************************************************
		 \brief
		 Loads a scene based on the pointer given
		*************************************************************************/
		void LoadScene(Scene* scene);

		/*!***********************************************************************
		 \brief
		 Loads the same scene again without saving
		*************************************************************************/
		void ReloadScene();

		/*!***********************************************************************
		 \brief
		 Gets the current scene loaded in SceneManager
		*************************************************************************/
		Scene* GetCurrentScene();

		/*!***********************************************************************
		\brief
		Serializes the scene order to the JSON file
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc);

		/*!***********************************************************************
		 \brief
		 Loads the scene order from the JSON file
		*************************************************************************/
		bool Deserialize(const Value& data);

		// Lets any subscribers know when a new scene is loaded
		Event<Scene*> onNewSceneLoad;

	private:
		std::vector<std::string> m_sceneOrder; // Names of all scenes loaded

		Scene* m_currentScene{ nullptr };	// Which scene is currently loaded?
		Scene* m_nextScene{ nullptr };		// Which scene to load next (at the end of the frame)?
		bool m_isReloading{ false };		// Is the scene reloading?
	};

	// For event subscription
	void SceneOnPlayToggle(bool isPlaying);
	void CheckSceneToLoad();

	/*!***********************************************************************
	 \brief
	 Global pointer for the Scene Manager (Singleton)
	*************************************************************************/
	extern SceneManager* SCENEMANAGER;
}
