/*!************************************************************************
 \file				InspectorGameObject.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains 

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "InspectorGameObject.h"
#include "Editor/EditorInspector.h"
#include "Editor/EditorHierarchy.h"
#include "Editor/EditorAssets.h"

#include "LitterBox/Core/Core.h"
#include "LitterBox/GameLogic/CPPGameLogic/CPPGameLogic.h"
#include "Utils/CommandManager.h"

// All the commands
#include "Commands/GameObjectCommands.h"
#include "Commands/TransformCommands.h"
#include "Commands/RenderCommands.h"

#include "LitterBox/Components/RenderComponent.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Components/RigidBodyComponent.h"
#include "LitterBox/Components/TransformComponent.h"
#include "LitterBox/Components/AudioSourceComponent.h"
#include "LitterBox/Components/AnimatorComponent.h"
#include "LitterBox/Components/ParticleComponent.h"
#include "LitterBox/Animation/ParticleSystem.h"

namespace LB
{
	std::shared_ptr<InspectorGameObject> InspectorGO;

	/*!***********************************************************************
	  \brief
	  Initializes the EditorInspector layer.
	  \return
	  Nothing.
	*************************************************************************/
	void InspectorGameObject::Initialize()
	{
		EDITORHIERACHY->onNewObjectSelected.Subscribe(LB::UpdateInspectedGO);
		CORE->onPlayingModeToggle.Subscribe(LB::DeselectObject);
		SCENEMANAGER->onNewSceneLoad.Subscribe(LB::DeselectObject);

		InspectorGO = InspectorGameObject::Instance();

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
	void InspectorGameObject::UpdateLayer()
	{
		// If no game object is selected, don't render the inspector (as there's nothing to inspect)
		if (!m_inspectedGO) return;

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
				if (std::filesystem::exists(FILESYSTEM->GetFilePath(m_inspectedGO->GetName() + ".prefab")))
				{
					DebuggerLogWarning("Prefab with that name already exists! Use a different name!");
				}
				else
				{
					DebuggerLog("Creating Prefab!");
					std::filesystem::path prefab("Prefabs");
					std::filesystem::path assetFileName(GetInspectedGO()->GetName());
					JSONSerializer::SerializeToFile((EDITORASSETS->defaultDirectory / prefab / assetFileName).string() + ".prefab", *GetInspectedGO());
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
			if (ImGui::MenuItem("Audio Listener"))
			{
				if (m_inspectedGO->HasComponent<CPAudioListener>())
				{
					DebuggerLogWarning("Audio Listener already exists.");
					ImGui::CloseCurrentPopup();
				}
				else
				{
					m_inspectedGO->AddComponent(C_CPAudioListener, FACTORY->GetCMs()[C_CPAudioListener]->Create());
					//m_inspectedGO->GetComponent<CPAudioSource>()->Initialise();
					DebuggerLog("Audio Listener component Added!");
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

			ImGui::Separator();
			if (ImGui::MenuItem("Animator Component"))
			{
				if (m_inspectedGO->HasComponent<CPAnimator>())
				{
					DebuggerLogWarning("Animator Component already exists.");
					ImGui::CloseCurrentPopup();
				}
				else
				{
					m_inspectedGO->AddComponent(C_CPAnimator, FACTORY->GetCMs()[C_CPAnimator]->Create());
					m_inspectedGO->GetComponent<CPAnimator>()->Initialise();
					DebuggerLog("Animator component Added!");
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Particle Component"))
			{
				if (m_inspectedGO->HasComponent<CPParticle>())
				{
					DebuggerLogWarning("Particle Component already exists.");
					ImGui::CloseCurrentPopup();
				}
				else
				{
					m_inspectedGO->AddComponent(C_CPParticle, FACTORY->GetCMs()[C_CPParticle]->Create());
					m_inspectedGO->GetComponent<CPParticle>()->Initialise();
					DebuggerLog("Particle component Added!");
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
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.8f);
		if (ImGui::InputText("##Name", m_inspectedName, 256))
		{
			m_inspectedGO->SetName(m_inspectedName);
		}
		ImGui::SameLine();
		bool isActive = m_inspectedGO->IsActive();
		ImGui::Checkbox("Active", &isActive);
		if (isActive != m_inspectedGO->IsActive())
		{
			m_inspectedGO->SetActive(isActive);
		}

		if (m_inspectedGO->HasComponent<CPTransform>())
		{
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				Vec2<float> pos = m_inspectedGO->GetComponent<CPTransform>()->GetLocalPosition();
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
					std::shared_ptr<MoveCommand> moveCommand = std::make_shared<MoveCommand>(m_inspectedGO->GetComponent<CPTransform>(), pos);
					COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(moveCommand));
				}

				Vec2<float> scale = m_inspectedGO->GetComponent<CPTransform>()->GetLocalScale();
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

				float rotation = m_inspectedGO->GetComponent<CPTransform>()->GetLocalRotation();
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
				ImGui::Checkbox("Snap Values?", &InspectorGameObject::Instance()->ToggleSnapMode());

				if (InspectorGameObject::Instance()->GetSnapMode())
				{
					if (ImGui::RadioButton("Translate", InspectorGO->GetGizmosOperation() == ImGuizmo::TRANSLATE))
					{
						InspectorGO->SetGizmosOperation(ImGuizmo::TRANSLATE);
					}
					ImGui::SameLine();
					ImGui::Text("%6s", "X");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##SnapTransX", &InspectorGO->SetSnapTranslate(), 1.f, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##SnapTransY", &InspectorGO->SetSnapTranslate() + 1, 1.f, 0.0f, 0.0f, "%.2f");

					if (InspectorGO->GetSnapTranslate() < 0.f)
					{
						InspectorGO->SetSnapTranslate() = 0.f;
					}
					if (*(&(InspectorGO->SetSnapTranslate()) + 1) < 0.f)
					{
						*(&(InspectorGO->SetSnapTranslate()) + 1) = 0.f;
					}

					if (ImGui::RadioButton("Scale", InspectorGO->GetGizmosOperation() == ImGuizmo::SCALE))
					{
						InspectorGO->SetGizmosOperation(ImGuizmo::SCALE);
					}
					ImGui::SameLine();
					ImGui::Text("%10s", "X");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##SnapScaleX", &InspectorGO->SetSnapScale(), 0.1f, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##SnapScaleY", &InspectorGO->SetSnapScale() + 1, 0.1f, 0.0f, 0.0f, "%.2f");

					if (InspectorGO->GetSnapScale() < 0.f)
					{
						InspectorGO->SetSnapScale() = 0.f;
					}
					if (*(&(InspectorGO->GetSnapScale()) + 1) < 0.f)
					{
						*(&(InspectorGO->SetSnapScale()) + 1) = 0.f;
					}

					if (ImGui::RadioButton("Rotate", InspectorGO->GetGizmosOperation() == ImGuizmo::ROTATE))
					{
						InspectorGO->SetGizmosOperation(ImGuizmo::ROTATE);
					}
					ImGui::SameLine();
					ImGui::Text("%9s", "Deg");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(extendedWidth);
					ImGui::DragFloat("##SnapRotate", &InspectorGO->SetSnapRotate(), 1.f, 0.0f, 0.0f, "%.2f");
					if (InspectorGO->GetSnapRotate() < 0.f)
					{
						InspectorGO->SetSnapRotate() = 0.f;
					}
					if (InspectorGO->GetSnapRotate() > 180.f)
					{
						InspectorGO->SetSnapRotate() = 180.f;
					}
				}
				else
				{
					InspectorGO->SetGizmosOperation(ImGuizmo::UNIVERSAL);
				}
			}
		}
		if (m_inspectedGO->HasComponent<CPRender>())
		{
			float width = m_inspectedGO->GetComponent<CPRender>()->w;
			float height = m_inspectedGO->GetComponent<CPRender>()->h;
			float z = m_inspectedGO->GetComponent<CPRender>()->z_val;
			if (ImGui::CollapsingHeader("Render", ImGuiTreeNodeFlags_DefaultOpen))
			{
				isActive = m_inspectedGO->GetComponent<CPRender>()->m_active;
				ImGui::PushID("RenderActive");
				ImGui::Checkbox("Active", &isActive);
				ImGui::PopID();
				if (isActive != m_inspectedGO->GetComponent<CPRender>()->m_active)
				{
					m_inspectedGO->GetComponent<CPRender>()->ToggleActiveFlag(isActive);
				}

				ImGui::Text("%-17s Width", "Sprite Size");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				bool widthChanged = ImGui::DragFloat("##SpriteWidth", &width, 1.0f, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ImGui::Text("Height");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				bool heightChanged = ImGui::DragFloat("##SpriteHeight", &height, 1.0f, 0.0f, 0.0f, "%.2f");

				//for z value
				ImGui::SameLine();
				ImGui::Text("z-axis");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				bool zChanged = ImGui::DragFloat("##z-axis", &z, 1.0f, 0.0f, 0.0f, "%.2f");

				if (widthChanged || heightChanged || zChanged)
				{
					std::shared_ptr<RenderPosCommand> renderPosCommand = std::make_shared<RenderPosCommand>(m_inspectedGO->GetComponent<CPRender>(), Vec3<float>{width, height, z});
					COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(renderPosCommand));
					//m_inspectedGO->GetComponent<CPRender>()->w = width;
					//m_inspectedGO->GetComponent<CPRender>()->h = height;
					//m_inspectedGO->GetComponent<CPRender>()->z_val = z;
				}

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
							std::shared_ptr<RenderImageCommand> renderImgCommand = std::make_shared<RenderImageCommand>(m_inspectedGO->GetComponent<CPRender>(), tex.second);
							COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(renderImgCommand));
							/*if (str == "none") m_inspectedGO->GetComponent<CPRender>()->UpdateTexture(-1, static_cast<int>(width), static_cast<int>(height));
							else m_inspectedGO->GetComponent<CPRender>()->UpdateTexture(tex.second, static_cast<int>(width), static_cast<int>(height));*/
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


				//-------------For sprite sheet---------------
				ImGui::Text("%-19s", "SpriteSheet");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(dropdownWidth);
				std::string& ssheetName = m_inspectedGO->GetComponent<CPRender>()->spriteSheetName;
				//int inspectedTextureID = m_inspectedGO->GetComponent<CPRender>()->texture;
				//This allows you to drag into the text
				/*if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* spritesheet = ImGui::AcceptDragDropPayload("SPRITE SHEET"))
					{
						const char* textureName = (const char*)spritesheet->Data;
						m_inspectedGO->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second, ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->width, ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->height);
					}
				}*/
				if (ImGui::BeginCombo("##Sprite Sheet", ssheetName.c_str())) 
				{
					for (auto& [str, sheet] : ASSETMANAGER->SpriteSheets)
					{
						std::filesystem::path tempPath{ sheet.GetName() };
						if (ImGui::Selectable(tempPath.filename().stem().string().c_str()))
						{
							std::shared_ptr<RenderSpriteSheetCommand> renderSSheetCommand = std::make_shared<RenderSpriteSheetCommand>(m_inspectedGO->GetComponent<CPRender>(), str);
							COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(renderSSheetCommand));
							//ssheetName = str;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine();
				if (ImGui::Button("X")) // To deselect the sprite sheet
				{
					ssheetName = "None";
				}
				if (ssheetName != "None")
				{
					ImGui::SameLine();
					ImGui::SetNextItemWidth(dropdownWidth);
					if (ImGui::BeginCombo("##Sprite Sheet tile", std::to_string(m_inspectedGO->GetComponent<CPRender>()->spriteIndex).c_str()))
					{
						for (auto& tile : ASSETMANAGER->GetSpriteSheet(ssheetName).Sprites())
						{
							if (ImGui::Selectable(std::to_string(tile.m_index).c_str()))
							{
								std::shared_ptr<RenderSpriteSheetLayerCommand> renderSSheetCommand = std::make_shared<RenderSpriteSheetLayerCommand>(m_inspectedGO->GetComponent<CPRender>(), tile.m_index);
								COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(renderSSheetCommand));
								/*m_inspectedGO->GetComponent<CPRender>()->spriteIndex = tile.m_index;
								m_inspectedGO->GetComponent<CPRender>()->SetSpriteTexture(ssheetName, tile.m_index);*/
							}
						}
						ImGui::EndCombo();
					}
				}
				////This allows you to drag into the combo
				//if (ImGui::BeginDragDropTarget())
				//{
				//	if (const ImGuiPayload* textureData = ImGui::AcceptDragDropPayload("TEXTURE"))
				//	{
				//		const char* textureName = (const char*)textureData->Data;
				//		m_inspectedGO->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second, ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->width, ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->height);
				//	}
				//}
				//-------------For sprite sheet---------------

				//store the name of the layer enum
				static std::string renderdroplayer{};

				//this is the layer selector
				ImGui::Text("%-19s", "Layer");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(dropdownWidth);

				//create the string based on the layer enum
				switch (m_inspectedGO->GetComponent<CPRender>()->get_r_type())
				{
				case Renderer::Renderer_Types::RT_OBJECT:
					renderdroplayer = "Object layer";
					break;
				case Renderer::Renderer_Types::RT_BACKGROUND:
					renderdroplayer = "Background layer";
					break;
				case Renderer::Renderer_Types::RT_UI:
					renderdroplayer = "UI layer";
					break;
				case Renderer::Renderer_Types::RT_MENU:
					renderdroplayer = "Menu layer";
					break;
				}

				//TODO: Add ability to get c string based on render layer
				if (ImGui::BeginCombo("##Layer", renderdroplayer.c_str()))
				{
					//loop through each enum
					for (auto layer{ Renderer::Renderer_Types::RT_OBJECT };
						layer != Renderer::Renderer_Types::Last;
						++layer)
					{
						//skip the debug enum
						if (layer == Renderer::Renderer_Types::RT_DEBUG)
							continue;

						//get the string for the layer
						switch (layer)
						{
						case Renderer::Renderer_Types::RT_OBJECT:
							renderdroplayer = "Object layer";
							break;
						case Renderer::Renderer_Types::RT_BACKGROUND:
							renderdroplayer = "Background layer";
							break;
						case Renderer::Renderer_Types::RT_UI:
							renderdroplayer = "UI layer";
							break;
						case Renderer::Renderer_Types::RT_MENU:
							renderdroplayer = "Menu layer";
							break;
						}

						if (ImGui::Selectable(renderdroplayer.c_str()))
						{
							Renderer::GRAPHICS->swap_object_type(layer, m_inspectedGO->GetComponent<CPRender>());
						}
					}
					ImGui::EndCombo();
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
				isActive = m_inspectedGO->GetComponent<CPRigidBody>()->m_active;
				ImGui::PushID("RBActive");
				ImGui::Checkbox("Active", &isActive);
				ImGui::PopID();
				if (isActive != m_inspectedGO->GetComponent<CPRigidBody>()->m_active)
				{
					m_inspectedGO->GetComponent<CPRigidBody>()->ToggleActiveFlag(isActive);
				}

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
				bool& isColActive = m_inspectedGO->GetComponent<CPCollider>()->m_active;
				ImGui::PushID("ColliderActive");
				ImGui::Checkbox("Active", &isColActive);
				ImGui::PopID();
				if (isColActive != m_inspectedGO->GetComponent<CPCollider>()->m_active)
				{
					m_inspectedGO->GetComponent<CPCollider>()->ToggleActiveFlag(isColActive);
				}

				// Interface Buttons
				ImGui::Text("%-19s", "Layer");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(dropdownWidth);
				if (ImGui::BeginCombo("##CollisionLayer", m_inspectedGO->GetComponent<CPCollider>()->GetLayerName().c_str()))
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
				/*
				ImGui::Text("%-19s", "Grid");
				for (size_t grids = 0; grids < m_inspectedGO->GetComponent<CPCollider>()->GetGridFrames().size(); ++grids)
				{
					ImGui::Text("%d", m_inspectedGO->GetComponent<CPCollider>()->GetGridFrames()[grids]);
				}
				*/
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
					m_inspectedGO->GetComponent<CPCollider>()->AddVertice(Vec2<float>{0.f, 0.f});
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
				isActive = m_inspectedGO->GetComponent<CPScriptCPP>()->m_active;
				ImGui::PushID("CPPScriptActive");
				ImGui::Checkbox("Active", &isActive);
				ImGui::PopID();
				if (isActive != m_inspectedGO->GetComponent<CPScriptCPP>()->m_active)
				{
					m_inspectedGO->GetComponent<CPScriptCPP>()->ToggleActiveFlag(isActive);
				}

				ImGui::Text("%-17s", "Script Name");
				ImGui::SameLine();

				if (ImGui::BeginCombo("##ScriptCombo", m_inspectedGO->GetComponent<CPScriptCPP>()->GetScriptType().name()))
				{
					for (auto const& script : CPPGameLogic::Instance()->GetRegistry())
					{
						if (ImGui::Selectable(script.first.name()))
						{
							m_inspectedGO->GetComponent<CPScriptCPP>()->SetScriptType(script.first);
						}
					}
					ImGui::EndCombo();
				}

				//strcpy_s(m_nameBuffer1, sizeof(m_nameBuffer1), m_inspectedGO->GetComponent<CPScriptCPP>()->GetName().c_str());
				//if (ImGui::InputText("##ScriptName", m_nameBuffer1, 256))
				//{
				//	m_inspectedGO->GetComponent<CPScriptCPP>()->SetName(m_nameBuffer1);
				//}

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
				isActive = m_inspectedGO->GetComponent<CPAudioSource>()->m_active;
				ImGui::PushID("AudioActive");
				ImGui::Checkbox("Active", &isActive);
				ImGui::PopID();
				if (isActive != m_inspectedGO->GetComponent<CPAudioSource>()->m_active)
				{
					m_inspectedGO->GetComponent<CPAudioSource>()->ToggleActiveFlag(isActive);
				}

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
				//store the name of the layer enum
				static std::string soundTypeName{};

				//this is the layer selector
				ImGui::Text("%-19s", "Sound Type");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(dropdownWidth);

				//create the string based on the layer enum
				switch (m_inspectedGO->GetComponent<CPAudioSource>()->soundType)
				{
				case SoundType::SFX:
					soundTypeName = "SFX";
					break;
				case SoundType::BGM:
					soundTypeName = "BGM";
					break;
				}

				//TODO: Add ability to get c string based on render layer
				if (ImGui::BeginCombo("##SoundType", soundTypeName.c_str()))
				{
					//loop through each enum
					for (int i{ 0 }; i < SoundType::COUNT; ++i)
					{
						//get the string for the layer
						switch (i)
						{
						case SoundType::SFX:
							soundTypeName = "SFX";
							break;
						case SoundType::BGM:
							soundTypeName = "BGM";
							break;
						}

						if (ImGui::Selectable(soundTypeName.c_str()))
						{
							m_inspectedGO->GetComponent<CPAudioSource>()->soundType = static_cast<SoundType>(i);
						}
					}
					ImGui::EndCombo();
				}

				ImGui::Text("%-19s", "Play On Awake");
				ImGui::SameLine();
				ImGui::Checkbox("##PlayOnAwake", &m_inspectedGO->GetComponent<CPAudioSource>()->playOnAwake);

				ImGui::Text("%-19s", "Loop");
				ImGui::SameLine();
				ImGui::Checkbox("##Loop", &m_inspectedGO->GetComponent<CPAudioSource>()->loop);

				ImGui::Text("%-19s", "3D Sound");
				ImGui::SameLine();
				ImGui::Checkbox("##3DSound", &m_inspectedGO->GetComponent<CPAudioSource>()->is3D);

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
		if (m_inspectedGO->HasComponent<CPAudioListener>())
		{
			if (ImGui::CollapsingHeader("Audio Listener", ImGuiTreeNodeFlags_DefaultOpen))
			{
				isActive = m_inspectedGO->GetComponent<CPAudioListener>()->m_active;
				ImGui::PushID("AudioListenerActive");
				ImGui::Checkbox("Active", &isActive);	
				ImGui::PopID();
				if (isActive != m_inspectedGO->GetComponent<CPAudioListener>()->m_active)
				{
					m_inspectedGO->GetComponent<CPAudioListener>()->ToggleActiveFlag(isActive);
				}
				// Delete Component
				if (ImGui::Button("Delete Audio Listener Component"))
				{
					m_inspectedGO->RemoveComponent(C_CPAudioListener);
				}
			}
		}
		if (m_inspectedGO->HasComponent<CPText>())
		{
			if (ImGui::CollapsingHeader("Text Component", ImGuiTreeNodeFlags_DefaultOpen))
			{
				isActive = m_inspectedGO->GetComponent<CPText>()->m_active;
				ImGui::PushID("TextActive");
				ImGui::Checkbox("Active", &isActive);
				ImGui::PopID();
				if (isActive != m_inspectedGO->GetComponent<CPText>()->m_active)
				{
					m_inspectedGO->GetComponent<CPText>()->ToggleActiveFlag(isActive);
				}

				//float textXPos = m_inspectedGO->GetComponent<CPText>()->get_msg().x;
				//float textYPos = m_inspectedGO->GetComponent<CPText>()->get_msg().y;
				float xbound = m_inspectedGO->GetComponent<CPText>()->get_msg().xbound;
				float textSize = m_inspectedGO->GetComponent<CPText>()->get_msg().scale;
				ImGui::Text("%-17s X", "Width");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##Width", &xbound, 1.0f, 0.0f, 0.0f, "%.1f");
				//ImGui::SameLine();
				//ImGui::SetNextItemWidth(normalWidth);
				//ImGui::DragFloat("##TextPosX", &textXPos, 1.0f, 0.0f, 0.0f, "%.2f");
				//ImGui::SameLine();
				//ImGui::Text("Y");
				//ImGui::SameLine();
				//ImGui::SetNextItemWidth(normalWidth);
				//ImGui::DragFloat("##TextPosY", &textYPos, 1.0f, 0.0f, 0.0f, "%.2f");

				//when coord sys is true we are using world space
				//when its false we are using screen space
				ImGui::SameLine();
				isActive = m_inspectedGO->GetComponent<CPText>()->get_coord_sys();
				ImGui::PushID("WorldCoords");
				ImGui::Checkbox("Use World coordinates", &isActive);
				ImGui::PopID();
				if (isActive != m_inspectedGO->GetComponent<CPText>()->get_coord_sys())
				{
					m_inspectedGO->GetComponent<CPText>()->use_world_coords(isActive);
				}

				ImGui::Text("%-17s", "Size");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##TextSize", &textSize, 0.01f, 0.0f, 0.0f, "%.2f");
				//m_inspectedGO->GetComponent<CPText>()->update_msg_pos(Vec2<float>(textXPos, textYPos));
				////m_inspectedGO->GetComponent<CPText>()->update_msg_size(textSize);

				//m_inspectedGO->GetComponent<CPText>()->get_msg().x = textXPos;
				//m_inspectedGO->GetComponent<CPText>()->get_msg().y = textYPos;
				m_inspectedGO->GetComponent<CPText>()->get_msg().scale = textSize;
				m_inspectedGO->GetComponent<CPText>()->get_msg().xbound = xbound;

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
				static ImVec4 color = ImVec4(col.x, col.y, col.z, 1.f);
				ImGui::Text("Text Color:");
				ImGui::SameLine();
				if (ImGui::ColorEdit3("##TextColor", (float*)&color))
				{
					col = Vec3<float>(color.x, color.y, color.z);
					//m_inspectedGO->GetComponent<CPText>()->update_msg_color(col);
					m_inspectedGO->GetComponent<CPText>()->get_msg().color = col;
				}
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
		if (m_inspectedGO->HasComponent<CPAnimator>())
		{
			if (ImGui::CollapsingHeader("Animator", ImGuiTreeNodeFlags_DefaultOpen))
			{
				isActive = m_inspectedGO->GetComponent<CPAnimator>()->m_active;
				ImGui::PushID("AnimatorActive");
				ImGui::Checkbox("Active", &isActive);
				ImGui::PopID();
				if (isActive != m_inspectedGO->GetComponent<CPAnimator>()->m_active)
				{
					m_inspectedGO->GetComponent<CPAnimator>()->ToggleActiveFlag(isActive);
				}

				ImGui::Text("%-17s", "Controller Name");
				ImGui::SameLine();

				if (ImGui::BeginCombo("##Controller", m_inspectedGO->GetComponent<CPAnimator>()->GetControllerName().c_str()))
				{
					for (auto& [str, controller] : ASSETMANAGER->AnimControllers)
					{
						if (ImGui::Selectable(controller.m_name.c_str()))
						{
							m_inspectedGO->GetComponent<CPAnimator>()->SetControllerName(controller.m_name);
						}
					}

					ImGui::EndCombo();
				}
				if (m_inspectedGO->GetComponent<CPAnimator>()->GetControllerName() != "No controller")
				{
					ImGui::Text("%-17s", "Default State");
					ImGui::SameLine();
					if (ImGui::BeginCombo("##DefaultState", m_inspectedGO->GetComponent<CPAnimator>()->m_defaultState.c_str()))
					{
						if (ImGui::Selectable("None"))
						{
							m_inspectedGO->GetComponent<CPAnimator>()->m_defaultState = "None";
						}
						for (auto const& state : ASSETMANAGER->AnimControllers[m_inspectedGO->GetComponent<CPAnimator>()->GetControllerName()].GetStates())
						{
							if (ImGui::Selectable(state.c_str()))
							{
								m_inspectedGO->GetComponent<CPAnimator>()->m_defaultState = state;
							}
						}

						ImGui::EndCombo();
					}
				}
				ImGui::Text("%-17s", "Play on Awake");
				ImGui::SameLine();
				if (ImGui::Checkbox("##PlayOnAwake", &m_inspectedGO->GetComponent<CPAnimator>()->m_playOnAwake))
				{ }
				ImGui::Text("%-17s", "Awake Delay");
				ImGui::SameLine();
				if (ImGui::DragFloat("##AwakeDelay", &m_inspectedGO->GetComponent<CPAnimator>()->m_awakeDelay))
				{ }
				ImGui::Text("%-17s", "Repeating");
				ImGui::SameLine();
				if (ImGui::Checkbox("##Repeating", &m_inspectedGO->GetComponent<CPAnimator>()->m_repeating))
				{ }
				ImGui::Text("%-17s", "Should Cull");
				ImGui::SameLine();
				if (ImGui::Checkbox("##ShouldCull", &m_inspectedGO->GetComponent<CPAnimator>()->m_shouldCull))
				{ }

				if (ImGui::Button("Delete Animator Component"))
				{
					m_inspectedGO->RemoveComponent(C_CPAnimator);
				}
			}
		}
		if (m_inspectedGO->HasComponent<CPParticle>()) 
		{
			if (ImGui::CollapsingHeader("Particle", ImGuiTreeNodeFlags_DefaultOpen))
			{
				isActive = m_inspectedGO->GetComponent<CPParticle>()->m_active;
				ImGui::PushID("ParticleActive");
				ImGui::Checkbox("Active", &isActive);
				ImGui::PopID();
				if (isActive != m_inspectedGO->GetComponent<CPParticle>()->m_active)
				{
					m_inspectedGO->GetComponent<CPParticle>()->ToggleActiveFlag(isActive);
				}

				// Type of particle emitter
				std::string emitterType = m_inspectedGO->GetComponent<CPParticle>()->GetEmitterType();
				ImGui::Text("%-17s", "Emitter Type");
				ImGui::SameLine();
				if (ImGui::BeginCombo("##ParticleType", m_inspectedGO->GetComponent<CPParticle>()->GetEmitterType().c_str()))
				{

					for (auto& [str, type] : ParticleManager::Instance()->GetVectorEmits())
					{
						if (ImGui::Selectable(str.c_str()))
						{
							m_inspectedGO->GetComponent<CPParticle>()->mEmitterType = type;
						}
					}
					ImGui::EndCombo();
				}
				// Emitter Rate
				ImGui::Separator();
				ImGui::Text("EMITTER RATE");
				ImGui::Separator();
				ImGui::Text("%-17s", "Emitter Rate");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);

				ImGui::Text("%-17s", "Emitter Rate");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##EmitterRate", &m_inspectedGO->GetComponent<CPParticle>()->mEmitterRate, 1.0f, 0.0f, 0.0f, "%.2f");
				if (m_inspectedGO->GetComponent<CPParticle>()->mEmitterRate <= 0.f) {
					m_inspectedGO->GetComponent<CPParticle>()->mEmitterRate = 0.f;
				}

				// Emitter Rate Randomness
				bool& isEmitterRateRandom = m_inspectedGO->GetComponent<CPParticle>()->mEmitterRateRandomness;
				ImGui::PushID("EmitterRateRandom");
				ImGui::Checkbox("Emitter Rate Random", &isEmitterRateRandom);
				ImGui::PopID();
				if (isEmitterRateRandom != m_inspectedGO->GetComponent<CPParticle>()->mEmitterRateRandomness)
				{
					m_inspectedGO->GetComponent<CPParticle>()->mIsActive = isEmitterRateRandom;
				}

				// Emitter Rate Random Min and Max
				ImGui::Text("%-17s", "Rate Random Min");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##EmitterRateMin", &m_inspectedGO->GetComponent<CPParticle>()->mEmitterRateRandomnessMin, 1.0f, 0.0f, 0.0f, "%.2f");
				if (m_inspectedGO->GetComponent<CPParticle>()->mEmitterRateRandomnessMin <= 0.f) {
					m_inspectedGO->GetComponent<CPParticle>()->mEmitterRateRandomnessMin = 0.f;
				}
				ImGui::Text("%-17s", "Rate Random Max");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##EmitterRateMax", &m_inspectedGO->GetComponent<CPParticle>()->mEmitterRateRandomnessMin, 1.0f, 0.0f, 0.0f, "%.2f");
				if (m_inspectedGO->GetComponent<CPParticle>()->mEmitterRateRandomnessMax <= 0.f) {
					m_inspectedGO->GetComponent<CPParticle>()->mEmitterRateRandomnessMax = 0.f;
				}




				// Emitter type stats =======================================
				ImGui::Separator();
				ImGui::Text(emitterType.c_str());
				ImGui::Separator();
				// Emitter Velocity
				if (emitterType == "Trail")
				{
					Vec2<float>& velocity = m_inspectedGO->GetComponent<CPParticle>()->mEmitterVelocity;
					ImGui::Text("%-17s X", "Velocity");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##VelocityX", &velocity.x, 1.0f, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##VelocityY", &velocity.y, 1.0f, 0.0f, 0.0f, "%.2f");
				}

				// Emitter X Min and Max Variation
				if (emitterType == "Trail")
				{
					ImGui::Text("%-17s Min", "Vel Variation X");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##MinX", &m_inspectedGO->GetComponent<CPParticle>()->mEmitterVariationMinX, 1.0f, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::Text("Max");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##MaxX", &m_inspectedGO->GetComponent<CPParticle>()->mEmitterVariationMaxX, 1.0f, 0.0f, 0.0f, "%.2f");
					// Emitter Y Min and Max Variation
					ImGui::Text("%-17s Min", "Vel Variation Y");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##MinY", &m_inspectedGO->GetComponent<CPParticle>()->mEmitterVariationMinY, 1.0f, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::Text("Max");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##MaxY", &m_inspectedGO->GetComponent<CPParticle>()->mEmitterVariationMaxY, 1.0f, 0.0f, 0.0f, "%.2f");
				}

				// Radial Spawn In Circle
				if (emitterType == "Radial" || emitterType == "InverseRadial")
				{
					bool& isRadialSpawnInCircle = m_inspectedGO->GetComponent<CPParticle>()->mEmitterRadialSpawnCircle;
					ImGui::PushID("RadialSpawnInCircle");
					ImGui::Checkbox("Spawn In Circle", &isRadialSpawnInCircle);
					ImGui::PopID();
					if (isRadialSpawnInCircle != m_inspectedGO->GetComponent<CPParticle>()->mEmitterRadialSpawnCircle)
					{
						m_inspectedGO->GetComponent<CPParticle>()->mIsActive = isRadialSpawnInCircle;
					}
				}

				// Inv Radial Distance
				if (emitterType == "InverseRadial")
				{
					ImGui::Text("%-17s", "InvRadial Distance Min");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##InvRadSpeedMin", &m_inspectedGO->GetComponent<CPParticle>()->mInvRadDistanceMin, 1.0, 0.f, 0.f, "%.2f");
					ImGui::Text("%-17s", "InvRadial Distance Max");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##InvRadSpeedMax", &m_inspectedGO->GetComponent<CPParticle>()->mInvRadDistanceMax, 1.0, 0.f, 0.f, "%.2f");
				}


				// Radial Num
				if (emitterType == "Radial" || emitterType == "InverseRadial")
				{
					ImGui::Text("%-17s", "Radial Num");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragInt("##RadialNum", &m_inspectedGO->GetComponent<CPParticle>()->mRadialParticles, 1, 0, 0, "%d");
				}
				// Radial Speed
				if (emitterType == "Radial")
				{
					ImGui::Text("%-17s", "Radial Speed");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(normalWidth);
					ImGui::DragFloat("##RadialSpeed", &m_inspectedGO->GetComponent<CPParticle>()->mEmitterRadialSpeed, 1.0, 0.f, 0.f, "%.2f");
				}

				// End of EmitterType stats ======================================================


				// Emitter Rotation
				ImGui::Separator();
				ImGui::Text("ROTATION");
				ImGui::Separator();
				ImGui::Text("%-17s", "Rotation Speed");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##RotationSpeed", &m_inspectedGO->GetComponent<CPParticle>()->mRotationSpeed, 1.0, 0.f, 0.f, "%.2f");

				ImGui::Text("%-17s Min", "Rotation Spd Variation");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##RotSpdVariaionMin", &m_inspectedGO->GetComponent<CPParticle>()->mRotationSpeedVariationMin, 1.0, 0.f, 0.f, "%.2f");
				ImGui::Text("%-17s Max", "Rotation Spd Variation");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##RotSpdVariaionMax", &m_inspectedGO->GetComponent<CPParticle>()->mRotationSpeedVariationMax, 1.0, 0.f, 0.f, "%.2f");

				
				// Emitter Size
				ImGui::Separator();
				ImGui::Text("SIZE");
				ImGui::Separator();
				ImGui::Text("%-17s Begin", "Emitter Size");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##SizeBegin", &m_inspectedGO->GetComponent<CPParticle>()->mEmitterSizeBegin, 0.1f, 0.0f, 100.0f, "%.2f");
				if (m_inspectedGO->GetComponent<CPParticle>()->mEmitterSizeBegin <= 0.f) {
					m_inspectedGO->GetComponent<CPParticle>()->mEmitterSizeBegin = 0.f;
				}
				ImGui::SameLine();
				ImGui::Text("End");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##SizeEnd", &m_inspectedGO->GetComponent<CPParticle>()->mEmitterSizeEnd, 0.1f, 0.0f, 100.0f, "%.2f");
				if (m_inspectedGO->GetComponent<CPParticle>()->mEmitterSizeEnd <= 0.f) {
					m_inspectedGO->GetComponent<CPParticle>()->mEmitterSizeEnd = 0.f;
				}

				// Emitter Lifetime
				ImGui::Separator();
				ImGui::Text("LIFETIME");
				ImGui::Separator();
				ImGui::Text("%-17s", "Emitter Lifetime");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##EmitterLifeTime", &m_inspectedGO->GetComponent<CPParticle>()->mEmitterLifetime, 0.5f, 0.0f, 0.0f, "%.2f");
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::Text("Lifetime Left: %.2f", m_inspectedGO->GetComponent<CPParticle>()->mEmitterLifetimeRemaining);
				// Particle Lifetime
				ImGui::Text("%-17s", "Particle Lifetime");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##ParticleLifeTime", &m_inspectedGO->GetComponent<CPParticle>()->mParticleLifetime, 0.5f, 0.0f, 0.0f, "%.2f");

				bool& isParticleLifetimeRandom = m_inspectedGO->GetComponent<CPParticle>()->mParticleLifetimeRandomness;
				ImGui::PushID("ParticleLifetimeRandom");
				ImGui::Checkbox("Particle Lifetime Random", &isParticleLifetimeRandom);
				ImGui::PopID();
				if (isParticleLifetimeRandom != m_inspectedGO->GetComponent<CPParticle>()->mParticleLifetimeRandomness)
				{
					m_inspectedGO->GetComponent<CPParticle>()->mIsActive = isParticleLifetimeRandom;
				}

				ImGui::Text("%-17s", "Particle Lifetime Min");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##ParticleLifeTimeMin", &m_inspectedGO->GetComponent<CPParticle>()->mParticleLifetimeMin, 0.5f, 0.0f, 0.0f, "%.2f");

				ImGui::Text("%-17s", "Particle Lifetime Max");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##ParticleLifeTimeMax", &m_inspectedGO->GetComponent<CPParticle>()->mParticleLifetimeMax, 0.5f, 0.0f, 0.0f, "%.2f");

				// Particle Lifetime Delay
				ImGui::Text("%-17s Min", "Particle Lifetime Delay");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##ParticleLifeTimeDelayMin", &m_inspectedGO->GetComponent<CPParticle>()->mEmitterLifetimeDelayMin, 0.01f, 0.0f, 0.0f, "%.2f");

				ImGui::Text("%-17s Max", "Particle Lifetime Delay");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(normalWidth);
				ImGui::DragFloat("##ParticleLifeTimeDelayMax", &m_inspectedGO->GetComponent<CPParticle>()->mEmitterLifetimeDelayMax, 0.01f, 0.0f, 0.0f, "%.2f");


				// Emitter Active
				bool& isEmitterActive = m_inspectedGO->GetComponent<CPParticle>()->mIsActive;
				ImGui::PushID("EmitterActive");
				ImGui::Checkbox("Emitter Active", &isEmitterActive);
				ImGui::PopID();
				if (isEmitterActive != m_inspectedGO->GetComponent<CPParticle>()->mIsActive)
				{
					m_inspectedGO->GetComponent<CPParticle>()->mIsActive = isEmitterActive;

					if (m_inspectedGO->GetComponent<CPParticle>()->mIsActive == true)
					{
						m_inspectedGO->GetComponent<CPParticle>()->RestartLifetime();
					}
				}

				// Loop Active
				bool& isEmitterLooping = m_inspectedGO->GetComponent<CPParticle>()->mIsLooping;
				ImGui::PushID("EmitterLooping");
				ImGui::Checkbox("Emitter Looping", &isEmitterLooping);
				ImGui::PopID();
				if (isEmitterLooping != m_inspectedGO->GetComponent<CPParticle>()->mIsLooping)
				{
					m_inspectedGO->GetComponent<CPParticle>()->mIsActive = isEmitterActive;
				}


				if (ImGui::Button("Delete Particle Component"))
				{
					m_inspectedGO->RemoveComponent(C_CPParticle);
				}
			
			}
		}
		//----------------------------------------INSPECT COMPONENTS WINDOW---------------------------------------
	}

	/*!***********************************************************************
	  \brief
	  Update the currently inspected game object.
	  \return
	  Nothing.
	*************************************************************************/
	void InspectorGameObject::UpdateInspectedGO(GameObject* newInspectedGO)
	{
		// Make the inspector show the GO
		EDITORINSPECTOR->SetWindowGameObject();

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
	GameObject* InspectorGameObject::GetInspectedGO()
	{
		return m_inspectedGO;
	}

	/*!***********************************************************************
	  \brief
	  Check if a game object is currently being inspected.
	  \return
	  True if a game object is being inspected, false otherwise.
	*************************************************************************/
	bool InspectorGameObject::IsGOInspected()
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
		InspectorGO->UpdateInspectedGO(newInspectedGO);
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
			InspectorGO->UpdateInspectedGO(nullptr);
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
		InspectorGO->UpdateInspectedGO(nullptr);
	}

	/*!***********************************************************************
	  \brief
	  Get the current gizmos mode.
	  \return
	  Nothing.
	*************************************************************************/
	ImGuizmo::MODE InspectorGameObject::GetGizmosMode() const
	{
		return m_CurrentGizmosMode;
	}

	/*!***********************************************************************
	  \brief
	  Sets the current gizmos mode.
	  \return
	  Nothing.
	*************************************************************************/
	void InspectorGameObject::SetGizmosMode(ImGuizmo::MODE mode)
	{
		m_CurrentGizmosMode = mode;
	}

	/*!***********************************************************************
	  \brief
	  Get the current gizmos operation.
	  \return
	  Nothing.
	*************************************************************************/
	ImGuizmo::OPERATION InspectorGameObject::GetGizmosOperation() const
	{
		return m_CurrentGizmosOperation;
	}

	void InspectorGameObject::SetGizmosOperation(ImGuizmo::OPERATION operation)
	{
		m_CurrentGizmosOperation = operation;
	}

	/*!***********************************************************************
	  \brief
	  Gets the snap mode.
	  \return
	  Nothing.
	*************************************************************************/
	bool const& InspectorGameObject::GetSnapMode() const
	{
		return m_SnapMode;
	}

	/*!***********************************************************************
	  \brief
	  Toggles snap mode.
	  \return
	  Nothing.
	*************************************************************************/
	bool& InspectorGameObject::ToggleSnapMode()
	{
		return m_SnapMode;
	}

	/*!***********************************************************************
	  \brief
	  Get the snap value of translate.
	  \return
	  Nothing.
	*************************************************************************/
	float const& InspectorGameObject::GetSnapTranslate() const
	{
		return m_SnapTranslate[0];
	}

	/*!***********************************************************************
	  \brief
	  Get the snap value of rotate.
	  \return
	  Nothing.
	*************************************************************************/
	float const& InspectorGameObject::GetSnapRotate() const
	{
		return m_SnapRotate;
	}

	/*!***********************************************************************
	  \brief
	  Get the snap value of scale.
	  \return
	  Nothing.
	*************************************************************************/
	float const& InspectorGameObject::GetSnapScale() const
	{
		return m_SnapScale[0];
	}

	/*!***********************************************************************
	  \brief
	  Set the snap value of translate through ImGui.
	  \return
	  Nothing.
	*************************************************************************/
	float& InspectorGameObject::SetSnapTranslate()
	{
		return m_SnapTranslate[0];
	}

	/*!***********************************************************************
	  \brief
	  Set the snap value of rotate through ImGui.
	  \return
	  Nothing.
	*************************************************************************/
	float& InspectorGameObject::SetSnapRotate()
	{
		return m_SnapRotate;
	}

	/*!***********************************************************************
	  \brief
	  Set the snap value of scale through ImGui.
	  \return
	  Nothing.
	*************************************************************************/
	float& InspectorGameObject::SetSnapScale()
	{
		return m_SnapScale[0];
	}
}