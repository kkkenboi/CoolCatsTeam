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
	/*!***********************************************************************
	  \brief
	  Constructor for the EditorTMGridView class.
	  \return
	  Nothing.
	*************************************************************************/
	EditorTMGridView::EditorTMGridView(std::string layerName) : Layer(layerName),
		rowNum{10}, colNum{20}, tileSelected{0},
		tiles{}, tiles_names{}, tmpRow{rowNum}, tmpCol{colNum}
	{
		if (!EDITORTMGRIDVIEW)
			EDITORTMGRIDVIEW = this;
		else
			DebuggerLogError("Editor Tile Map Grid View already exists!");

		tiles.resize(rowNum * colNum, 0);
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
		if (ImGui::DragInt("##row", &tmpRow))
			confirmation = !confirmation ? true : false;
		
		//Setting the columns
		ImGui::SameLine();
		ImGui::Text("Columns:");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(75.f);
		if (ImGui::DragInt("##column", &tmpCol))
			confirmation = !confirmation ? true : false;

		//confirmation button
		if (confirmation) {
			ImGui::SameLine();
			if (ImGui::Button("Confirm")) {
				rowNum = tmpRow;
				colNum = tmpCol;
				tiles.resize(rowNum * colNum, 0);
				confirmation = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				tmpRow = rowNum;
				tmpCol = colNum;
				confirmation = false;
			}
		}

		//Just an arbitrary way of calculating button size
		//each image button in the editormapview is a square
		//which may be an issue with aspect ratios
		ImVec2 buttonSize{ ImGui::GetContentRegionAvail() };
		buttonSize.y /= (float)rowNum;
		buttonSize.x = buttonSize.y;

		//NOTE: for the table the flags that need to be enabled are
		//the scroll for X and Y axis otherwise you cannot create the map
		if (ImGui::BeginTable("table1", colNum,	 ImGuiTableFlags_ScrollY 
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
			//just a variable to store the value of the image button in
			//tiles vector
			int index{ 0 };
			//creating the table. NOTE: The editor has a fixed size for now
			//TODO: let player adjust the map size when they want
			for (int row = 0; row < rowNum; row++)
			{
				//going through it each row
				ImGui::TableNextRow();
				for (int column = 0; column < colNum; column++)
				{
					//check that we have valid tile at the image button
					//also need to check that there are UVs already created
					if (EDITORTMEDITOR->getNumOfTiles() &&
						(index = tiles.at(column + row * colNum) - 1) >= 0) {
						//getting the min max UVs and placing them in the
						//order that ImGui Follows. Which is flipped Y axis
						//to OpenGL
						min = {
							EDITORTMEDITOR->getMMUV(index).first.first,
							EDITORTMEDITOR->getMMUV(index).second.second
						};
						max = {
							EDITORTMEDITOR->getMMUV(index).second.first,
							EDITORTMEDITOR->getMMUV(index).first.second
						};
						//We also set the texture data because if not then
						//The tile becomes fully blue instead of black
						textureID = (ImTextureID)EDITORTMEDITOR->getTextureID();
					}
					else {
						min = defaultUV;
						max = defaultUV;
						textureID = 0;
					}


					ImGui::TableSetColumnIndex(column);
					//set the button to the texture object selected in EditorTMEditor
					ImGui::PushID(column + row * colNum); //IMPORTANT: this needs to be done otherwise only first button works
					if (ImGui::ImageButton(textureID, buttonSize, min, max, 0 ))
					{
						tiles.at(column + row * colNum) = tileSelected;
					}
					ImGui::PopID();//IMPORTANT: this needs to be done otherwise only first button works
				}
			}
			ImGui::EndTable();
		}

		ImGui::EndChild();
		
		ImGui::End();
	}
}