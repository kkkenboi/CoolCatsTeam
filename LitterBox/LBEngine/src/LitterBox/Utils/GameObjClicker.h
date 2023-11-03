/*!************************************************************************
 \file				GameObjClicker.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				28/10/2023
 \brief

 This file declares the GameObject function that selects objects in the SceneView.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

namespace LB 
{
	/*!***********************************************************************
	 * \brief
	 * Returns the GameObject that the clicker has clicked on
	 *************************************************************************/
	GameObject* CheckMousePosGameObj(Vec2<float> const& position);
}