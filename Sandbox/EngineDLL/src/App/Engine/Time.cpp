/*!************************************************************************
 \file				Time.cpp
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

#include "Time.h"

#include <iostream>

namespace LB
{
	RTTR_REGISTRATION
	{
		std::cout << "IT WORKS\n";

		rttr::registration::class_<Time>("LB::Time")
			.constructor<>()
			.method("Initialize", &Time::Initialize)
			.method("Shutdown", &Time::Shutdown);
	}

	void Time::Initialize()
	{

	}

	void Time::Shutdown()
	{

	}
}