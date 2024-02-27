/*!************************************************************************
 \file				EditorTMEditor.cpp
 \author(s)			Ryan Tan Jian Hao
 \par DP email(s):	ryanjianhao.tan@digipen.edu
 \par Course:       CSD2401A
 \date				12/12/2023
 \brief

 This source file contains functions definitions for the Tile map editor
 window. Which allows the user to select the background texture and set
 the UVs for each tile label.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "EditorTMEditor.h"
#include "LitterBox/Serialization/AssetManager.h"

namespace LB
{
	EditorTMEditor* EDITORTMEDITOR = nullptr;

	//variable defines
	//I don't know what this is for.
	//taken from EditorInspector in the Section that
	//draws the ImGui components for CPRender
	static float dropdownWidth = 150.f;
	static float normalWidth = 75.f;

	//height constraint of texture preview
	static float textPreviewLim = 250.f;
	/*!***********************************************************************
	  \brief
	  Constructor for the EditorSceneView class.
	  \param
	  The string identifier for the Layering system to know which window this is.
	*************************************************************************/
	EditorTMEditor::EditorTMEditor(std::string layerName) :
		Layer(layerName),
		textureID{ 0 },
		slotID{ 0 },
		textureAspect{ 0.f },
		textureSize{ 0.f,0.f },
		tiles{}
	{
		if (!EDITORTMEDITOR)
			EDITORTMEDITOR = this;
		else
			DebuggerLogError("Editor Tile Map Editor already exists!");
	}
	/*!***********************************************************************
	  \brief
	  Update function that is called every frame.
	*************************************************************************/
	void EditorTMEditor::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		ImGui::BeginChild("TileMapEditor");

		//-----------------------------------------For texture preview---------------------------
		//The texture preview is constrained by the height of the window because
		//it is assumed that the editor inspector window is in portrait mode
		//NOTE: best not to use window in landscape mode
		
		//arbitrary number can be changed anytime
		textureSize.x = ImGui::GetContentRegionAvail().x;

		if(textureSize.x)
		//-----------------------------------------For texture preview---------------------------


		//--------------------TEXTURE SELECTOR--------------------
		//----------------------------------TAKEN FROM EDITOR INSPECTOR CPRENDER--------------------------------
		//create a selection box to select textures
		// Interface Buttons
		ImGui::Text("%-19s", "Image");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(dropdownWidth);
		//This allows you to drag into the text
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* textureData = ImGui::AcceptDragDropPayload("TEXTURE"))
			{
				const char* textureName = (const char*)textureData->Data;
				//I hope and pray that this is texture ID handle and not bounded texture slot
				textureID = ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->id;
				slotID = ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second;
				
				////to get the texture aspect ratio
				//textureAspect = (float)ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->height 
				//	/ (float)ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->width;

				////to get the right size to display the texture image
				////DOES NOT SCALE WHEN WINDOW DIMENSIONS CHANGE
				//textureSize.y = textureSize.x * textureAspect;
				calPreviewSize(
					(float)ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->width,
					(float)ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->height,
					textPreviewLim
				);
			}
		}
		if (ImGui::BeginCombo("##Texture", ASSETMANAGER->GetTextureName(slotID).c_str()))
		{
			for (auto& [str, tex] : ASSETMANAGER->Textures)
			{
				std::filesystem::path tempPath{ str };
				if (ImGui::Selectable(tempPath.filename().stem().string().c_str()))
				{
					//getting the TEXTURE ID HANDLE and not the BOUNDED TEXTURE SLOT
					slotID = tex.second;

					//check if we selected none
					if (!slotID) {
						textureID = 0;
						break;
					}
					
					textureID = tex.first->id;
					
					////to get the texture aspect ratio
					//textureAspect = (float)tex.first->height
					//	/ (float)tex.first->width;

					////to get the right size to display the texture image
					////DOES NOT SCALE WHEN WINDOW DIMENSIONS CHANGE
					//textureSize.y = textureSize.x * textureAspect;

					calPreviewSize(
						(float)tex.first->width,
						(float)tex.first->height,
						textPreviewLim
					);
					break;
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
				textureID = ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->id;
				slotID = ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second;
				
				////to get the texture aspect ratio
				//textureAspect = (float)ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->height
				//	/ (float)ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->width;

				////to get the right size to display the texture image
				////DOES NOT SCALE WHEN WINDOW DIMENSIONS CHANGE
				//textureSize.y = textureSize.x * textureAspect;
				calPreviewSize(
					(float)ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->width,
					(float)ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->height,
					textPreviewLim
				);
			}
		}
		//----------------------------------TAKEN FROM EDITOR INSPECTOR CPRENDER--------------------------------
		//--------------------TEXTURE SELECTOR--------------------

		ImGui::Separator();
		//TODO figure out the preview option
		uint64_t warning_remover = (uint64_t)textureID;
		if (warning_remover != static_cast<unsigned int>(-1))
			ImGui::Image(reinterpret_cast<ImTextureID>(warning_remover), textureSize, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::Separator();

		if (!textureID) {
			ImGui::EndChild();

			ImGui::End();
			return;
		}

		//add row and column input
		//Inputs to split the tile map sprite sheet
		//no real reason that its a static local variable. You can create member variables if you so please
		static int row{}, col{};
		//tracks whether changes to the number of tiles has been made
		static bool changed{ false };
		ImGui::Text("%-17s", "Rows");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(normalWidth);
		if (ImGui::InputInt("##Rows", &row))
			changed = true;
		ImGui::Text("%-17s", "Columns");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(normalWidth);
		if (ImGui::InputInt("##Columns", &col))
			changed = true;

		//creating vertex of UV data
		//print confirm button if we changed the row and col values
		if (changed && ImGui::Button("Confirm")) {
			createUV(row, col);
			changed = false;
		}

		//display details of each tile here
		unsigned int tileIndex{ 1 };
		for (auto& min_max : tiles) {
			//label the tile
			ImGui::Text("Tile %d:", tileIndex++);

			//draw image with the UVs
			ImGui::SameLine();
			//The min max is mixed up because ImGui uses a different texel coordinate
			//system from openGL so we do the mixing anytime we use ImGui.
			//The format of the min max UV coordinates uses OpenGL texel coordinates instead

			warning_remover = (uint64_t)textureID;
			if (warning_remover != static_cast<unsigned int>(-1))
				ImGui::Image(reinterpret_cast<ImTextureID>(warning_remover),
					ImVec2{ normalWidth, normalWidth }, 
					ImVec2{ min_max.first.first, min_max.second.second }, 
					ImVec2{ min_max.second.first, min_max.first.second });
			
			//TODO let user fine tune UV
		}

		ImGui::EndChild();
		
		ImGui::End();
	}

	/*!***********************************************************************
	  \brief
	  Calculates the minimum and maximum UVs required for splitting the texture
	  given.

	  \param rows
	  The number of times to split the texture horizontally
	  \param cols
	  The number of times to split the texture vertically
	*************************************************************************/
	void EditorTMEditor::createUV(int rows, int cols) {
		if (rows <= 0 || cols <= 0) {
			DebuggerLogError("Number of Rows or Columns input for the tilemap is not more than 0");
			return;
		}

		float x_inc{ 1.f / static_cast<float>(cols) }, y_inc{ 1.f / static_cast<float>(rows) };

		tiles.clear();

		for(int y{rows}; y > 0; --y)
			for (int x{ 0 }; x < cols; ++x) {
				//0,0 is bottom left of texture. Therefore, if the first texture is
				//the top left tile, then the top left would be 0,1
				//the min would be the same x axis but 1 increment down from 1
				//the max would be 1 increment right from 0 and the same y axis
				std::pair<float, float> min{ static_cast<float>(x) * x_inc, static_cast<float>(y - 1) * y_inc };
				std::pair<float, float> max{ static_cast<float>(x + 1) * x_inc, static_cast<float>(y) * y_inc };

				tiles.emplace_back(min, max);
			}
	}

	/*!***********************************************************************
	  \brief
	  Calcullates preview size, for the texture in ImGui window so it displays
	  nicely regardless of texture size

	  \param width
	  The width of the texture
	  \param height
	  The height of the texture
	  \param previewLimit
	  How small you want the viewport in ImGui to be
	*************************************************************************/
	void EditorTMEditor::calPreviewSize(float width, float height, float previewLimit) {
		//error handling for width and height
		if (width <= 0.f || height <= 0.f)
			return;

		//make aspect ratio dependent on width by default
		if (width >= height) {
			//height based aspect ratio
			textureAspect = width / height;
			textureSize.y = previewLimit;
			textureSize.x = textureSize.y * textureAspect;
			return;
		}
		else { //if height is greater than width then we based the aspect ratio of the height
			//width based aspect ratio
			textureAspect = height / width;
			//limit the width
			textureSize.x = previewLimit + 100.f;
			//calculate new height
			textureSize.y = textureSize.x * textureAspect;
		}
	}
}
