/*!************************************************************************
 \file				EditorHierarchy.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains functions defintions for the hierarchy layer of the
 Editor. This is to ensure that there is a overview of all the game objects
 in the game.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "pch.h"
#include "EditorInspector.h"
#include "LitterBox/Components/RenderComponent.h"
#include "LitterBox/Components/RigidBodyComponent.h"
#include "LitterBox/Components/TransformComponent.h"


// For testing 
#include "LitterBox/Factory/GameObjectFactory.h"
namespace LB
{
	// For testing
	GameObject* inspectTest;
	//

	EditorInspector::EditorInspector(std::string layerName) : Layer(layerName) 
	{
	}

	void EditorInspector::Initialize()
	{
		// For testing
		inspectTest = FACTORY->SpawnGameObject();
		std::cout << inspectTest->GetComponents().size() << std::endl;
		EDITOR->InspectGO(inspectTest);

		m_AssetsIterator = ASSETMANAGER->Textures.begin();
		//
	}

	void EditorInspector::UpdateLayer()
	{

		ImGui::Begin(GetName().c_str(), 0, ImGuiWindowFlags_MenuBar);

		if ((GOMANAGER->GetGameObjects().size() <= 0) || (EDITOR->InspectedGO() == nullptr))
		{	
			ImGui::End();
			// Comment the return to interact with the rest of the function, 
			// but currently a test GameObject is assigned to the m_GameObjectPointer
			return; 
		}

		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("Add Component");
		}

		// Upon clicking to add a component to the Inspected Game Object
		if (ImGui::BeginPopup("Add Component"))
		{

			if (ImGui::MenuItem("Render"))
			{
				if (EDITOR->InspectedGO()->HasComponent<CPRender>())
				{
					std::cout << "Render already exists." << std::endl;
					ImGui::CloseCurrentPopup();
				}
				else
				{
					EDITOR->InspectedGO()->AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
					EDITOR->InspectedGO()->GetComponent<CPRender>()->Initialise();
					std::cout << "Render added!" << std::endl;
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("RigidBody"))
			{
				if (EDITOR->InspectedGO()->HasComponent<CPRigidBody>())
				{
					std::cout << "RigidBody already exists." << std::endl;
					ImGui::CloseCurrentPopup();
				}
				else
				{
					EDITOR->InspectedGO()->AddComponent(C_CPRigidBody, FACTORY->GetCMs()[C_CPRigidBody]->Create());
					EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->Initialise();
					std::cout << "RigidBody added!" << std::endl;
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Collider"))
			{
				if (EDITOR->InspectedGO()->HasComponent<CPCollider>())
				{
					std::cout << "Collider already exists." << std::endl;
					ImGui::CloseCurrentPopup();
				}
				else
				{
					EDITOR->InspectedGO()->AddComponent(C_CPCollider, FACTORY->GetCMs()[C_CPCollider]->Create());
					EDITOR->InspectedGO()->GetComponent<CPCollider>()->Initialise();
					std::cout << "Collider added!" << std::endl;
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

		int normalWidth = 75;
		int dropdownWidth = 150;
		//std::cout << inspectTest->GetComponents().size() << std::endl;

		// Individual Component Sections
		if (EDITOR->InspectedGO()->HasComponent<CPTransform>())
		{
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_None))
			{
				Vec2<float> pos = EDITOR->InspectedGO()->GetComponent<CPTransform>()->GetPosition();
				ImGui::Text("Position %10s", "X");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::InputFloat("##X", &pos.x, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::InputFloat("##Y", &pos.y, 0.0f, 0.0f, "%.2f");
				EDITOR->InspectedGO()->GetComponent<CPTransform>()->SetPosition(pos);

				// For Testing
				Vec2<float> returnval = EDITOR->InspectedGO()->GetComponent<CPTransform>()->GetPosition();
				std::cout << returnval.x << " " << returnval.y << std::endl;
				//
			}
		}
		if (EDITOR->InspectedGO()->HasComponent<CPRender>())
		{
			if (ImGui::CollapsingHeader("Render", ImGuiTreeNodeFlags_None))
			{
				// Interface Buttons
				ImGui::Text("Image : "); 
				ImGui::SameLine();
				ImGui::SetNextItemWidth(dropdownWidth);
				int inspectedTextureID = EDITOR->InspectedGO()->GetComponent<CPRender>()->texture;
				if (ImGui::BeginCombo("##Texture", ASSETMANAGER->GetTextureName(inspectedTextureID).c_str()))
				{
					for (auto& [str, tex] : ASSETMANAGER->Textures)
					{
						if (ImGui::Selectable(str.c_str()))
						{
							EDITOR->InspectedGO()->GetComponent<CPRender>()->UpdateTexture(tex.second);
						}
					}
					ImGui::EndCombo();
				}

				// Delete Component
				if (ImGui::Button("Delete Render Component"))
				{
					EDITOR->InspectedGO()->RemoveComponent(C_CPRender);
				}
			}
		}
		if (EDITOR->InspectedGO()->HasComponent<CPRigidBody>())
		{
			if (ImGui::CollapsingHeader("RigidBody", ImGuiTreeNodeFlags_None))
			{
				// Interface Buttons


				// Delete Component
				if (ImGui::Button("Delete RigidBody Component"))
				{
					EDITOR->InspectedGO()->RemoveComponent(C_CPRigidBody);
				}
			}
		}
		if (EDITOR->InspectedGO()->HasComponent<CPCollider>())
		{
			if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_None))
			{
				// Interface Buttons


				// Delete Component
				if (ImGui::Button("Delete Collider Component"))
				{
					EDITOR->InspectedGO()->RemoveComponent(C_CPCollider);
				}
			}
		}


		ImGui::End();
	}

}
