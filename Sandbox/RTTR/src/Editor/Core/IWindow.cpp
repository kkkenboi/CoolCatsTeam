/*!************************************************************************
 \file				IWindow.cpp
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

#include "IWindow.h"

namespace LB
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<IWindow>("IWindow")
			.method("Initialize", &IWindow::Initialize)
			.method("Update", &IWindow::Update)
			.method("Shutdown", &IWindow::Shutdown);
	}
}