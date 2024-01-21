/*!************************************************************************
 \file                EditorRenderLayer.h
 \author(s)           Ryan Tan Jian Hao
 \par DP email(s):    ryanjianhao.tan\@digipen.edu
 \par Course:         CSD2401A
 \date                13-01-2024
 \brief
 This file contains the class EditorRenderLayer which handles
 the ImGui display of the different Renderers in the RenderSystem

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Editor.h"
#include "LitterBox/Engine/Layer.h"

namespace LB
{
	class EditorRenderLayer : public Layer {
	public:
		/*!***********************************************************************
		  \brief
		  This constructor initialises the pointer to the Singleton class of the
		  EditorRenderLayer
		*************************************************************************/
		EditorRenderLayer(std::string layerName);

		/*!***********************************************************************
		  \brief
		  This function initializes the EditorRenderLayer
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		  \brief
		  This function updates the ImGui window to contain the elements of
		  the Render layer.
		*************************************************************************/
		void UpdateLayer() override;

		/*!***********************************************************************
		  \brief
		  This function is called when EditorRenderLayer is destroyed
		*************************************************************************/
		void Destroy() override;

	private:
	};

	extern EditorRenderLayer* RENDERLAYER;
}