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

namespace LB
{
	SceneManager* SCENEMANAGER = nullptr;

	SceneManager::SceneManager()
	{
		if (!DEBUG)
			SCENEMANAGER = this;
		else
			std::cerr << "SceneManager System already exist" << std::endl;
	}


}