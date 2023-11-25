/*!************************************************************************
 \file                EditorCollisionLayer.h
 \author(s)           Justine Carlo Villa Ilao
 \par DP email(s):    justine.c@digipen.edu
 \par Course:         CSD2401A
 \date                25-11-2023
 \brief
 This file contains the class EditorCollisionLayer which handles
 the ImGui display of the CollisionLayerSystem of the game

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Editor.h"
#include "Litterbox/Engine/Layer.h"

namespace LB 
{
	class EditorCollisionLayer : public Layer {
	public:
		/*!***********************************************************************
		  \brief
		  This constructor initialises the pointer to the Singleton class of the
		  EditorCollisionLayer
		*************************************************************************/
		EditorCollisionLayer(std::string layerName);

		/*!***********************************************************************
		  \brief
		  This function initializes the EditorCollisionLayer
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		  \brief
		  This function updates the ImGui window to contain the elements of
		  the CollisionLayerSystem, allowing you to display the current layer
		  and what it can collide with as well as editing the amount of layers
		  currently being used within the game scene
		*************************************************************************/
		void UpdateLayer() override;

		/*!***********************************************************************
		  \brief
		  This function is called when EditorCollisionLayer is destroyed
		*************************************************************************/
		void Destroy() override;

	private:
	};

	extern EditorCollisionLayer* COLLISIONLAYER;
}