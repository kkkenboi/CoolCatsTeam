/*!************************************************************************
 \file				EditorTMGridView.cpp
 \author(s)			Ryan Tan Jian Hao
 \par DP email(s):	ryanjianhao.tan@digipen.edu
 \par Course:       CSD2401A
 \date				11/12/2023
 \brief

 This source file contains function definitions for the Tile map editor layer
 of the Editor. This is to allow the developer to easily and intuitively
 view and create maps using a tiling system.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "EditorTMGridView.h"
#include "EditorTMEditor.h"
#include "LitterBox/Renderer/TileMap.h"

namespace LB
{
	static float columnWidth = 200.0f;
	static float normalWidth = 75.0f;

	EditorTMGridView* EDITORTMGRIDVIEW = nullptr;
	/*!***********************************************************************
	  \brief
	  Constructor for the EditorTMGridView class.
	  \return
	  Nothing.
	*************************************************************************/
	EditorTMGridView::EditorTMGridView(std::string layerName) : Layer(layerName)
	{
		if (!EDITORTMGRIDVIEW)
			EDITORTMGRIDVIEW = this;
		else
			DebuggerLogError("Editor Tile Map Grid View already exists!");

		m_tiles.resize(m_rowNum * m_colNum, -1);
	}
	/*!***********************************************************************
	  \brief
	  Initializes the EditorTMGridView layer.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorTMGridView::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::BeginChild("TileMapSelectionView", ImVec2(-1, ImGui::GetContentRegionAvail().y * 0.92f));

		ImGui::Text("Select Sprite Sheet:");
		if (ImGui::BeginCombo("##Spritesheet", m_spriteSheet.GetName().c_str()))
		{
			for (auto& [str, sSheet] : ASSETMANAGER->SpriteSheets)
			{
				if (ImGui::Selectable(str.c_str()))
				{
					m_spriteSheet = sSheet;
					break;
				}
			}
			ImGui::EndCombo();
		}

		int index{ 0 };
		ImGui::Text("Tiles");
		if (ImGui::ImageButton(0, ImVec2{ normalWidth, normalWidth }))
		{
			m_tileSelected = -1;
		}
		for (int index{ 0 }; index < m_spriteSheet.Size(); ++index)
		{
			if (index % 2 == 0) ImGui::SameLine();
			ImGui::PushID(index);
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(ASSETMANAGER->GetTextureIndex(m_spriteSheet.GetPNGRef()))), ImVec2{ normalWidth, normalWidth }
				, ImVec2{ m_spriteSheet[index].m_min.x, m_spriteSheet[index].m_max.y }
				, ImVec2{ m_spriteSheet[index].m_max.x, m_spriteSheet[index].m_min.y }))
			{
				m_tileSelected = index;
			}
			ImGui::PopID();
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("TileMapGridView", ImVec2(-1, ImGui::GetContentRegionAvail().y * 0.92f));

		ImGui::Text("%s", "Tile Selected");
		ImGui::SameLine();
		if (m_tileSelected < m_spriteSheet.Size())
		{
			ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(ASSETMANAGER->GetTextureIndex(m_spriteSheet.GetPNGRef()))), ImVec2{ 35.0f, 35.0f }
				, ImVec2{ m_spriteSheet[m_tileSelected].m_min.x, m_spriteSheet[m_tileSelected].m_max.y }
				, ImVec2{ m_spriteSheet[m_tileSelected].m_max.x, m_spriteSheet[m_tileSelected].m_min.y });
		}
		else
		{
			ImGui::Image(0, ImVec2{ 35.0f, 35.0f });
		}

		//bool to keep track of grid changes
		static bool confirmation{false};
		//padding from tile selection
		ImGui::SameLine();
		ImGui::Text("%-19s", " ");

		//Setting the rows
		ImGui::SameLine();
		ImGui::Text("Rows:");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(75.f);
		if (ImGui::InputInt("##row", &m_tmpRow))
			confirmation = true;
		
		//Setting the columns
		ImGui::SameLine();
		ImGui::Text("Columns:");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(75.f);
		if (ImGui::InputInt("##column", &m_tmpCol))
			confirmation = true;

		//confirmation button
		if (confirmation) {
			ImGui::SameLine();
			if (ImGui::Button("Confirm")) {
				ResizeGrid(m_tmpRow, m_tmpCol);
				confirmation = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				m_tmpRow = m_rowNum;
				m_tmpCol = m_colNum;
				confirmation = false;
			}
		}

		ImGui::Separator();

		//Just an arbitrary way of calculating button size
		//each image button in the editormapview is a square
		//which may be an issue with aspect ratios
		ImVec2 buttonSize{ ImGui::GetContentRegionAvail() };
		buttonSize.y /= (float)m_rowNum;
		buttonSize.x = buttonSize.y;

		//NOTE: for the table the flags that need to be enabled are
		//the scroll for X and Y axis otherwise you cannot create the map
		if (ImGui::BeginTable("tilemap", m_colNum,	 ImGuiTableFlags_ScrollY 
												|	 ImGuiTableFlags_ScrollX 
												|	 ImGuiTableFlags_NoPadInnerX
												))
		{
			//caching the min and max UVs so if the image button's
			//value is 0 then we just print the same UV to make sure
			//no part of the image is loaded
			ImVec2 min{ defaultUV };
			ImVec2 max{ defaultUV };
			ImTextureID textureID{ 0 };

			//creating the table. NOTE: The editor has a fixed size for now
			//TODO: let player adjust the map size when they want
			for (int row = 0; row < m_rowNum; row++)
			{
				//going through it each row
				ImGui::TableNextRow();
				for (int column = 0; column < m_colNum; column++)
				{
					int Index2D{column + row * m_colNum};
					if (m_tiles[Index2D] >= 0 && m_tiles[Index2D] < m_spriteSheet.Size())
					{
						int index{ m_tiles[Index2D] };
						min = ImVec2{ m_spriteSheet[index].m_min.x, m_spriteSheet[index].m_max.y };
						max = ImVec2{ m_spriteSheet[index].m_max.x, m_spriteSheet[index].m_min.y };
						textureID = (ImTextureID)(uint64_t)ASSETMANAGER->GetTextureIndex(m_spriteSheet.GetPNGRef());
					}
					else
					{
						min = defaultUV;
						max = defaultUV;
						textureID = 0;
					}

					ImGui::TableSetColumnIndex(column);
					//set the button to the texture object selected in EditorTMEditor
					ImGui::PushID(Index2D); //IMPORTANT: this needs to be done otherwise only first button works
					ImGui::Image(textureID, buttonSize, min, max);
					//if (ImGui::ImageButton(textureID, buttonSize, min, max, 0 ))
					//{
					//	m_tiles.at(Index2D) = m_tileSelected;
					//}
					if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
						m_tiles.at(Index2D) = m_tileSelected;
					}
					ImGui::PopID();//IMPORTANT: this needs to be done otherwise only first button works
				}
			}
			ImGui::EndTable();
		}

		ImGui::EndChild();

		// Under the table
		ImGui::Columns(1);
		ImGui::Separator();

		if (ImGui::Button("Print"))
		{
			for (int row = 0; row < m_rowNum; ++row)
			{
				for (int column = 0; column < m_colNum; ++column)
				{
					std::cout << m_tiles.at(column + row * m_colNum) << ' ';
				}
				std::cout << std::endl;
			}
			
			TileMap tm(m_rowNum, m_colNum, m_spriteSheet.m_row, m_spriteSheet.m_col, m_spriteSheet.GetPNGRef(), m_tiles);
			LoadMap(tm);
		}
		
		ImGui::End();
	}

	void EditorTMGridView::ResizeGrid(int newRow, int newCol)
	{
		std::vector<int> newTileMap;
		newTileMap.resize(newRow* newCol, -1);

		for (int row = 0; row < newRow; ++row)
		{
			for (int column = 0; column < newCol; ++column)
			{
				if (column + row * m_colNum < m_tiles.size() && column < m_colNum)
				{
					newTileMap[column + row * newCol] = m_tiles[column + row * m_colNum];
				}
				else
				{
					newTileMap[column + row * newCol] = -1;
				}
			}
		}

		m_rowNum = newRow;
		m_colNum = newCol;
		m_tiles = newTileMap;
	}
}