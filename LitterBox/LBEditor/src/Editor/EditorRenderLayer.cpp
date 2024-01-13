/*!************************************************************************
 \file                EditorRenderLayer.cpp
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

#include "pch.h"
#include "EditorRenderLayer.h"
#include "LitterBox/Renderer/ForwardDeclerators.h"

namespace LB {
	EditorRenderLayer* RENDERLAYER = nullptr;

	static float const DropDownWidth = 150.f;

	/*!***********************************************************************
	  \brief
	  This constructor initialises the pointer to the Singleton class of the
	  EditorRenderLayer
	*************************************************************************/
	EditorRenderLayer::EditorRenderLayer(std::string layerName) : Layer(layerName)
	{
		if (!RENDERLAYER)
		{
			RENDERLAYER = this;
		}
		else
		{
			DebuggerLogError("Render Layer editor already exists!");
		}
	}

	/*!***********************************************************************
	  \brief
	  This function initializes the EditorRenderLayer
	*************************************************************************/
	void EditorRenderLayer::Initialize()
	{

	}

	/*!***********************************************************************
	  \brief
	  This function updates the ImGui window to contain the elements of
	  the Render layer.
	*************************************************************************/
	void EditorRenderLayer::UpdateLayer()
	{
		if (ImGui::Begin(GetName().c_str()))
		{
			for (auto layer{ Renderer::Renderer_Types::RT_OBJECT };
				layer != Renderer::Renderer_Types::Last; ++layer)
			{
				ImGui::Text("Render Layer");
			}
		}
		ImGui::End();
	}

	/*!***********************************************************************
	  \brief
	  This function is called when EditorRenderLayer is destroyed
	*************************************************************************/
	void EditorRenderLayer::Destroy()
	{
	}
}