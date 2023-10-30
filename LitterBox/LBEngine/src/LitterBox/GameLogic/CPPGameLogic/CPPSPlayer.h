/*!************************************************************************
 \file				CPPSPlayer.h
 \author(s)
 \par DP email(s):
 \par Course:		CSD2401A
 \date				28-09-2023
 \brief
 This file contains

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class CPPSPlayer : public CPPBehaviour
	{
		void Start() override;

		void Update() override;

		void Destroy() override;

		CPRigidBody* rb;
		CPRender* rend;
	};
}