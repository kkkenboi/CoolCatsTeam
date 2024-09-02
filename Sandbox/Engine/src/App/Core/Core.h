/*!************************************************************************
 \file				Core.h
 \author(s)
 \par DP email(s):
 \par Course:
 \date
 \brief

 This file contains

 Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <vector>
#include <memory>

#include "ISystem.h"

namespace LB
{
	class __declspec(dllexport) LBEngine
	{
	public:
		LBEngine();

		void Initialize();

		void Update();

		void Shutdown();

	private:
		std::vector<std::shared_ptr<ISystem>> m_systems{}, m_updateSystems{}, m_fixedUpdateSystems{};
	};
}
