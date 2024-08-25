/*!************************************************************************
 \file				Serializer.cpp
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

#include "Serializer.h"

namespace LB
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<Serializer>("LB::Serializer")
			.method("Initialize", &Serializer::Initialize)
			.method("Shutdown", &Serializer::Shutdown);
	}

	void Serializer::Initialize()
	{

	}

	void Serializer::Shutdown()
	{

	}
}
