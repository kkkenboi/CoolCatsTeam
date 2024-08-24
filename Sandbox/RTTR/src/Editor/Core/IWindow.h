/*!************************************************************************
 \file				IWindow.h
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

#include "rttr/registration.h"

namespace LB
{
	class IWindow
	{
	public:
		virtual void Initialize() = 0;

		virtual void Update() = 0;

		virtual void Shutdown() = 0;
	};
}
