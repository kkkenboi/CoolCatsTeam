/*!************************************************************************
 \file				EditorHierarchy.cpp
 \author(s)			Kenji Brannon Chong, Vanessa Chua Siew Jin
 \par DP email(s):	kenjibrannon.c@digipen.edu, vanessasiewjin.chua@digipen.edu
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
#include "EditorHierarchy.h"
#include "EditorInspector.h"
#include "EditorSceneView.h"
#include "EditorAssets.h"
#include "LitterBox/Core/Core.h"
#include "LitterBox/Components/RenderComponent.h"
#include "LitterBox/Components/RigidBodyComponent.h"
#include "LitterBox/Components/TransformComponent.h"
#include "LitterBox/Components/AudioSourceComponent.h"

#include "Utils/CommandManager.h"
#include "Commands/TransformCommands.h"
#include "Commands/GameObjectCommands.h"

#include "LitterBox/Serialization/FileSystemManager.h"

namespace LB
{
	EditorInspector* EDITORINSPECTOR{ nullptr };

	/*!***********************************************************************
	  \brief
	  Constructor for the EditorInspector class.
	  \return
	  Nothing.
	*************************************************************************/
	EditorInspector::EditorInspector(std::string layerName) : Layer(layerName)
	{
		if (!EDITORINSPECTOR)
			EDITORINSPECTOR = this;
		else
			DebuggerLogError("Editor Inspector already exist!");
	}

	/*!***********************************************************************
	  \brief
	  Initializes the EditorInspector layer.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorInspector::Initialize()
	{
		EDITORHIERACHY->onNewObjectSelected.Subscribe(LB::UpdateInspectedGO);
		CORE->onPlayingModeToggle.Subscribe(LB::DeselectObject);
		SCENEMANAGER->onNewSceneLoad.Subscribe(LB::DeselectObject);

		// Set default snap values
		m_SnapTranslate[0] = 10.f, m_SnapTranslate[1] = 10.f;
		m_SnapRotate = 10.f;
		m_SnapScale[0] = 0.1f, m_SnapScale[1] = 0.1f;

	}

	/*!***********************************************************************
	  \brief
	  Updates the EditorInspector layer.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorInspector::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str(), 0, 0);

		// If no game object is selected, don't render the inspector (as there's nothing to inspect)
		if (!m_inspectedGO)
		{	
			ImGui::End();
			return; 
		}

		float normalWidth = 75.f;
		float extendedWidth = 173.f;
		float dropdownWidth = 150.f;

		//------------------------------------------ADD COMPONENT WINDOW------------------------------------------
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("Add Component");
		}
		if (!isPrefab)
		{
			ImGui::SameLine();
			if (ImGui::Button("Create Prefab"))
			{
				if (std::filesystem::exists(FILESYSTEM->GetFilePath(m_inspectedGO->GetName() + ".json")))
				{
					DebuggerLogWarning("Prefab with that name already exists! Use a different name!");
				}
				else
				{
					DebuggerLog("Creating Prefab!");
					std::filesystem::path prefab("Prefabs");
					std::filesystem::path assetFileName(GetInspectedGO()->GetName());
					JSONSerializer::SerializeToFile((EDITORASSETS->defaultDirectory/prefab / assetFileName).string(), *GetInspectedGO());
				}
			}
		}
	/*	if (isPrefab)
		{
			ImGui::SameLine();
			if (ImGui::Button("Save"))
			{
				DebuggerLogFormat("Saving Prefab %s ",GetInspectedGO()->GetName().c_str());
				JSONSerializer::SerializeToFile(GetInspectedGO()->GetName(), *GetInspectedGO());
			}
		}*/

		// Upon clicking to add a component to the Inspected Game Object
		if (ImGui::BeginPopup("Add Component"))
		{

			if (ImGui::MenuItem("Render"))
			{
				if (m_inspectedGO->HasComponent<CPRender>())
				{
					DebuggerLogWarning("Render already exists.");
					ImGui::CloseCurrentPopup();
				}
				else
				{
					m_inspectedGO->AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
					m_inspectedGO->GetComponent<CPRender>()->Initialise();
					DebuggerLog("Render added!");
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("RigidBody"))
			{
				if (m_inspectedGO->HasComponent<CPRigidBody>())
				{
					DebuggerLogWarning("RigidBody already exists.");
					ImGui::CloseCurrentPopup();
				}
				else
				{
					m_inspectedGO->AddComponent(C_CPRigidBody, FACTORY->GetCMs()[C_CPRigidBody]->Create());
					m_inspectedGO->GetComponent<CPRigidBody>()->Initialise();
					DebuggerLog("RigidBody added!");
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Collider"))
			{
				if (m_inspectedGO->HasComponent<CPCollider>())
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
					m_inspectedGO->AddComponent(C_CPCollider, FACTORY->GetCMs()[C_CPCollider]->Create());
					m_inspectedGO->GetComponent<CPCollider>()->Initialise();
					DebuggerLog("Collider added!");
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("CPP Script"))
			{
				if (m_inspectedGO->HasComponent<CPScriptCPP>())
				{
					DebuggerLogWarning("CPP Script already exists.");
					ImGui::CloseCurrentPopup();
				}
				else
				{
					m_inspectedGO->AddComponent(C_CPScriptCPP, FACTORY->GetCMs()[C_CPScriptCPP]->Create());
					m_inspectedGO->GetComponent<CPScriptCPP>()->Initialise();
					DebuggerLog("CPP Script added!");
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Audio Source"))
			{
				if (m_inspectedGO->HasComponent<CPAudioSource>())
				{
					DebuggerLogWarning("Audio Source already exists.");
					ImGui::CloseCurrentPopup();
				}
				else
				{
					m_inspectedGO->AddComponent(C_CPAudioSource, FACTORY->GetCMs()[C_CPAudioSource]->Create());
					//m_inspectedGO->GetComponent<CPAudioSource>()->Initialise();
					DebuggerLog("Audio Source component Added!");
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Text Component"))
			{
				if (m_inspectedGO->HasComponent<CPText>())
				{
					DebuggerLogWarning("Text Component already exists.");
					ImGui::CloseCurrentPopup();
				}
				else
				{
					m_inspectedGO->AddComponent(C_CPText, FACTORY->GetCMs()[C_CPText]->Create());
					m_inspectedGO->GetComponent<CPText>()->Initialise();
					DebuggerLog("Text component Added!");
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
		//------------------------------------------ADD COMPONENT WINDOW------------------------------------------

		ImGui::Separator();

		//----------------------------------------INSPECT COMPONENTS WINDOW---------------------------------------
		// Individual Component Sections
		ImGui::Text("%-17s", "Name");
		ImGui::SameLine();
		if (ImGui::InputText("##Name", m_inspectedName, 256))
		{
			m_inspectedGO->SetName(m_inspectedName);
		}

		if (m_inspectedGO->HasComponent<CPTransform>())
		{
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				Vec2<float> pos = m_inspectedGO->GetComponent<CPTransform>()->GetPosition();
				ImGui::Text("%-17s X", "Position");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				bool posXChanged = ImGui::DragFloat("##PosX", &pos.x, 1.0f, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				bool posYChanged = ImGui::DragFloat("##PosY", &pos.y, 1.0f, 0.0f, 0.0f, "%.2f");
				if (posXChanged || posYChanged)
				{
					std::shared_ptr<MoveCommand> moveCommand = std::make_shared<MoveCommand>( m_inspectedGO->GetComponent<CPTransform>(), pos );
					COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(moveCommand));
				}

				Vec2<float> scale = m_inspectedGO->GetComponent<CPTransform>()->GetScale();
				ImGui::Text("%-17s X", "Scale");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				bool scaleXChanged = ImGui::DragFloat("##ScaleX", &scale.x, 0.1f, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				bool scaleYChanged = ImGui::DragFloat("##ScaleY", &scale.y, 0.1f, 0.0f, 0.0f, "%.2f");
				if (scaleXChanged || scaleYChanged)
				{
					std::shared_ptr<ScaleCommand> scaleCommand = std::make_shared<ScaleCommand>(m_inspectedGO->GetComponent<CPTransform>(), scale);
					COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(scaleCommand));
				}

				float rotation = m_inspectedGO->GetComponent<CPTransform>()->GetRotation();
				ImGui::Text("%-19s", "Angle");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				bool rotationChanged = ImGui::DragFloat("##Angle", &rotation, 0.1f, 0.0f, 0.0f, "%.3f");
				if (rotationChanged)
				{
					std::shared_ptr<RotateCommand> rotateCommand = std::make_shared<RotateCommand>(m_inspectedGO->GetComponent<CPTransform>(), rotation);
					COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(rotateCommand));
				}

				//DebuggerLogFormat("Snap Mode: %d", EDITORINSPECTOR->GetSnapMode());
				// Choose snapping in translate, rotate and scale
				ImGui::Checkbox("Snap Values?", &EDITORINSPECTOR->ToggleSnapMode());

				if (EDITORINSPECTOR->GetSnapMode())
				{
					if (ImGui::RadioButton("Translate", EDITORINSPECTOR->GetGizmosOperation() == ImGuizmo::TRANSLATE))
					{
						EDITORINSPECTOR->SetGizmosOperation(ImGuizmo::TRANSLATE);
					}
					ImGui::SameLine();
					ImGui::Text("%6s", "X");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##SnapTransX", &EDITORINSPECTOR->SetSnapTranslate(), 1.f, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##SnapTransY", &EDITORINSPECTOR->SetSnapTranslate()+1, 1.f, 0.0f, 0.0f, "%.2f");

					if (EDITORINSPECTOR->GetSnapTranslate() < 0.f)
					{
						EDITORINSPECTOR->SetSnapTranslate() = 0.f;
					}
					if (*(&(EDITORINSPECTOR->SetSnapTranslate()) + 1) < 0.f)
					{
						*(&(EDITORINSPECTOR->SetSnapTranslate()) + 1) = 0.f;
					}

					if (ImGui::RadioButton("Scale", EDITORINSPECTOR->GetGizmosOperation() == ImGuizmo::SCALE))
					{
						EDITORINSPECTOR->SetGizmosOperation(ImGuizmo::SCALE);
					}
					ImGui::SameLine();
					ImGui::Text("%10s", "X");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##SnapScaleX", &EDITORINSPECTOR->SetSnapScale(), 0.1f, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##SnapScaleY", &EDITORINSPECTOR->SetSnapScale() + 1, 0.1f, 0.0f, 0.0f, "%.2f");

					if (EDITORINSPECTOR->GetSnapScale() < 0.f)
					{
						EDITORINSPECTOR->SetSnapScale() = 0.f;
					}
					if (*(&(EDITORINSPECTOR->GetSnapScale()) + 1) < 0.f)
					{
						*(&(EDITORINSPECTOR->SetSnapScale()) + 1) = 0.f;
					}

					if (ImGui::RadioButton("Rotate", EDITORINSPECTOR->GetGizmosOperation() == ImGuizmo::ROTATE))
					{
						EDITORINSPECTOR->SetGizmosOperation(ImGuizmo::ROTATE);
					}
					ImGui::SameLine();
					ImGui::Text("%9s", "Deg");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(extendedWidth);
					ImGui::DragFloat("##SnapRotate", &EDITORINSPECTOR->SetSnapRotate(), 1.f, 0.0f, 0.0f, "%.2f");
					if (EDITORINSPECTOR->GetSnapRotate() < 0.f)
					{
						EDITORINSPECTOR->SetSnapRotate() = 0.f;
					}
					if (EDITORINSPECTOR->GetSnapRotate() > 180.f)
					{
						EDITORINSPECTOR->SetSnapRotate() = 180.f;
					}
				}
				else
				{
					EDITORINSPECTOR->SetGizmosOperation(ImGuizmo::UNIVERSAL);
				}
			}
		}
		if (m_inspectedGO->HasComponent<CPRender>())
		{
			float width = m_inspectedGO->GetComponent<CPRender>()->w;
			float height = m_inspectedGO->GetComponent<CPRender>()->h;
			if (ImGui::CollapsingHeader("Render", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("%-17s Width", "Sprite Size");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				bool widthChanged = ImGui::DragFloat("##SpriteWidth", &width, 1.0f, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ImGui::Text("Height");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				bool heightChanged = ImGui::DragFloat("##SpriteHeight", &height, 1.0f, 0.0f, 0.0f, "%.2f");
				if (widthChanged || heightChanged)
				{
					m_inspectedGO->GetComponent<CPRender>()->w = width;
					m_inspectedGO->GetComponent<CPRender>()->h = height;
				}
				
			/*	if (widthChanged || heightChanged)
				{
					std::shared_ptr<MoveCommand> moveCommand = std::make_shared<MoveCommand>(m_inspectedGO->GetComponent<CPTransform>(), pos);
					COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(moveCommand));
				}*/
				// Interface Buttons
				ImGui::Text("%-19s", "Image");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(dropdownWidth);
				int inspectedTextureID = m_inspectedGO->GetComponent<CPRender>()->texture;
				//This allows you to drag into the text
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* textureData = ImGui::AcceptDragDropPayload("TEXTURE"))
					{
						const char* textureName = (const char*)textureData->Data;
						m_inspectedGO->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second, ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->width, ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->height);
					}
				}
				if (ImGui::BeginCombo("##Texture", ASSETMANAGER->GetTextureName(inspectedTextureID).c_str()))
				{
					for (auto& [str, tex] : ASSETMANAGER->Textures)
					{
						std::filesystem::path tempPath{ str };
						if (ImGui::Selectable(tempPath.filename().stem().string().c_str()))
						{
							m_inspectedGO->GetComponent<CPRender>()->UpdateTexture(tex.second, width, height);
						}
					}
					ImGui::EndCombo();
				}
				//This allows you to drag into the combo
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* textureData = ImGui::AcceptDragDropPayload("TEXTURE"))
					{
						const char* textureName = (const char*)textureData->Data;
						m_inspectedGO->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second, ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->width, ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->height);
					}
				}
				// Delete Component
				if (ImGui::Button("Delete Render Component"))
				{
					m_inspectedGO->RemoveComponent(C_CPRender);
				}
			}
		}
		if (m_inspectedGO->HasComponent<CPRigidBody>())
		{
			if (ImGui::CollapsingHeader("RigidBody", ImGuiTreeNodeFlags_DefaultOpen))
			{
				// Interface Buttons
				float mass = m_inspectedGO->GetComponent<CPRigidBody>()->mMass;
				ImGui::Text("%-19s", "Mass");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				ImGui::DragFloat("##Mass", &mass, 1.0f, 0.0f, 0.0f, "%.2f");

				if (mass <= 0.f)
				{
					m_inspectedGO->GetComponent<CPRigidBody>()->mMass = 0.1f;
				}
				else
				{
					m_inspectedGO->GetComponent<CPRigidBody>()->mMass = mass;
				}

				float restitution = m_inspectedGO->GetComponent<CPRigidBody>()->mRestitution;
				ImGui::Text("%-19s", "Restitution");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				ImGui::DragFloat("##Restitution", &restitution, 1.0f, 0.0f, 0.0f, "%.2f");

				if (restitution <= 0.f)
				{
					m_inspectedGO->GetComponent<CPRigidBody>()->mRestitution = 0.f;
				}
				else if (restitution >= 1.f)
				{
					m_inspectedGO->GetComponent<CPRigidBody>()->mRestitution = 1.f;
				}
				else
				{
					m_inspectedGO->GetComponent<CPRigidBody>()->mRestitution = restitution;
				}


				float friction = m_inspectedGO->GetComponent<CPRigidBody>()->mFriction;
				ImGui::Text("%-19s", "Friction");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				ImGui::DragFloat("##Friction", &friction, 1.0f, 0.0f, 0.0f, "%.2f");

				if (friction <= 0.f)
				{
					m_inspectedGO->GetComponent<CPRigidBody>()->mFriction = 0.f;
				}
				else if (friction >= 1.f)
				{
					m_inspectedGO->GetComponent<CPRigidBody>()->mFriction = 1.f;
				}
				else
				{
					m_inspectedGO->GetComponent<CPRigidBody>()->mFriction = friction;
				}

				Vec2<float> velocity = m_inspectedGO->GetComponent<CPRigidBody>()->mVelocity;
				ImGui::Text("%-17s X", "Velocity");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##VelocityX", &velocity.x, 1.0f, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##VelocityY", &velocity.y, 1.0f, 0.0f, 0.0f, "%.2f");

				Vec2<float> acceleration = m_inspectedGO->GetComponent<CPRigidBody>()->mAcceleration;
				ImGui::Text("%-17s X", "Acceleration");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##AccelerationX", &acceleration.x, 1.0f, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##AccelerationY", &acceleration.y, 1.0f, 0.0f, 0.0f, "%.2f");

				ImGui::Text("%-10s", "Is Static");
				ImGui::SameLine();
				ImGui::Checkbox("##isStatic", &m_inspectedGO->GetComponent<CPRigidBody>()->isStatic);

				// Delete Component
				if (ImGui::Button("Delete RigidBody Component"))
				{
					m_inspectedGO->RemoveComponent(C_CPRigidBody);
				}
			}
		}
		if (m_inspectedGO->HasComponent<CPCollider>())
		{
			if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_DefaultOpen))
			{
				// Interface Buttons
				ImGui::Text("%-19s", "Layer");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(dropdownWidth);
				if (ImGui::BeginCombo("##Layer", m_inspectedGO->GetComponent<CPCollider>()->GetLayerName().c_str()))
				{

					for (auto& [str, type] : COLLIDERS->GetLayerSystem().GetLayerVector())
					{
						if (ImGui::Selectable(str.c_str()))
						{
							m_inspectedGO->GetComponent<CPCollider>()->m_collisionlayer = type;
						}
					}
					ImGui::EndCombo();
				}


				float width = m_inspectedGO->GetComponent<CPCollider>()->m_widthUnscaled;
				ImGui::Text("%-19s", "Width");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				ImGui::DragFloat("##Width", &width, 1.0f, 0.0f, 0.0f, "%.2f");

				if (width < 0.f)
				{
					m_inspectedGO->GetComponent<CPCollider>()->m_widthUnscaled = 0.f;
				}
				else
				{
					m_inspectedGO->GetComponent<CPCollider>()->m_widthUnscaled = width;
				}

				float height = m_inspectedGO->GetComponent<CPCollider>()->m_heightUnscaled;
				ImGui::Text("%-19s", "Height");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				ImGui::DragFloat("##Height", &height, 1.0f, 0.0f, 0.0f, "%.2f");

				if (height < 0.f)
				{
					m_inspectedGO->GetComponent<CPCollider>()->m_heightUnscaled = 0.f;
				}
				else
				{
					m_inspectedGO->GetComponent<CPCollider>()->m_heightUnscaled = height;
				}

				float radius = m_inspectedGO->GetComponent<CPCollider>()->m_radiusUnscaled;
				ImGui::Text("%-19s", "Radius");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				ImGui::DragFloat("##Radius", &radius, 1.0f, 0.0f, 0.0f, "%.2f");

				if (radius < 0.f)
				{
					m_inspectedGO->GetComponent<CPCollider>()->m_radiusUnscaled = 0.f;
				}
				else
				{
					m_inspectedGO->GetComponent<CPCollider>()->m_radiusUnscaled = radius;
				}

				ImGui::Text("%-19s", "Shape Type");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(dropdownWidth);
				if (ImGui::BeginCombo("##ShapeType", m_inspectedGO->GetComponent<CPCollider>()->GetShapeName().c_str()))
				{

					for (auto& [str, type] : COLLIDERS->m_shapeTypes)
					{
						if (ImGui::Selectable(str.c_str()))
						{
							m_inspectedGO->GetComponent<CPCollider>()->m_shape = type;
						}
					}
					ImGui::EndCombo();
				}

				if (ImGui::Button("Add Vertice"))
				{
					m_inspectedGO->GetComponent<CPCollider>()->AddVertice(Vec2<float>{0.f,0.f});
				}

				char labelX[50]{};
				char labelY[50]{};
				for (size_t i{}; i < m_inspectedGO->GetComponent<CPCollider>()->m_untransformedVerts.size(); ++i)
				{
					Vec2<float> untransformedVerts = m_inspectedGO->GetComponent<CPCollider>()->m_untransformedVerts[i];
					ImGui::Text("%s %-8d", "Vertice", i);
					ImGui::SameLine();
					
					ImGui::Text("X");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					sprintf_s(labelX, "##untransformedVertsX%d", static_cast<int>(i));
					ImGui::DragFloat(labelX, &untransformedVerts.x, 1.0f, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();

					m_inspectedGO->GetComponent<CPCollider>()->m_untransformedVerts[i].x = untransformedVerts.x;

					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					sprintf_s(labelY, "##untransformedVertsY%d", static_cast<int>(i));
					ImGui::DragFloat(labelY, &untransformedVerts.y, 1.0f, 0.0f, 0.0f, "%.2f");

					m_inspectedGO->GetComponent<CPCollider>()->m_untransformedVerts[i].y = untransformedVerts.y;

				}

				// Delete Component
				if (ImGui::Button("Delete Collider Component"))
				{
					m_inspectedGO->RemoveComponent(C_CPCollider);
				}
			}
		}
		if (m_inspectedGO->HasComponent<CPScriptCPP>())
		{
			if (ImGui::CollapsingHeader("CPP Script", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("%-17s", "Script Name");
				ImGui::SameLine();
				strcpy_s(m_nameBuffer1, sizeof(m_nameBuffer1), m_inspectedGO->GetComponent<CPScriptCPP>()->GetName().c_str());
				if (ImGui::InputText("##ScriptName", m_nameBuffer1, 256))
				{
					m_inspectedGO->GetComponent<CPScriptCPP>()->SetName(m_nameBuffer1);
				}

				// Delete Component
				if (ImGui::Button("Delete CPP Script Component"))
				{
					m_inspectedGO->RemoveComponent(C_CPScriptCPP);
				}
			}
		}
		
		if (m_inspectedGO->HasComponent<CPAudioSource>())
		{
			if (ImGui::CollapsingHeader("Audio Source Component", ImGuiTreeNodeFlags_DefaultOpen))
			{
				// Interface Buttons
				ImGui::Text("%-19s", "Audio Clip Name");
				ImGui::SameLine();
				//This is for dragging into the text thing
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* audioData = ImGui::AcceptDragDropPayload("AUDIO"))
					{
						const char* audioClipName = reinterpret_cast<const char*>(audioData->Data);
						//DebuggerLogWarningFormat("clip name : %s", std::string(audioClipName).c_str());
						m_inspectedGO->GetComponent<CPAudioSource>()->UpdateAudio(std::string(audioClipName));

					}
				}
				ImGui::SetNextItemWidth(dropdownWidth);
				std::string inspectedAudioClipName = m_inspectedGO->GetComponent<CPAudioSource>()->AudioClipName;
				if (ImGui::BeginCombo("##AudioClips", inspectedAudioClipName.c_str()))
				{
					for (auto& [str, clip] : ASSETMANAGER->SoundMap)
					{
						if (ImGui::Selectable(str.c_str()))
						{
							m_inspectedGO->GetComponent<CPAudioSource>()->UpdateAudio(str);
						}
					}
					ImGui::EndCombo();
				}
				//This is for dragging into the combo
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* audioData = ImGui::AcceptDragDropPayload("AUDIO"))
					{
						const char* audioClipName = reinterpret_cast<const char*>(audioData->Data);
						//DebuggerLogWarningFormat("clip name : %s", std::string(audioClipName).c_str());
						m_inspectedGO->GetComponent<CPAudioSource>()->UpdateAudio(std::string(audioClipName));
						
					}
				}
				ImGui::Text("%-19s", "Play On Awake");
				ImGui::SameLine();
				ImGui::Checkbox("##PlayOnAwake", &m_inspectedGO->GetComponent<CPAudioSource>()->playOnAwake);

				ImGui::Text("%-19s", "Loop");
				ImGui::SameLine();
				ImGui::Checkbox("##Loop", &m_inspectedGO->GetComponent<CPAudioSource>()->loop);

				float vol = m_inspectedGO->GetComponent<CPAudioSource>()->volume;
				ImGui::Text("%-19s", "Volume");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				ImGui::DragFloat("##Volume", &vol, 0.01f, 0.0f, 1.0f, "%.2f");
				if (vol < 0.f)
				{
					m_inspectedGO->GetComponent<CPAudioSource>()->SetVolume(0.0f);
				}
				else
				{
					m_inspectedGO->GetComponent<CPAudioSource>()->SetVolume(vol);
				}

				float pitch = m_inspectedGO->GetComponent<CPAudioSource>()->pitch;
				ImGui::Text("%-19s", "Pitch");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(extendedWidth);
				ImGui::DragFloat("##Pitch", &pitch, 0.01f, -10.0f, 10.0f, "%.2f");

				if (pitch < -10.f)
				{
					m_inspectedGO->GetComponent<CPAudioSource>()->SetPitch(-10.f);
				}
				else
				{
					m_inspectedGO->GetComponent<CPAudioSource>()->SetPitch(pitch);
				}


				// Delete Component
				if (ImGui::Button("Delete Audio Source Component"))
				{
					m_inspectedGO->RemoveComponent(C_CPAudioSource);
				}
			}
		}
		if (m_inspectedGO->HasComponent<CPText>())
		{
			if (ImGui::CollapsingHeader("Text Component", ImGuiTreeNodeFlags_DefaultOpen))
			{
				float textXPos = m_inspectedGO->GetComponent<CPText>()->get_msg().x;
				float textYPos = m_inspectedGO->GetComponent<CPText>()->get_msg().y;
				float textSize = m_inspectedGO->GetComponent<CPText>()->get_msg().scale;
				ImGui::Text("%-17s X", "Position");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##TextPosX", &textXPos, 1.0f, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##TextPosY", &textYPos, 1.0f, 0.0f, 0.0f, "%.2f");

				ImGui::Text("%-17s", "Scale");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##TextSize", &textSize, 1.0f, 0.0f, 0.0f, "%.2f");
				//m_inspectedGO->GetComponent<CPText>()->update_msg_pos(Vec2<float>(textXPos, textYPos));
				//m_inspectedGO->GetComponent<CPText>()->update_msg_size(textSize);

				m_inspectedGO->GetComponent<CPText>()->get_msg().x = textXPos;
				m_inspectedGO->GetComponent<CPText>()->get_msg().y = textYPos;
				m_inspectedGO->GetComponent<CPText>()->get_msg().scale = textSize;

				ImGui::Text("%-19s", "Text");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(dropdownWidth);
				if (m_inspectedGO) 
				strcpy_s(m_textBuffer, sizeof(m_textBuffer), m_inspectedGO->GetComponent<CPText>()->get_msg().text.c_str());
				if (ImGui::InputText("##TextMessage", m_textBuffer, 256))
				{
					m_inspectedGO->GetComponent<CPText>()->set_msg(m_textBuffer);
				}
				Vec3<float> col = m_inspectedGO->GetComponent<CPText>()->get_msg().color;
				static ImVec4 color = ImVec4(col.x, col .y,col.z, 1.f);
				ImGui::Text("Text Color:");
				ImGui::SameLine();
				ImGui::ColorEdit3("##TextColor", (float*)&color);
				col = Vec3<float>(color.x, color.y, color.z);
				//m_inspectedGO->GetComponent<CPText>()->update_msg_color(col);
				m_inspectedGO->GetComponent<CPText>()->get_msg().color = col;
				ImGui::Text("%-19s", "Font Name");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(dropdownWidth);
				std::string inspectedFontName = m_inspectedGO->GetComponent<CPText>()->get_msg().font_file_name_wo_ext;
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* fontData = ImGui::AcceptDragDropPayload("FONT"))
					{
						const char* fontName = (const char*)fontData->Data;
						m_inspectedGO->GetComponent<CPText>()->get_msg().font_file_name_wo_ext = fontName;
						//m_inspectedGO->GetComponent<CPText>()->update_msg_font(fontName);
					}
				}
				if (ImGui::BeginCombo("##FontName", inspectedFontName.c_str()))
				{
					for (auto& [str, fp] : ASSETMANAGER->assetMap)
					{
						std::filesystem::path p{ fp };
						if (p.extension() == ".otf" || p.extension() == ".ttf")
						{
							if (ImGui::Selectable(str.c_str()))
							{
								//m_inspectedGO->GetComponent<CPText>()->update_msg_font(str);
								m_inspectedGO->GetComponent<CPText>()->get_msg().font_file_name_wo_ext = str;

							}
						}
					}
					ImGui::EndCombo();
				}
				// Delete Component
				if (ImGui::Button("Delete Text Component"))
				{
					m_inspectedGO->RemoveComponent(C_CPText);
				}


			}
			
		}
		//----------------------------------------INSPECT COMPONENTS WINDOW---------------------------------------

		ImGui::End();
	}

	/*!***********************************************************************
	  \brief
	  Update the currently inspected game object.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorInspector::UpdateInspectedGO(GameObject* newInspectedGO)
	{
		if (isPrefab && m_inspectedGO)
		{
			GOMANAGER->RemoveDDOLGameObject(m_inspectedGO);
			isPrefab = false;
		}

		m_inspectedGO = newInspectedGO;

		// If GO was deselected, don't update name
		if (!newInspectedGO) return;
		strcpy_s(m_inspectedName, sizeof(m_inspectedName), newInspectedGO->GetName().c_str());
	}

	/*!***********************************************************************
	  \brief
	  Get the currently inspected game object.
	  \return
	  A pointer to the currently inspected GameObject.
	*************************************************************************/
	GameObject* EditorInspector::GetInspectedGO()
	{
		return m_inspectedGO;
	}

	/*!***********************************************************************
	  \brief
	  Check if a game object is currently being inspected.
	  \return
	  True if a game object is being inspected, false otherwise.
	*************************************************************************/
	bool EditorInspector::IsGOInspected()
	{
		return m_inspectedGO != nullptr;
	}

	// For event subscription
	/*!***********************************************************************
	  \brief
	  To get the original function, UpdateInspectedGO called as an event.
	  \return
	  Nothing.
	*************************************************************************/
	void UpdateInspectedGO(GameObject* newInspectedGO)
	{
		EDITORINSPECTOR->UpdateInspectedGO(newInspectedGO);
	}

	/*!***********************************************************************
	  \brief
	  To get the original function, DeselectObject called as an event.
	  \return
	  Nothing.
	*************************************************************************/
	void DeselectObject(bool isPlaying)
	{
		if (!isPlaying)
			EDITORINSPECTOR->UpdateInspectedGO(nullptr);
	}

	/*!***********************************************************************
	  \brief
	  To get the original function, DeselectObject called as an event, overloaded.
	  \return
	  Nothing.
	*************************************************************************/
	void DeselectObject(Scene* newScene)
	{
		UNREFERENCED_PARAMETER(newScene);
		EDITORINSPECTOR->UpdateInspectedGO(nullptr);
	}

	/*!***********************************************************************
	  \brief
	  Get the current gizmos mode.
	  \return
	  Nothing.
	*************************************************************************/
	ImGuizmo::MODE EditorInspector::GetGizmosMode() const
	{
		return m_CurrentGizmosMode;
	}

	/*!***********************************************************************
	  \brief
	  Sets the current gizmos mode.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorInspector::SetGizmosMode(ImGuizmo::MODE mode)
	{
		m_CurrentGizmosMode = mode;
	}

	/*!***********************************************************************
	  \brief
	  Get the current gizmos operation.
	  \return
	  Nothing.
	*************************************************************************/
	ImGuizmo::OPERATION EditorInspector::GetGizmosOperation() const
	{
		return m_CurrentGizmosOperation;
	}

	void EditorInspector::SetGizmosOperation(ImGuizmo::OPERATION operation)
	{
		m_CurrentGizmosOperation = operation;
	}

	/*!***********************************************************************
	  \brief
	  Gets the snap mode.
	  \return
	  Nothing.
	*************************************************************************/
	bool const& EditorInspector::GetSnapMode() const
	{
		return m_SnapMode;
	}

	/*!***********************************************************************
	  \brief
	  Toggles snap mode.
	  \return
	  Nothing.
	*************************************************************************/
	bool& EditorInspector::ToggleSnapMode()
	{
		return m_SnapMode;
	}

	/*!***********************************************************************
	  \brief
	  Get the snap value of translate.
	  \return
	  Nothing.
	*************************************************************************/
	float const& EditorInspector::GetSnapTranslate() const
	{
		return m_SnapTranslate[0];
	}

	/*!***********************************************************************
	  \brief
	  Get the snap value of rotate.
	  \return
	  Nothing.
	*************************************************************************/
	float const& EditorInspector::GetSnapRotate() const
	{
		return m_SnapRotate;
	}

	/*!***********************************************************************
	  \brief
	  Get the snap value of scale.
	  \return
	  Nothing.
	*************************************************************************/
	float const& EditorInspector::GetSnapScale() const
	{
		return m_SnapScale[0];
	}

	/*!***********************************************************************
	  \brief
	  Set the snap value of translate through ImGui.
	  \return
	  Nothing.
	*************************************************************************/
	float& EditorInspector::SetSnapTranslate()
	{
		return m_SnapTranslate[0];
	}

	/*!***********************************************************************
	  \brief
	  Set the snap value of rotate through ImGui.
	  \return
	  Nothing.
	*************************************************************************/
	float& EditorInspector::SetSnapRotate()
	{
		return m_SnapRotate;
	}

	/*!***********************************************************************
	  \brief
	  Set the snap value of scale through ImGui.
	  \return
	  Nothing.
	*************************************************************************/
	float& EditorInspector::SetSnapScale()
	{
		return m_SnapScale[0];
	}

}
