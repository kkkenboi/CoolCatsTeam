/*!************************************************************************
 \file				EditorCollisionGrid.h
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:       CSD2401A
 \date				09/02/2024
 \brief

 This file contains the Editor things for the CollisionGrid in ImGui

 Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Editor.h"
#include "Litterbox/Engine/Layer.h"
#include "LitterBox/Physics/ColliderManager.h"

namespace LB
{
	class EditorCollisionGrid : public Layer {
	public:

		/*!***********************************************************************
		  \brief
		  Constructs the EditorCollisionGrid
		*************************************************************************/
		EditorCollisionGrid(std::string layerName);
		/*!***********************************************************************
		  \brief
		  Initializes the EditorCollisionGrid
		*************************************************************************/
		void Initialize() override;
		/*!***********************************************************************
		  \brief
		  Updates the EditorCollisionGrid
		*************************************************************************/
		void UpdateLayer() override;
		/*!***********************************************************************
		  \brief
		  Destroys the EditorCollisionGrid class
		*************************************************************************/
		void Destroy() override;
	};

	extern EditorCollisionGrid* COLLISIONGRID;
}