/*!************************************************************************
 \file				Serializer.h
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

#include "App/Core/ISystem.h"

namespace LB
{
	class Serializer : public ISystem
	{
	public:
		void Initialize() override;

		void Shutdown() override;

		RTTR_ENABLE(ISystem)
	};
}
