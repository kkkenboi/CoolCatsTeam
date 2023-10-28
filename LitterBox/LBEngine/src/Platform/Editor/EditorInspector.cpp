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
		//std::cout << inspectTest->GetComponents().size() << std::endl;
		DebuggerLogFormat("Component Size: %d", inspectTest->GetComponents().size());
		EDITOR->InspectGO(inspectTest);


		//
	}

	void EditorInspector::UpdateLayer()
	{

		ImGui::Begin(GetName().c_str(), 0, 0);

		if ((GOMANAGER->GetGameObjects().size() <= 0) || (EDITOR->InspectedGO() == nullptr))
		{	
			ImGui::End();
			// Comment the return to interact with the rest of the function, 
			// but currently a test GameObject is assigned to the m_GameObjectPointer
			return; 
		}

		float normalWidth = 75.f;
		float extendedWidth = 173.f;
		float dropdownWidth = 150.f;

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
					DebuggerLogWarning("Render already exists.");
					ImGui::CloseCurrentPopup();
				}
				else
				{
					EDITOR->InspectedGO()->AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
					EDITOR->InspectedGO()->GetComponent<CPRender>()->Initialise();
					DebuggerLog("Render added!");
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("RigidBody"))
			{
				if (EDITOR->InspectedGO()->HasComponent<CPRigidBody>())
				{
					DebuggerLogWarning("RigidBody already exists.");
					ImGui::CloseCurrentPopup();
				}
				else
				{
					EDITOR->InspectedGO()->AddComponent(C_CPRigidBody, FACTORY->GetCMs()[C_CPRigidBody]->Create());
					EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->Initialise();
					DebuggerLog("RigidBody added!");
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Collider"))
			{
				if (EDITOR->InspectedGO()->HasComponent<CPCollider>())
				{
					DebuggerLogWarning("Collider already exists.");
					ImGui::CloseCurrentPopup();
				}
				//else if (!EDITOR->InspectedGO()->HasComponent<CPRigidBody>())
				//{
				//	DebuggerLogWarning("Game Object doesn't have RigidBody component, add RigidBody component first.");
				//	ImGui::CloseCurrentPopup();
				//}
				else
				{
					EDITOR->InspectedGO()->AddComponent(C_CPCollider, FACTORY->GetCMs()[C_CPCollider]->Create());
					EDITOR->InspectedGO()->GetComponent<CPCollider>()->Initialise();
					DebuggerLog("Collider added!");
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

		ImGui::Separator();

		// Individual Component Sections
		if (EDITOR->InspectedGO()->HasComponent<CPTransform>())
		{
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_None))
			{
				Vec2<float> pos = EDITOR->InspectedGO()->GetComponent<CPTransform>()->GetPosition();
				ImGui::Text("%-17s X", "Position");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##PosX", &pos.x, 1.0f, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##PosY", &pos.y, 1.0f, 0.0f, 0.0f, "%.2f");
				EDITOR->InspectedGO()->GetComponent<CPTransform>()->SetPosition(pos);

				// For Testing
				Vec2<float> returnval = EDITOR->InspectedGO()->GetComponent<CPTransform>()->GetPosition();
				DebuggerLogFormat("Position X: %f, Position Y: %f", returnval.x, returnval.y);
				//
			}
		}
		if (EDITOR->InspectedGO()->HasComponent<CPRender>())
		{
			if (ImGui::CollapsingHeader("Render", ImGuiTreeNodeFlags_None))
			{
				// Interface Buttons
				ImGui::Text("%-19s", "Image");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(dropdownWidth);
				int inspectedTextureID = EDITOR->InspectedGO()->GetComponent<CPRender>()->texture;
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* textureData = ImGui::AcceptDragDropPayload("TEXTURE"))
					{
						const char* textureName = (const char*)textureData->Data;
						EDITOR->InspectedGO()->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->Textures[textureName].second);
					}
				}
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
				float mass = EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->mMass;
				ImGui::Text("%-19s", "Mass");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				ImGui::InputFloat("##Mass", &mass, 0.0f, 0.0f, "%.2f");

				if (mass <= 0.f)
				{
					EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->mMass = 0.1f;
				}
				else
				{
					EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->mMass = mass;
				}

				float restitution = EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->mRestitution;
				ImGui::Text("%-19s", "Restitution");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				ImGui::InputFloat("##Restitution", &restitution, 0.0f, 0.0f, "%.2f");

				if (restitution <= 0.f)
				{
					EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->mRestitution = 0.f;
				}
				else if (restitution >= 1.f)
				{
					EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->mRestitution = 1.f;
				}
				else
				{
					EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->mRestitution = restitution;
				}


				float friction = EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->mFriction;
				ImGui::Text("%-19s", "Friction");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				ImGui::InputFloat("##Friction", &friction, 0.0f, 0.0f, "%.2f");

				if (friction <= 0.f)
				{
					EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->mFriction = 0.f;
				}
				else if (friction >= 1.f)
				{
					EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->mFriction = 1.f;
				}
				else
				{
					EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->mFriction = friction;
				}

				Vec2<float> velocity = EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->mVelocity;
				ImGui::Text("%-17s X", "Velocity");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::InputFloat("##VelocityX", &velocity.x, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::InputFloat("##VelocityY", &velocity.y, 0.0f, 0.0f, "%.2f");

				Vec2<float> acceleration = EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->mAcceleration;
				ImGui::Text("%-17s X", "Acceleration");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::InputFloat("##AccelerationX", &acceleration.x, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::InputFloat("##AccelerationY", &acceleration.y, 0.0f, 0.0f, "%.2f");

				static bool isStatic = false;
				ImGui::Text("%-10s", "Is Static");
				ImGui::SameLine();
				ImGui::Checkbox("##isStatic", &isStatic);
				EDITOR->InspectedGO()->GetComponent<CPRigidBody>()->isStatic = isStatic;

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
				float width = EDITOR->InspectedGO()->GetComponent<CPCollider>()->m_widthUnscaled;
				ImGui::Text("%-19s", "Width");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				ImGui::InputFloat("##Width", &width, 0.0f, 0.0f, "%.2f");

				if (width < 0.f)
				{
					EDITOR->InspectedGO()->GetComponent<CPCollider>()->m_widthUnscaled = 0.f;
				}
				else
				{
					EDITOR->InspectedGO()->GetComponent<CPCollider>()->m_widthUnscaled = width;
				}

				float height = EDITOR->InspectedGO()->GetComponent<CPCollider>()->m_heightUnscaled;
				ImGui::Text("%-19s", "Height");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				ImGui::InputFloat("##Height", &height, 0.0f, 0.0f, "%.2f");

				if (height < 0.f)
				{
					EDITOR->InspectedGO()->GetComponent<CPCollider>()->m_heightUnscaled = 0.f;
				}
				else
				{
					EDITOR->InspectedGO()->GetComponent<CPCollider>()->m_heightUnscaled = height;
				}

				float radius = EDITOR->InspectedGO()->GetComponent<CPCollider>()->m_radiusUnscaled;
				ImGui::Text("%-19s", "Radius");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				ImGui::InputFloat("##Radius", &radius, 0.0f, 0.0f, "%.2f");

				if (radius < 0.f)
				{
					EDITOR->InspectedGO()->GetComponent<CPCollider>()->m_radiusUnscaled = 0.f;
				}
				else
				{
					EDITOR->InspectedGO()->GetComponent<CPCollider>()->m_radiusUnscaled = radius;
				}

				ImGui::Text("%-19s", "Shape Type");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(dropdownWidth);
				if (ImGui::BeginCombo("##ShapeType", EDITOR->InspectedGO()->GetComponent<CPCollider>()->GetShapeName().c_str()))
				{

					for (auto& [str, type] : COLLIDERS->m_shapeTypes)
					{
						if (ImGui::Selectable(str.c_str()))
						{
							EDITOR->InspectedGO()->GetComponent<CPCollider>()->m_shape = type;
						}
					}
					ImGui::EndCombo();
				}

				if (ImGui::Button("Add Vertice"))
				{
					EDITOR->InspectedGO()->GetComponent<CPCollider>()->AddVertice(Vec2<float>{0.f,0.f});
				}

				for (size_t i{}; i < EDITOR->InspectedGO()->GetComponent<CPCollider>()->m_untransformedVerts.size(); ++i)
				{
					Vec2<float> untransformedVerts = EDITOR->InspectedGO()->GetComponent<CPCollider>()->m_untransformedVerts[i];
					ImGui::Text("%s %-8d", "Vertice", i);
					ImGui::SameLine();
					

					ImGui::Text("X");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::InputFloat("##untransformedVerts", &untransformedVerts.x, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();

					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::InputFloat("##untransformedVerts", &untransformedVerts.y, 0.0f, 0.0f, "%.2f");

				}

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
