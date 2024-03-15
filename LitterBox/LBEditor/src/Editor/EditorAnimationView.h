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
	class EditorAnimationView : public Layer, public Singleton<EditorAnimationView>
	{
	public:
		/*!***********************************************************************
		  \brief
		  This constructor initialises the pointer to the Singleton class of the
		  EditorCollisionLayer
		*************************************************************************/
		EditorAnimationView(std::string layerName);

		/*!***********************************************************************
		  \brief
		  Caches the editor sprite sheet for default rendering of animations
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		  \brief
		  Renders the animation view layer.
		*************************************************************************/
		void UpdateLayer() override;

		/*!***********************************************************************
		  \brief
		  Destroys the animation editor layer.
		*************************************************************************/
		void Destroy() override;

	private:
		SpriteSheet* m_editorSSheet{nullptr};
	};
}