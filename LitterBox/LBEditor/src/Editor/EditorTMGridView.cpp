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

namespace LB
{
	EditorTMGridView* EDITORTMGRIDVIEW = nullptr;

	EditorTMGridView::EditorTMGridView(std::string layerName) : Layer(layerName),
		rowNum{10}, colNum{20}, tileSelected{0},
		tiles{}, tiles_names{}
	{
		if (!EDITORTMGRIDVIEW)
			EDITORTMGRIDVIEW = this;
		else
			DebuggerLogError("Editor Tile Map Grid View already exists!");

		tiles.resize(rowNum * colNum, 0);
	}

	void EditorTMGridView::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		ImGui::BeginChild("TileMapGridView");

		ImGui::Text("%-19s", "Tile Selected");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(75.f);
		//combo to get drop down box of available tiles
		//that have been split up in TMEditor
		if (ImGui::BeginCombo("##Selected Tile", std::to_string(tileSelected).c_str())) {
			for (int i{ 0 }; i < EDITORTMEDITOR->getNumOfTiles() + 1; ++i) {
				//scuff way of converting int to c-style string
				if (ImGui::Selectable(std::to_string(i).c_str())) {
					//IMPORTANT NOTE: tileSelected is the INDEX of the UV min_max data
					//however in ImGui, the tiles start labeled from 1
					tileSelected = i;
				}
			}

			ImGui::EndCombo();
		}

		ImVec2 buttonSize{ ImGui::GetContentRegionAvail() };
		buttonSize.y /= (float)rowNum;
		buttonSize.x = buttonSize.y;

		if (ImGui::BeginTable("table1", colNum, ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX))
		{
			ImVec2 min{ defaultUV };
			ImVec2 max{ defaultUV };
			ImTextureID textureID{ 0 };
			for (int row = 0; row < rowNum; row++)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < colNum; column++)
				{
					//check that we have valid tile at the image button
					if (EDITORTMEDITOR->getNumOfTiles() &&
						tiles.at(column + row * colNum) != 0) {
						min = {
							EDITORTMEDITOR->getMMUV(tiles.at(column + row * colNum)).first.first,
							EDITORTMEDITOR->getMMUV(tiles.at(column + row * colNum)).second.second
						};
						max = {
							EDITORTMEDITOR->getMMUV(tiles.at(column + row * colNum)).second.first,
							EDITORTMEDITOR->getMMUV(tiles.at(column + row * colNum)).first.second
						};
						textureID = (ImTextureID)EDITORTMEDITOR->getTextureID();
					}
					else {
						min = defaultUV;
						max = defaultUV;
						textureID = 0;
					}


					ImGui::TableSetColumnIndex(column);
					//set the button to the texture object selected in EditorTMEditor
					ImGui::PushID(column + row * colNum);
					if (ImGui::ImageButton("Button", textureID, buttonSize, min, max))
					{
						tiles.at(column + row * colNum) = tileSelected;
					}
					ImGui::PopID();
				}
			}
			ImGui::EndTable();
		}

		ImGui::EndChild();
		
		ImGui::End();
	}
}