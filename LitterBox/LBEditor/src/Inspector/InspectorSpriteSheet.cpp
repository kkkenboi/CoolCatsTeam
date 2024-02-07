/*!************************************************************************
 \file				InspectorSpriteSheet.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains the class is where it updates the spritesheet, split,
 save, etc the spritesheet in the inspector. Definitions.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include <imgui.h>
#include "InspectorSpriteSheet.h"

namespace LB
{
	//setting variables for it size
	static float fullWidth;
	static float columnWidth = 250.0f;
	static float normalWidth = 150.f;
	
	/*!***********************************************************************
	  \brief
	  Initialize function, where it init the textureID, slotID, textureAspect,
	  textureSize
	*************************************************************************/
    void InspectorSpriteSheet::Initialize()
    {
		textureID = 0;
		slotID = 0;
		textureAspect = 0.0f;
		textureSize = ImVec2(0.0f, 0.0f);
    }

	/*!***********************************************************************
	  \brief
	 Where ImGui will be to show on the Inspector, runs the splitting sprite
	 function
	*************************************************************************/
    void InspectorSpriteSheet::UpdateLayer()
    {
		// Name
		ImGui::Text("%-17s", "Name");
		ImGui::SameLine();
		if (ImGui::InputText("##Name", m_name, 256))
		{
			m_inspectedSheet.SetName(m_name);
		}

        // PNGRef
        ImGui::Text("%-17s", "Texture");
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

		SpilttingTheSprites(); //split the sprite at the bottom of the inspector for the spritesheet
    }

	/*!***********************************************************************
	  \brief
	 Saving the SpriteSheet, save to member row and col and it also 
	 where it serialise the spritesheet
	*************************************************************************/
	void InspectorSpriteSheet::SaveSpriteSheet()
	{
		// TO BE REFACTORED
		m_inspectedSheet.m_row = m_row;
		m_inspectedSheet.m_col = m_col;

		ASSETMANAGER->GetSpriteSheet(m_fileName) = m_inspectedSheet;

		JSONSerializer::SerializeToFile(m_inspectedSheet.GetName(), m_inspectedSheet);
	}

	// Load from AssetManager
	/*!***********************************************************************
	 \brief
	Loads the AssetManager and deserailise the spritesheet
	*************************************************************************/
	void InspectorSpriteSheet::LoadSpriteSheet(std::string name)
	{
		m_fileName = name;
		m_inspectedSheet = ASSETMANAGER->GetSpriteSheet(m_fileName);

		m_textureID = ASSETMANAGER->GetTextureIndex(m_inspectedSheet.GetPNGRef());
		strcpy_s(m_name, sizeof(m_name), m_inspectedSheet.GetName().c_str());

		// TO BE REFACTORED
		m_row = m_inspectedSheet.m_row;
		m_col = m_inspectedSheet.m_col;
	}

	/*!***********************************************************************
	  \brief
	 Creating the UV of the spritesheet of the spritesheet
	*************************************************************************/
	void InspectorSpriteSheet::createUV(int rows, int cols) {
		if (rows <= 0 || cols <= 0) {
			DebuggerLogError("Number of Rows or Columns input for the tilemap is not more than 0");
			return;
		}

		float x_inc{ 1.f / static_cast<float>(cols) }, y_inc{ 1.f / static_cast<float>(rows) };

		//tiles.clear();
		m_inspectedSheet.Sprites().clear();

		// TO BE REFACTORED
		m_inspectedSheet.m_row = m_row;
		m_inspectedSheet.m_col = m_col;

		for (int y{ rows }; y > 0; --y)
			for (int x{ 0 }; x < cols; ++x) {
				//0,0 is bottom left of texture. Therefore, if the first texture is
				//the top left tile, then the top left would be 0,1
				//the min would be the same x axis but 1 increment down from 1
				//the max would be 1 increment right from 0 and the same y axis
				std::pair<float, float> min{ x * x_inc, (y - 1) * y_inc };
				std::pair<float, float> max{ (x + 1) * x_inc, y * y_inc };

				m_inspectedSheet.Slice({ min.first, min.second }, { max.first, max.second });
				//tiles.emplace_back(min, max);
			}
	}

	/*!***********************************************************************
	  \brief
	 Shows Imgui to allow user to manual and auto slice (auto slice for now),
	 user can set the amount of rows and cols for how many sprites they want
	 to split into from the spritesheet
	*************************************************************************/
	void InspectorSpriteSheet::SpilttingTheSprites()
	{
		static bool changed{ false };

		ImGui::Text("Auto Slicer");
		ImGui::Text("Rows");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(normalWidth);
		if (ImGui::InputInt("##Rows", &m_row))
		{
			changed = true;
		}
		ImGui::SameLine();
		ImGui::Text("Columns");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(normalWidth);
		if (ImGui::InputInt("##Columns", &m_col))
		{
			changed = true;
		}
		ImGui::PushID("AutoSlicer");
		if (changed && ImGui::Button("Auto Slice", ImVec2(columnWidth, 25.0f)))
		{
			createUV(m_row, m_col);
			changed = false;
		}
		ImGui::PopID();

		ImGui::Dummy(ImVec2(0.0f, 35.0f));

		ImGui::Text("Sprites");

		//display details of each tile here
		if (!m_inspectedSheet.Size()) return;
		if (ImGui::BeginTable("SlicedSpriteSheet", m_inspectedSheet.m_col))
		{
			//Creating a table to place the sprites evenly by its row and cols
			for (int r = { 0 }; r < m_inspectedSheet.m_row; ++r) //go thru rows
			{
				ImGui::TableNextRow(); //go next row
				for (int c = 0; c < m_inspectedSheet.m_col; ++c) //go thru cols
				{
					ImGui::TableSetColumnIndex(c);
					int tileNum = (c + r * m_inspectedSheet.m_col) + 1;
					ImGui::PushID(tileNum);
					ImGui::Text("Sprite %i", tileNum);
					if (ImGui::ImageButton((ImTextureID)ASSETMANAGER->GetTextureIndex(m_inspectedSheet.GetPNGRef()), ImVec2{normalWidth, normalWidth}
						, ImVec2{ m_inspectedSheet[c + r * m_inspectedSheet.m_row].m_min.x, m_inspectedSheet[c + r * m_inspectedSheet.m_row].m_max.y }
						, ImVec2{ m_inspectedSheet[c + r * m_inspectedSheet.m_row].m_max.x, m_inspectedSheet[c + r * m_inspectedSheet.m_row].m_min.y }))
					{

						DebuggerLogFormat("Sprite %i is selected Min %f %f Max %f %f", c + r * m_inspectedSheet.m_col,
							m_inspectedSheet[c + r * m_inspectedSheet.m_row].m_min.x, m_inspectedSheet[c + r * m_inspectedSheet.m_row].m_max.y,
							m_inspectedSheet[c + r * m_inspectedSheet.m_row].m_max.x, m_inspectedSheet[c + r * m_inspectedSheet.m_row].m_min.y);
					}
					ImGui::PopID();
				}
			}
			ImGui::EndTable();
		}
	}
}