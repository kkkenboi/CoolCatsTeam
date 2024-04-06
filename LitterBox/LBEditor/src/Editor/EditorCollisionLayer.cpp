/*!************************************************************************
 \file                EditorCollisionLayer.cpp
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

#include "pch.h"
#include "EditorCollisionLayer.h"
#include "LitterBox/Physics/ColliderManager.h"

namespace LB
{
	EditorCollisionLayer* COLLISIONLAYER = nullptr;

	static float const DropDownWidth = 150.f;

	/*!***********************************************************************
	  \brief
	  This constructor initialises the pointer to the Singleton class of the
	  EditorCollisionLayer
	*************************************************************************/
	EditorCollisionLayer::EditorCollisionLayer(std::string layerName) : Layer(layerName)
	{
		if (!COLLISIONLAYER)
		{
			COLLISIONLAYER = this;
		}
		else
		{
			DebuggerLogError("Collision Layer already exists!");
		}
	}

	/*!***********************************************************************
	  \brief
	  This function initializes the EditorCollisionLayer
	*************************************************************************/
	void EditorCollisionLayer::Initialize()
	{

	}

	size_t currentItem{ 0 };
	char LayerNameNew[64] = "New Layer Name";

	/*!***********************************************************************
	  \brief
	  This function updates the ImGui window to contain the elements of
	  the CollisionLayerSystem, allowing you to display the current layer
	  and what it can collide with as well as editing the amount of layers
	  currently being used within the game scene
	*************************************************************************/
	void EditorCollisionLayer::UpdateLayer()
	{
		if (ImGui::Begin(GetName().c_str()))
		{
			if (ImGui::Button("Save"))
			{
				JSONSerializer::SerializeToFile("CollisionMatrix", COLLIDERS->GetLayerSystem());
			}
			ImGui::SameLine();
			if (ImGui::Button("Load"))
			{
				JSONSerializer::DeserializeFromFile("CollisionMatrix", COLLIDERS->GetLayerSystem());
			}
			ImGui::SameLine();
			ImGui::Text("%-19s", "Layer");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(DropDownWidth);
			if (ImGui::BeginCombo("##LayerCollider", COLLIDERS->GetLayerSystem().GetLayerVector()[currentItem].first.c_str()) )
			{
				for (size_t i = 0; i < COLLIDERS->GetLayerSystem().GetLayerVector().size(); ++i)
				{
					bool isSelected = (i == currentItem);
					if (ImGui::Selectable(COLLIDERS->GetLayerSystem().GetLayerVector()[i].first.c_str(), isSelected))
					{
						currentItem = i;
					}
				}
				ImGui::EndCombo();
			}

			// Text Box that you can edit that has the name of the new layer to add
			ImGui::SetNextItemWidth(200.f);
			ImGui::InputText("##LayerName", LayerNameNew, 64);
			ImGui::SameLine();
			if (ImGui::Button("Add Layer"))
			{
				COLLIDERS->GetLayerSystem().AddLayer(std::string{ LayerNameNew });
			}

			for (size_t j = 0; j < COLLIDERS->GetLayerSystem().GetLayerVector().size(); ++j)
			{
				ImGui::SetNextItemWidth(DropDownWidth);
				//auto& ref = static_cast<bool>(COLLIDERS->GetLayerSystem().GetLayerMatrix()[i][j]);
				bool state = (COLLIDERS->GetLayerSystem().GetLayerMatrix()[currentItem][j] != 0);
				ImGui::Checkbox(COLLIDERS->GetLayerSystem().GetLayerVector()[j].first.c_str(),
					&state);
				ImGui::SameLine();
				if (ImGui::Button("-")) 
				{
					COLLIDERS->GetLayerSystem().RemoveLayer(COLLIDERS->GetLayerSystem().GetLayerVector()[j].first);
				}
				COLLIDERS->GetLayerSystem().GetLayerMatrix()[currentItem][j] = state;
				COLLIDERS->GetLayerSystem().GetLayerMatrix()[j][currentItem] = state;
			}

		}
		ImGui::End();
	}

	/*!***********************************************************************
	  \brief
	  This function is called when EditorCollisionLayer is destroyed
	*************************************************************************/
	void EditorCollisionLayer::Destroy()
	{

	}
}