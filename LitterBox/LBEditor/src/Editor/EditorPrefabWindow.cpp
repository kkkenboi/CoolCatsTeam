/*!************************************************************************
 \file				EditorHierarchy.cpp
 \author(s)			Amadeus Chia 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu
 \par Course:       CSD2401A
 \date				03/11/2023
 \brief

 This file contains functions defintions for the Editor prefab window
 TODO :: Implement the prefab preview here!! 

 
 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "pch.h"

#include "EditorPrefabWindow.h"
#include "EditorInspector.h"
#include "EditorAssets.h"

#include "LitterBox/Engine/Time.h"
#include "LitterBox/Utils/Math.h"
namespace LB
{
	EditorPrefabWindow* EDITORPREFAB = nullptr;
	/*!***********************************************************************
	  \brief
	  Constructor for the EditorPrefabWindow class.
	  (TODO:: Implement the prefab window preview here in the future!)
	  \return
	  Nothing.
	*************************************************************************/
	EditorPrefabWindow::EditorPrefabWindow(std::string layerName) : Layer(layerName)
	{
		if (!EDITORPREFAB) EDITORPREFAB = this;
		else DebuggerLogError("Editor Prefab already exists!");


	}

	static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
	}
	/*!***********************************************************************
	\brief
	Initializes the EditorPrefabWindow layer.
	\return
	Nothing.
	*************************************************************************/
	void EditorPrefabWindow::Initialize()
	{
	}

	/*!***********************************************************************
	\brief
	Initializes the EditorPrefabWindow layer. (Subscribes the callback function)
	\return
	Nothing.
	*************************************************************************/
	void EditorPrefabWindow::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());
		if(ImGui::Button("Save"))
		{
			DebuggerLogFormat("Saving %s prefab", InspectorGameObject::Instance()->GetInspectedGO()->GetName().c_str());
			//Save the prefab to file by it's name
			JSONSerializer::SerializeToFile(EDITORASSETS->currentDirectory.string() + "/" + InspectorGameObject::Instance()->GetInspectedGO()->GetName() + ".prefab", *InspectorGameObject::Instance()->GetInspectedGO());
		}
		if (InspectorGameObject::Instance()->isPrefab)
		{
			//We cache the obj so it's shorter to type
			GameObject* prefabGO = InspectorGameObject::Instance()->GetInspectedGO();
			//We make the scale a bit bigger just so that we can see 
			//Somehow the xScale has to be flipped so that the sprite is displayed correctly
			float xScale = -prefabGO->GetComponent<CPTransform>()->GetScale().x*100;
			float yScale = prefabGO->GetComponent<CPTransform>()->GetScale().y*100;
			//int prefabTexture = ASSETMANAGER->GetTextureUnit(ASSETMANAGER->GetTextureName(prefabGO->GetComponent<CPRender>()->texture));
			//this sets the prefab texture
			if (!prefabGO->HasComponent<CPRender>()) { ImGui::End(); return; }

			CPRender* render = prefabGO->GetComponent<CPRender>();
			int prefabTexture;
			if (render->spriteIndex >= 0)
			{
				prefabTexture = ASSETMANAGER->GetTextureIndex(ASSETMANAGER->GetSpriteSheet(render->spriteSheetName).GetPNGRef());
			}
			else
			{
				prefabTexture = ASSETMANAGER->GetTextureIndex(ASSETMANAGER->GetTextureName(prefabGO->GetComponent<CPRender>()->texture));
			}
			
			//Getting the imgui draw list so we can apply transformations to the image
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			float angle = prefabGO->GetComponent<CPTransform>()->GetRotation();
			ImVec2 p = ImGui::GetCursorScreenPos();	//This cursor is not mouse cursor!! it's the gui cursor!!
			//Some math for the rotation 
			float cos_a = cosf((-DegToRad(angle)) + static_cast<float>(PI));
			float sin_a = sinf((-DegToRad(angle)) + static_cast<float>(PI));
			//ImVec2 center{ prefabGO->GetComponent<CPTransform>()->GetPosition().x+100,prefabGO->GetComponent<CPTransform>()->GetPosition().y+100 };
			ImVec2 center{ p.x+ImGui::GetWindowWidth()/2,p.y + yScale/2};
			ImVec2 size{ render->w * 0.1f, render->h * 0.1f };

			//Applying the rotation
			ImVec2 pos[4] =
			{
				center + ImRotate(ImVec2(-size.x * xScale * 0.5f, -size.y * yScale * 0.5f), cos_a, sin_a),
				center + ImRotate(ImVec2(size.x * xScale * 0.5f, -size.y * yScale * 0.5f), cos_a, sin_a),
				center + ImRotate(ImVec2(size.x * xScale * 0.5f, size.y * yScale * 0.5f), cos_a, sin_a),
				center + ImRotate(ImVec2(-size.x * xScale * 0.5f, size.y * yScale * 0.5f), cos_a, sin_a)
			};
			ImVec2 uvs[4];
			
			if (render->spriteIndex >= 0)
			{
				uvs[0] = ImVec2(render->uv[0].x, render->uv[0].y);
				uvs[1] = ImVec2(render->uv[1].x, render->uv[1].y);
				uvs[2] = ImVec2(render->uv[2].x, render->uv[2].y);
				uvs[3] = ImVec2(render->uv[3].x, render->uv[3].y);
			}
			else
			{
				uvs[0] = ImVec2(0.0f, 0.0f);
				uvs[1] = ImVec2(1.0f, 0.0f);
				uvs[2] = ImVec2(1.0f, 1.0f);
				uvs[3] = ImVec2(0.0f, 1.0f);
			}
			
			//then we draw it
			drawList->AddImageQuad(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(prefabTexture)), pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], IM_COL32_WHITE);
		}
		ImGui::End();
	}
}