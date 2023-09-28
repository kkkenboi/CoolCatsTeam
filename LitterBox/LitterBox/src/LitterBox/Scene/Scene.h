/*!************************************************************************
 \file                Scene.h
 \author(s)           Ang Jiawei Jarrett
 \par DP email(s):    a.jiaweijarrett@digipen.edu
 \par Course:         CSD2401A
 \date                27-09-2023
 \brief
 The functions in the Scene class include:

**************************************************************************/

#pragma once

#include <list>
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class Scene
	{
	public:
		virtual void Init() {};
		virtual void Update() {};
		virtual void Destroy() {};

	private:
		std::string name;

		// Hierachy will be done in M2
		// GameObject* gameObjs; // All game objects are children of this root game object
	};

	//bool operator==(Scene const& lhs, Scene const& rhs)
	//{

	//}
}
