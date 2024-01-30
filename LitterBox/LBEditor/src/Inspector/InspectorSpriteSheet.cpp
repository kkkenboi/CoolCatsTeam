/*!************************************************************************
 \file				InspectorSpriteSheet.cpp
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

#include <imgui.h>
#include "InspectorSpriteSheet.h"

namespace LB
{
	//static float textPreviewLim = 250.f;
	static float fullWidth;
	static float columnWidth = 250.0f;
	static float normalWidth = 150.f;

	InspectorSpriteSheet* INSPECTORSPRITESHEET = nullptr;

    /*!***********************************************************************
     \brief

    *************************************************************************/
    void InspectorSpriteSheet::Initialize()
    {
	/*	textureID = 0;
		slotID = 0;
		textureAspect = 0.0f;
		textureSize = ImVec2(0.0f, 0.0f);*/
		if (!INSPECTORSPRITESHEET)
		{
			INSPECTORSPRITESHEET = this;
		}
		else
		{
			DebuggerLogError("Spritesheet already exists!");
		}
    }

    /*!***********************************************************************
     \brief

    *************************************************************************/
    void InspectorSpriteSheet::UpdateLayer()
    {
        // Name
        ImGui::Text("%-17s", "Sprite");
        ImGui::SameLine();

		if (ImGui::BeginCombo("##Texture", m_inspectedSheet.GetPNGRef().c_str()))
		{
			for (auto& [str, tex] : ASSETMANAGER->Textures)
			{
				std::filesystem::path tempPath{ str };
				if (ImGui::Selectable(tempPath.filename().stem().string().c_str()))
				{
					m_textureID = tex.first->id;
					m_inspectedSheet.SetPNGRef(ASSETMANAGER->GetTextureName(tex.second).c_str());
					break;
				}
			}

			ImGui::EndCombo();
		}
		if (ImGui::Button("Save"))
		{
			SaveSpriteSheet();
		}

		fullWidth = ImGui::GetContentRegionAvail().x;
		ImGui::Image((ImTextureID)m_textureID, ImVec2(fullWidth, fullWidth), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::Text("Slicer");
		if (ImGui::BeginTable("SlicerTable", 3))
		{
			ImGui::TableNextColumn();
			ImGui::Text("X");
			ImGui::SetNextItemWidth(normalWidth);
			if (ImGui::InputInt("##SliceX", &m_sliceX))
			{

			}
			ImGui::TableNextColumn();
			ImGui::Text("Y");
			ImGui::SetNextItemWidth(normalWidth);
			if (ImGui::InputInt("##SliceY", &m_sliceY))
			{

			}

			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			ImGui::Text("Width");
			ImGui::SetNextItemWidth(normalWidth);
			if (ImGui::InputInt("##SliceW", &m_sliceWidth))
			{

			}
			ImGui::TableNextColumn();
			ImGui::Text("Height");
			ImGui::SetNextItemWidth(normalWidth);
			if (ImGui::InputInt("##SliceH", &m_sliceHeight))
			{

			}

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			columnWidth = ImGui::GetColumnWidth(0);
			if (ImGui::Button("Slice", ImVec2(columnWidth, 25.0f)))
			{

			}
			ImGui::EndTable();
		}

		ImGui::Dummy(ImVec2(0.0f, 35.0f));

		ImGui::Text("Auto Slicer");
		ImGui::Text("Rows");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(normalWidth);
		if (ImGui::InputInt("##Rows", &m_row))
		{

		}
		ImGui::SameLine();
		ImGui::Text("Columns");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(normalWidth);
		if (ImGui::InputInt("##Columns", &m_col))
		{

		}
		ImGui::PushID("AutoSlicer");
		if (ImGui::Button("Auto Slice", ImVec2(columnWidth, 25.0f)))
		{

		}
		ImGui::PopID();

		ImGui::Dummy(ImVec2(0.0f, 35.0f));

		ImGui::Text("Sprites");
		for (Sprite const& sprite : m_inspectedSheet.Sprites())
		{
			ImGui::Text("Slice %d:", sprite.m_index);
			ImGui::InputInt("X", &m_spriteX);
			ImGui::SameLine();
			ImGui::InputInt("Y", &m_spriteY);
			ImGui::SameLine();
			ImGui::InputInt("Width", &m_spriteW);
			ImGui::SameLine();
			ImGui::InputInt("Height", &m_spriteH);
		}

        /*if (ImGui::Button("Add Slice"))
        {
            m_inspectedSheet->Slice({ 0, 0 }, 0, 0);
        }

        int x, y, w, h;
        for (Sprite const& sprite : m_inspectedSheet->Sprites())
        {
            ImGui::Text("Slice %d:", sprite.m_index);
            ImGui::InputInt("X", &x);
            ImGui::SameLine();
            ImGui::InputInt("Y", &y);
            ImGui::SameLine();
            ImGui::InputInt("Width", &w);
            ImGui::SameLine();
            ImGui::InputInt("Height", &h);
        }*/

		//SpilttingTheSprites(); //helping to split the size of the spritesheet
    }

	void InspectorSpriteSheet::SaveSpriteSheet()
	{
		JSONSerializer::SerializeToFile(m_inspectedSheet.GetName(), m_inspectedSheet);
	}


	// Load from AssetManager
	void InspectorSpriteSheet::LoadSpriteSheet(std::string name)
	{
		JSONSerializer::DeserializeFromFile(name.c_str(), m_inspectedSheet);
		m_textureID = ASSETMANAGER->GetTextureIndex(m_inspectedSheet.GetPNGRef());
	}

	//void InspectorSpriteSheet::createUV(int rows, int cols) {
	//	if (rows <= 0 || cols <= 0) {
	//		DebuggerLogError("Number of Rows or Columns input for the tilemap is not more than 0");
	//		return;
	//	}

	//	float x_inc{ 1.f / static_cast<float>(cols) }, y_inc{ 1.f / static_cast<float>(rows) };

	//	tiles.clear();

	//	for (int y{ rows }; y > 0; --y)
	//		for (int x{ 0 }; x < cols; ++x) {
	//			//0,0 is bottom left of texture. Therefore, if the first texture is
	//			//the top left tile, then the top left would be 0,1
	//			//the min would be the same x axis but 1 increment down from 1
	//			//the max would be 1 increment right from 0 and the same y axis
	//			std::pair<float, float> min{ x * x_inc, (y - 1) * y_inc };
	//			std::pair<float, float> max{ (x + 1) * x_inc, y * y_inc };

	//			tiles.emplace_back(min, max);
	//		}
	//}

	//void InspectorSpriteSheet::calPreviewSize(float width, float height, float previewLimit) {
	//	//error handling for width and height
	//	if (width <= 0.f || height <= 0.f)
	//		return;

	//	//make aspect ratio dependent on width by default
	//	if (width >= height) {
	//		//height based aspect ratio
	//		textureAspect = width / height;
	//		textureSize.y = previewLimit;
	//		textureSize.x = textureSize.y * textureAspect;
	//		return;
	//	}
	//	else { //if height is greater than width then we based the aspect ratio of the height
	//		//width based aspect ratio
	//		textureAspect = height / width;
	//		//limit the width
	//		textureSize.x = previewLimit + 100.f;
	//		//calculate new height
	//		textureSize.y = textureSize.x * textureAspect;
	//	}
	//}

	//void InspectorSpriteSheet::PreviewTexture()
	//{
	//	ImGui::Image((ImTextureID)textureID, textureSize, ImVec2(0, 1), ImVec2(1, 0));
	//	if (ImGui::BeginCombo("##Texture", ASSETMANAGER->GetTextureName(slotID).c_str()))
	//	{
	//		for (auto& [str, tex] : ASSETMANAGER->Textures)
	//		{
	//			std::filesystem::path tempPath{ str };
	//			if (ImGui::Selectable(tempPath.filename().stem().string().c_str()))
	//			{
	//				//getting the TEXTURE ID HANDLE and not the BOUNDED TEXTURE SLOT
	//				slotID = tex.second;

	//				//check if we selected none
	//				if (!slotID) {
	//					textureID = 0;
	//					break;
	//				}

	//				textureID = tex.first->id;

	//				calPreviewSize(
	//					(float)tex.first->width,
	//					(float)tex.first->height,
	//					textPreviewLim
	//				);
	//				break;
	//			}
	//		}

	//		ImGui::EndCombo();
	//	}
	//	//This allows you to drag into the combo
	//	if (ImGui::BeginDragDropTarget())
	//	{
	//		if (const ImGuiPayload* textureData = ImGui::AcceptDragDropPayload("TEXTURE"))
	//		{
	//			const char* textureName = (const char*)textureData->Data;
	//			textureID = ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->id;
	//			slotID = ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second;

	//			////to get the texture aspect ratio
	//			//textureAspect = (float)ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->height
	//			//	/ (float)ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->width;

	//			////to get the right size to display the texture image
	//			////DOES NOT SCALE WHEN WINDOW DIMENSIONS CHANGE
	//			//textureSize.y = textureSize.x * textureAspect;
	//			calPreviewSize(
	//				(float)ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->width,
	//				(float)ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->height,
	//				textPreviewLim
	//			);
	//		}
	//	}
	//}

	//void InspectorSpriteSheet::SpilttingTheSprites()
	//{
	//	//add row and column input
	//	//Inputs to split the tile map sprite sheet
	//	//no real reason that its a static local variable. You can create member variables if you so please
	//	static int row{}, col{};
	//	//tracks whether changes to the number of tiles has been made
	//	static bool changed{ false };
	//	ImGui::Text("%-17s", "Rows");
	//	ImGui::SameLine();
	//	ImGui::SetNextItemWidth(normalWidth);
	//	if (ImGui::InputInt("##Rows", &row))
	//		changed = true;
	//	ImGui::Text("%-17s", "Columns");
	//	ImGui::SameLine();
	//	ImGui::SetNextItemWidth(normalWidth);
	//	if (ImGui::InputInt("##Columns", &col))
	//		changed = true;

	//	//creating vertex of UV data
	//	//print confirm button if we changed the row and col values
	//	if (changed && ImGui::Button("Confirm")) {
	//		createUV(row, col);
	//		m_row = row;
	//		m_col = col;
	//		changed = false;
	//	}

	//	//display details of each tile here
	//	if (!m_col) return;
	//	if (ImGui::BeginTable("SlicedSpriteSheet", m_col))
	//	{
	//		//for (int tileIndex = {0}; tileIndex < m_row; ++tileIndex)
	//		for (int r = { 0 }; r < m_row; ++r)
	//		{
	//			ImGui::TableNextRow();
	//			for (int c = 0; c < m_col; ++c)
	//			{
	//				ImGui::TableSetColumnIndex(c);
	//				int tileNum = (c + r * m_row) + 1;
	//				ImGui::PushID(tileNum);
	//				ImGui::Text("Sprite %i", tileNum);
	//				if (ImGui::ImageButton((ImTextureID)textureID, ImVec2{ normalWidth, normalWidth }
	//					, ImVec2{ tiles[r + c * m_row].first.first, tiles[r + c * m_row].second.second }
	//					, ImVec2{ tiles[r + c * m_row].second.first, tiles[r + c * m_row].first.second }))
	//				{

	//					DebuggerLogFormat("Sprite %i is selected", tileNum);
	//				}
	//				ImGui::PopID();
	//			}
	//		}
	//		ImGui::EndTable();
	//	}
	//}
}