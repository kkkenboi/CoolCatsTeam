/*!************************************************************************
 \file				ISystem.cpp
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

#include "ISystem.h"

namespace LB
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<ISystem>("ISystem")
			.method("Initialize", &ISystem::Initialize)
			.method("Shutdown", &ISystem::Shutdown);

		rttr::registration::class_<IFixedUpdateSystem>("IFixedUpdateSystem")
			.method("FixedUpdate", &IFixedUpdateSystem::FixedUpdate);

		rttr::registration::class_<IUpdateSystem>("IUpdateSystem")
			.method("Update", &IUpdateSystem::Update);
	}
}