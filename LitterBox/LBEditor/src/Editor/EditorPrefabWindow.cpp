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
			DebuggerLogFormat("Saving %s prefab", EDITORINSPECTOR->GetInspectedGO()->GetName().c_str());
			//Save the prefab to file by it's name
			//JSONSerializer::SerializeToFile(EDITORINSPECTOR->GetInspectedGO()->GetName(), *EDITORINSPECTOR->GetInspectedGO());
		}
		if (EDITORINSPECTOR->isPrefab)
		{
			//We cache the obj so it's shorter to type
			GameObject* prefabGO = EDITORINSPECTOR->GetInspectedGO();
			DebuggerLogWarningFormat("Prefab texture : %s", ASSETMANAGER->GetTextureName(prefabGO->GetComponent<CPRender>()->texture).c_str());
			float xScale = prefabGO->GetComponent<CPTransform>()->GetScale().x;
			float yScale = prefabGO->GetComponent<CPTransform>()->GetScale().y;
			//int prefabTexture = ASSETMANAGER->GetTextureUnit(ASSETMANAGER->GetTextureName(prefabGO->GetComponent<CPRender>()->texture));
			int prefabTexture = ASSETMANAGER->GetTextureIndex(ASSETMANAGER->GetTextureName(prefabGO->GetComponent<CPRender>()->texture));
			ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(prefabTexture)), { 100*xScale,100*yScale }, { 0,1 }, {1,0});
		}
		ImGui::End();
	}
}