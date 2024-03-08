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
#include "LitterBox/Renderer/Renderer.h"

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
			static std::string dropdowntitle{};
			//loop through all possible layers
			for (auto layer{ Renderer::Renderer_Types::RT_OBJECT };
				layer != Renderer::Renderer_Types::Last; 
				++layer)
			{
				//NOTE: important to know which layers are present in
				//RenderSystem and exclude. Otherwise the loop will 
				//mess up and exceptions.
				if (layer == Renderer::Renderer_Types::RT_DEBUG)
					continue;

				//we need a c style string based on which layer we want to list
				//so we use this switch case and a static std::string for it
				switch (layer)
				{
				case Renderer::Renderer_Types::RT_OBJECT:
					dropdowntitle = "Object layer";
					break;
				case Renderer::Renderer_Types::RT_BACKGROUND:
					dropdowntitle = "Background layer";
					break;
				case Renderer::Renderer_Types::RT_UI:
					dropdowntitle = "UI layer";
					break;
				}

				//check box to toggle the layer list
				ImGui::Checkbox(("##" + dropdowntitle).c_str(), const_cast<bool*>(&Renderer::GRAPHICS->get_layer_active(layer)));
				ImGui::SameLine();
				
				if (ImGui::TreeNode(dropdowntitle.c_str()))
				{
					//TODO: ADD A BACKGROUND GAME OBJECT WITH A NAME IN THE GAME
					//OTHERWISE THIS SHIT WON'T WORK
					/*if (layer == Renderer::Renderer_Types::RT_BACKGROUND) {
						ImGui::TreePop();
						continue;
					}*/
					const auto& obj_list = Renderer::GRAPHICS->get_layer_objs(layer);
					for (auto& e : obj_list)
					{
						if (ImGui::Checkbox(e->gameObj->GetName().c_str(), &const_cast<CPRender*>(e)->activated))
						{
							//needs to retoggle the activation member variable because the set_active function toggles it as well
							const_cast<CPRender*>(e)->activated = !e->activated;
							const_cast<CPRender*>(e)->set_active();
						}
					}
					ImGui::TreePop();
				}
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