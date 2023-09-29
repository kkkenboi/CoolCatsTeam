/*!************************************************************************
 \file                SceneManager.cpp
 \author(s)           Ang Jiawei Jarrett
 \par DP email(s):    a.jiaweijarrett@digipen.edu
 \par Course:         CSD2401A
 \date                27-09-2023
 \brief
 The functions in the SceneManager class include:

**************************************************************************/

#include "SceneManager.h"

// Test scene, to be refactored in M2
#include "../../../Sandbox/src/SceneTestMain.h"

namespace LB
{
	SceneManager* SCENEMANAGER = nullptr;

	/*!***********************************************************************
	 \brief


	 \return

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


	 \return

	*************************************************************************/
	void SceneManager::Initialize()
	{
		SetSystemName("SceneManager System");
		currentScene->Init();
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void SceneManager::Update()
	{
		if (TIME->IsPaused()) return;
		currentScene->Update();
	}

	/*!***********************************************************************
	 \brief


	 \return

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