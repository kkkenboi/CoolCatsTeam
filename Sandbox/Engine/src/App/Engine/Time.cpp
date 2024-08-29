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

// DEBUG : TO REMOVE
#include <iostream>

namespace LB
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<Time>("LB::Time")
			.constructor<>()
			.method("Initialize", &Time::Initialize)
			.method("Shutdown", &Time::Shutdown);

		std::cout << "Time Registered" << std::endl;
	}

	void Time::Initialize()
	{

	}

	void Time::Shutdown()
	{

	}
}