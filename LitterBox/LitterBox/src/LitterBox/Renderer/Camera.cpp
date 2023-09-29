/*!************************************************************************
 \file                Camera.cpp
 \author(s)           Ryan Tan Jian Hao
 \par DP email(s):    ryanjianhao.tan\@digipen.edu
 \par Course:         CSD2401A
 \date                28-09-2023
 \brief
 
 File containing definition for Camera class's member functions.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "Camera.h"
#include "Platform\Windows\Windows.h"

namespace Renderer {
	/*!***********************************************************************
	\brief
	 Contrustor for Camera class object that does ortho graphic projection
	 with the left and bottom being 0 and the right and top being screen width
	 and height respectively. Also creates a world->NDC matrix to be used.
	*************************************************************************/
	Camera::Camera() :
		ortho{ 2.f / LB::WINDOWSSYSTEM->GetWidth(), 0.f ,0.f, 0.f,
						  0.f, 2.f / LB::WINDOWSSYSTEM->GetHeight(), 0.f, 0.f,
						  0.f, 0.f, 0.2f, 0.f,
						  -1.f, -1.f, -(0.2f), 1.f },
		world_NDC{ ortho }
	{
		//near = 4.f
		//far = -6.f
		float hvf = (float)LB::WINDOWSSYSTEM->GetHeight();
		float wvf = (float)LB::WINDOWSSYSTEM->GetWidth();
		float lvf = 0.f;
		float rvf = wvf;
		ortho = { 2.f / rvf - lvf, 0.f, 0.f, 0.f,
				 0.f, 2.f / hvf, 0.f, 0.f,
				 0.f, 0.f, 0.2f, 0.f,
				 -(rvf + lvf) / (rvf - lvf), -1.f, -0.2f, 1.f };
		world_NDC = ortho * nel;
	}
}