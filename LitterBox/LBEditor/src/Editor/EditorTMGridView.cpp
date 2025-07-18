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
#include "EditorAssets.h"

namespace LB
{
	static float columnWidth = 200.0f;
	static float normalWidth = 75.0f;

	ImVec4 hoverTint = ImVec4(0.8f, 0.8f, 0.8f, 0.2f); // Tint when hovered (light gray)

	EditorTMGridView* EDITORTMGRIDVIEW = nullptr;
	/*!***********************************************************************
	  \brief
	  Constructor for the EditorTMGridView class.
	  \return
	  Nothing.
	*************************************************************************/
	EditorTMGridView::EditorTMGridView(std::string layerName) : Layer(layerName), m_tilesVisibility(4, true)
	{
		if (!EDITORTMGRIDVIEW)
			EDITORTMGRIDVIEW = this;
		else
			DebuggerLogError("Editor Tile Map Grid View already exists!");

		for (auto& tilemap : m_tiles.GetTileMaps())
		{
			tilemap.getGrid().resize(tilemap.getRows() * tilemap.getCols(), -1);
		}

		strcpy_s(m_textBuffer, sizeof(m_textBuffer), m_tiles.m_name.c_str());
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

		// Draw first column, spritesheet and tile selection
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::BeginChild("TileMapSelectionView", ImVec2(-1, ImGui::GetContentRegionAvail().y * 0.92f));

		ImGui::Text("Loaded Tile Map:");
		if (ImGui::InputText("##Name", m_textBuffer, 256))
		{
			m_tiles.m_name = m_textBuffer;
		}

		ImGui::Text("Select Sprite Sheet:");
		if (ImGui::BeginCombo("##Spritesheet", m_tiles.m_spriteSheet.GetName().c_str()))
		{
			for (auto& [str, sSheet] : ASSETMANAGER->SpriteSheets)
			{
				if (ImGui::Selectable(str.c_str()))
				{
					m_tiles.m_spriteSheet = sSheet;
					m_tiles.m_spriteSheetName = str;
					break;
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Tiles");
		if (ImGui::ImageButton(0, ImVec2{ normalWidth, normalWidth }))
		{
			m_tileSelected = -1;
		}
		for (int index{ 0 }; index < m_tiles.m_spriteSheet.Size(); ++index)
		{
			if (index % 2 == 0) ImGui::SameLine();
			ImGui::PushID(index);
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(ASSETMANAGER->GetTextureIndex(m_tiles.m_spriteSheet.GetPNGRef()))), ImVec2{ normalWidth, normalWidth }
				, ImVec2{ m_tiles.m_spriteSheet[index].m_min.x, m_tiles.m_spriteSheet[index].m_max.y }
				, ImVec2{ m_tiles.m_spriteSheet[index].m_max.x, m_tiles.m_spriteSheet[index].m_min.y }))
			{
				m_tileSelected = index;
			}
			ImGui::PopID();
		}
		ImGui::EndChild();

		// Next column, the actual tile map grid
		ImGui::NextColumn();

		ImGui::BeginChild("TileMapGridView", ImVec2(-1, ImGui::GetContentRegionAvail().y * 0.92f));

		ImGui::Text("%s", "Tile Selected");
		ImGui::SameLine();
		if (m_tileSelected < m_tiles.m_spriteSheet.Size())
		{
			ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(ASSETMANAGER->GetTextureIndex(m_tiles.m_spriteSheet.GetPNGRef()))), ImVec2{ 35.0f, 35.0f }
				, ImVec2{ m_tiles.m_spriteSheet[m_tileSelected].m_min.x, m_tiles.m_spriteSheet[m_tileSelected].m_max.y }
				, ImVec2{ m_tiles.m_spriteSheet[m_tileSelected].m_max.x, m_tiles.m_spriteSheet[m_tileSelected].m_min.y });
		}
		else
		{
			ImGui::Image(0, ImVec2{ 35.0f, 35.0f });
		}
		ImGui::SameLine();
		ImGui::Text("%-3s", " ");

		//bool to keep track of grid changes
		static bool confirmation{ false };

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
		if (confirmation) 
		{
			ImGui::SameLine();
			if (ImGui::Button("Confirm")) {
				ResizeGrid(m_tmpRow, m_tmpCol);
				confirmation = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				m_tmpRow = m_tiles.GetTileMaps()[m_layer].getRows();
				m_tmpCol = m_tiles.GetTileMaps()[m_layer].getCols();
				confirmation = false;
			}
		}

		ImGui::Text("Zoom level: %.2f", m_zoom);
		ImGui::SameLine();
		ImGui::Text("%-3s", " ");

		// Set layer
		ImGui::SameLine();
		ImGui::Text("Current:");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(75.f);
		if (ImGui::InputInt("##layer", &m_layer))
		{
			m_layer = std::clamp(m_layer, 0, m_tiles.Size() - 1);
			m_tmpRow = m_tiles[m_layer].getRows();
			m_tmpCol = m_tiles[m_layer].getCols();
		}
		// Set layer name
		ImGui::SameLine();
		switch (m_layer)
		{
		case 0:
			ImGui::Text("Grass");
			break;
		case 1:
			ImGui::Text("Sand");
			break;
		case 2:
			ImGui::Text("Wall (Back)");
			break;
		case 3:
			ImGui::Text("Wall (Front)");
			break;
		default:
			ImGui::Text("Unnamed ");
			break;
		}
		ImGui::SameLine();
		ImGui::Text("%-3s", " ");

		// Toggle visibility of each layer
		bool isActive{ m_tilesVisibility[0] };

		ImGui::SameLine();
		if (ImGui::Checkbox("Grass", &isActive))
		{
			m_tilesVisibility[0] = isActive;
		}
		ImGui::SameLine();
		isActive = m_tilesVisibility[1];
		if (ImGui::Checkbox("Sand", &isActive))
		{
			m_tilesVisibility[1] = isActive;
		}
		ImGui::SameLine();
		isActive = m_tilesVisibility[2];
		if (ImGui::Checkbox("Wall (B)", &isActive))
		{
			m_tilesVisibility[2] = isActive;
		}
		ImGui::SameLine();
		isActive = m_tilesVisibility[3];
		if (ImGui::Checkbox("Wall (F)", &isActive))
		{
			m_tilesVisibility[3] = isActive;
		}

		ImGui::Separator();

		int rowNum{ m_tiles[m_layer].getRows() };
		int colNum{ m_tiles[m_layer].getCols() };

		//Just an arbitrary way of calculating button size
		//each image button in the editormapview is a square
		//which may be an issue with aspect ratios
		ImVec2 buttonSize{ ImGui::GetContentRegionAvail() };
		buttonSize.y /= (float)rowNum * m_zoom;
		buttonSize.x = buttonSize.y;

		//NOTE: for the table the flags that need to be enabled are
		//the scroll for X and Y axis otherwise you cannot create the map
		if (ImGui::BeginTable("tilemap", colNum,	 ImGuiTableFlags_ScrollY
												|	 ImGuiTableFlags_ScrollX 
												|	 ImGuiTableFlags_NoPadInnerX
												))
		{
			// Handle scrolling input if hovered
			if (ImGui::IsWindowHovered() && ImGui::GetIO().MouseWheel != 0.0f)
			{
				// If ctrl is held, zoom in/out
				if (INPUT->IsKeyPressed(KeyCode::KEY_LEFTCONTROL))
				{
					m_zoom += ImGui::GetIO().MouseWheel > 0.0f ? -m_zoomStep * static_cast<float>(TIME->GetUnscaledDeltaTime()) : m_zoomStep * static_cast<float>(TIME->GetUnscaledDeltaTime());
				}
				// If left shift is held, scroll horizontally
				else if (INPUT->IsKeyPressed(KeyCode::KEY_LEFTSHIFT))
				{
					ImGui::SetScrollX(ImGui::GetScrollX() - ImGui::GetIO().MouseWheel * 100);
				}
				// Else, scroll vertically
				else
				{
					ImGui::SetScrollY(ImGui::GetScrollY() - ImGui::GetIO().MouseWheel * 100);
				}
			}

			//caching the min and max UVs so if the image button's
			//value is 0 then we just print the same UV to make sure
			//no part of the image is loaded
			ImVec2 min{ defaultUV };
			ImVec2 max{ defaultUV };
			ImTextureID textureID{ 0 };

			// Remove padding between cell rows
			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
			
			for (int row = 0; row < rowNum; row++)
			{
				//going through it each row
				ImGui::TableNextRow();
				for (int column = 0; column < colNum; column++)
				{
					ImGui::TableSetColumnIndex(column);

					int layer{ 0 };
					int Index2D{column + row * colNum};
					bool isDrawn{ false };

					ImGui::PushID(Index2D); //IMPORTANT: this needs to be done otherwise only first button works
					while (layer < m_tiles.Size())
					{
						if (!m_tilesVisibility[layer])
						{
							++layer;
							continue;
						}

						if (m_tiles[layer][Index2D] >= 0 && m_tiles[layer][Index2D] < m_tiles.m_spriteSheet.Size())
						{
							if (!isDrawn)
							{
								int index_other{ m_tiles[layer][Index2D] };
								min = ImVec2{ m_tiles.m_spriteSheet[index_other].m_min.x, m_tiles.m_spriteSheet[index_other].m_max.y };
								max = ImVec2{ m_tiles.m_spriteSheet[index_other].m_max.x, m_tiles.m_spriteSheet[index_other].m_min.y };
								textureID = (ImTextureID)(uint64_t)ASSETMANAGER->GetTextureIndex(m_tiles.m_spriteSheet.GetPNGRef());

								ImGui::Image(textureID, buttonSize, min, max);

								isDrawn = true;
							}
							else
							{
								int index_other{ m_tiles[layer][Index2D] };
								min = ImVec2{ m_tiles.m_spriteSheet[index_other].m_min.x, m_tiles.m_spriteSheet[index_other].m_min.y };
								max = ImVec2{ m_tiles.m_spriteSheet[index_other].m_max.x, m_tiles.m_spriteSheet[index_other].m_max.y };
								ImGui::GetWindowDrawList()->AddImage(textureID, ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + buttonSize.x, ImGui::GetCursorScreenPos().y - buttonSize.y * 1.1f), min, max);
							}
						}
						++layer;
					}
					if (!isDrawn)
					{
						min = defaultUV;
						max = defaultUV;
						textureID = 0;
						ImGui::Image(textureID, buttonSize, min, max);
					}
					if (ImGui::IsItemHovered()) 
					{
						// Add a transulcent white tint when hovered
						ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + buttonSize.x, ImGui::GetCursorScreenPos().y - buttonSize.y * 1.1f), ImGui::GetColorU32(hoverTint));

						if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
						{
							m_tiles[m_layer].getGrid().at(Index2D) = m_tileSelected;
						}
						else if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
						{
							m_tiles[m_layer].getGrid().at(Index2D) = -1;
						}
						else if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
						{
							m_tileSelected = m_tiles[m_layer].getGrid().at(Index2D);
						}
					}
					ImGui::PopID();//IMPORTANT: this needs to be done otherwise only first button works
				}
			}
			ImGui::PopStyleVar();
			ImGui::EndTable();
		}
		ImGui::EndChild();

		// Under the table
		ImGui::Columns(1);
		ImGui::Separator();

		if (ImGui::Button("Build Map"))
		{
			BuildMap(m_tiles);
		}
		ImGui::SameLine();
		if (ImGui::Button("Save Map"))
		{
			SaveMap();
		}
		
		ImGui::End();
	}

	/*!***********************************************************************
	  \brief
	  Resizes the tilemap grid
	*************************************************************************/
	void EditorTMGridView::ResizeGrid(int newRow, int newCol)
	{
		int colNum { m_tiles[m_layer].getCols() };

		for (int layer{ 0 }; layer < m_tiles.Size(); ++layer)
		{
			std::vector<int> newTileMap;
			newTileMap.resize(newRow * newCol, -1);

			for (int row = 0; row < newRow; ++row)
			{
				for (int column = 0; column < newCol; ++column)
				{
					if (column + row * colNum < m_tiles[layer].Size() && column < colNum)
					{
						newTileMap[column + row * newCol] = m_tiles[layer][column + row * colNum];
					}
					else
					{
						newTileMap[column + row * newCol] = -1;
					}
				}
			}
			m_tiles[layer].getGrid() = newTileMap;
			m_tiles[layer].getRows() = newRow;
			m_tiles[layer].getCols() = newCol;
		}
	}

	/*!***********************************************************************
	  \brief
	  Loads a map from the file given
	*************************************************************************/
	void EditorTMGridView::LoadMap(std::string const& mapName)
	{
		JSONSerializer::DeserializeFromFile(mapName.c_str(), m_tiles);
		m_layer = 0;
		m_tileSelected = -1;
		m_tmpRow = m_tiles[m_layer].getRows();
		m_tmpCol = m_tiles[m_layer].getCols();
		strcpy_s(m_textBuffer, sizeof(m_textBuffer), m_tiles.m_name.c_str());
	}

	/*!***********************************************************************
	  \brief
	  Saves the current loaded map
	*************************************************************************/
	void EditorTMGridView::SaveMap()
	{
		std::filesystem::path tilemapPath{ m_tiles.m_name + ".tilemap" };
		JSONSerializer::SerializeToFile((EDITORASSETS->currentDirectory/tilemapPath).string(), m_tiles);
	}
}