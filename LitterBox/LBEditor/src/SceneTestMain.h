/*!************************************************************************
 \file				SceneTestMain.h
 \author(s)			Vanessa Chua Siew Jin,
					Carlo Villa Ilao Justine,
					CHIA Amadeus Jinhan
 \par DP email(s):	vanessasiewjin.chua@digipen.edu,
					justine.c@digipen.edu,
					amadeusjinhan.chia@digipen.edu
 \par Course:		CSD2401A
 \date				24-09-2023
 \brief
 The functions in the SceneTestMain class include:

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "LitterBox.h"
using namespace LB;

/*!***********************************************************************
 \brief
 The main scene is a scene so we need to inherit
*************************************************************************/
class SceneTestMain : public Scene
{
public:
	/*!***********************************************************************
	 \brief
	 Initialises the current scene
	*************************************************************************/
	void Init() override;
	/*!***********************************************************************
	 \brief
	 Updates the scene
	*************************************************************************/
	void Update() override;
	/*!***********************************************************************
	 \brief
	 Destroys the scene
	*************************************************************************/
	void Destroy() override;
};
