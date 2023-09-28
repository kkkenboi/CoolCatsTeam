/*!************************************************************************
 \file                SceneManager.h
 \author(s)           Ang Jiawei Jarrett
 \par DP email(s):    a.jiaweijarrett@digipen.edu
 \par Course:         CSD2401A
 \date                27-09-2023
 \brief
 The functions in the SceneManager class include:

**************************************************************************/

#pragma once

#include <string>
#include <vector>
#include "LitterBox/Core/System.h"
#include "LitterBox/Scene/Scene.h"

namespace LB
{
	class SceneManager : public ISystem
	{
	public:
		void Initialize() override { SetSystemName("SceneManager System"); }

		void CreateScene();
		void LoadScene(int index);
		void LoadScene(std::string name);

	private:
		std::vector<Scene> scenes;
	};

	extern SceneManager* SCENEMANAGER;
}
