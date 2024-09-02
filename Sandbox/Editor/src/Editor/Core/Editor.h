/*!************************************************************************
 \file				Editor.h
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

#include "IWindow.h"

namespace LB
{
	class __declspec(dllexport) Editor
	{
	public:
		void InitializeEditor();

		void UpdateEditor();

		void ShutdownEditor();

	private:
		std::vector<std::shared_ptr<IWindow>> m_windows;
	};
}
