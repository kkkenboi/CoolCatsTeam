/*!************************************************************************
 \file				ComponentTypeID.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				20/01/2024
 \brief
 This file contains the enum IDs for all components in the GameObject.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

namespace LB
{
	/*!***********************************************************************
	 \brief
	 List of all component types in the engine
	*************************************************************************/
	enum ComponentTypeID
	{
		C_CPNone = 0,
		C_CPRigidBody,
		C_CPTransform,
		C_CPRender,
		C_CPScript,
		C_CPScriptCPP,
		C_CPCollider,
		C_CPCamera,
		C_CPAudioSource,
		C_CPText,
		C_CPAnimator
	};
}