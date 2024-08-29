/*!************************************************************************
 \file				Application.cpp
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

#include "Application.h"

namespace LB 
{
	LB::Application::Application()
	{
		m_running = true;

		m_engine = std::make_shared<LBEngine>();
	}

	void LB::Application::NewFrame() 
	{

	}

	void LB::Application::EndFrame() 
	{

	}
}
