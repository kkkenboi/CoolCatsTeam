/*!************************************************************************
 \file				ISystem.h
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
#include "rttr/type"

namespace LB
{
	class ISystem
	{
	public:
		virtual void Initialize() = 0;

		virtual void Shutdown() = 0;

		virtual ~ISystem() = default;

		RTTR_ENABLE()
	};

	class IFixedUpdateSystem : public ISystem
	{
	public:
		virtual void FixedUpdate() = 0;

		RTTR_ENABLE(ISystem)
	};

	class IUpdateSystem : public ISystem
	{
	public:
		virtual void Update() = 0;

		RTTR_ENABLE(ISystem)
	};
}
