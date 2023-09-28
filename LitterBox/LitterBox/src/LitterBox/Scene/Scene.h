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

	private:
		std::string name;

		std::list<GameObject> gameObjs;
	};

	bool operator==(Scene const& lhs, Scene const& rhs)
	{

	}
}
