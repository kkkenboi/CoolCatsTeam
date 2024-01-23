/*!************************************************************************
 \file				EditorAnimationEditor.cpp
 \author(s)			Vanessa Chua Siew Jin, Kenji Brannon Chong
 \par DP email(s):	vanessasiewjin@digipen.edu, kenjibrannon.c@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the EditorAniamtionEditor class and all its functionalities 
 for the aniamtion view layer of the Editor.

 Editor.
  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "EditorAnimationEditor.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Animation/SpriteSheet.h" //Since it uses sprite sheet to animate

/*!***********************************************************************
\brief
Constructor of the Animation Editor
*************************************************************************/

namespace LB
{
	EditorAnimationEditor* EDITORANINMATIONEDITOR = nullptr;

	//variable defines
	//I don't know what this is for.
	//taken from EditorInspector in the Section that
	//draws the ImGui components for CPRender
	static float dropdownWidth = 150.f;
	static float normalWidth = 75.f;

	//height constraint of texture preview
	static float textPreviewLim = 250.f;

	EditorAnimationEditor::EditorAnimationEditor(std::string layerName) :
		Layer(layerName),
		textureID{ 0 },
		slotID{ 0 },
		textureAspect{ 0.f },
		textureSize{ 0.f,0.f },
		tiles{}
	{
		if (!EDITORANINMATIONEDITOR)
			EDITORANINMATIONEDITOR = this;
		else
			DebuggerLogError("Editor Tile Map Editor already exists!");
	}

	//test listing for list box
	const char* list[] = { "First texture", "Second texture" };
	int currItem{ 0 };

	/*!***********************************************************************
	\brief
	 Updating the layers of the animator editor
	*************************************************************************/
	void EditorAnimationEditor::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		ImGui::BeginChild("AnimatorEditor");

		
		//----------------------------------TAKEN FROM EDITOR INSPECTOR CPRENDER--------------------------------
		//--------------------TEXTURE SELECTOR--------------------

		ImGui::Separator();
		//TODO figure out the preview option
		ImGui::Image((ImTextureID)textureID, textureSize, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::Separator();

		if (!textureID) {
			ImGui::EndChild();

			ImGui::End();
			return;
		}

		//SpilttingTheSprites(); //helping to split the size of the spritesheet

		ImGui::EndChild();

		ImGui::End();

	}

	//void EditorAnimationEditor::SpilttingTheSprites()
	//{
		////add row and column input
		////Inputs to split the tile map sprite sheet
		////no real reason that its a static local variable. You can create member variables if you so please
		//static int row{}, col{};
		////tracks whether changes to the number of tiles has been made
		//static bool changed{ false };
		//ImGui::Text("%-17s", "Rows");
		//ImGui::SameLine();
		//ImGui::SetNextItemWidth(normalWidth);
		//if (ImGui::InputInt("##Rows", &row))
		//	changed = true;
		//ImGui::Text("%-17s", "Columns");
		//ImGui::SameLine();
		//ImGui::SetNextItemWidth(normalWidth);
		//if (ImGui::InputInt("##Columns", &col))
		//	changed = true;

		////creating vertex of UV data
		////print confirm button if we changed the row and col values
		//if (changed && ImGui::Button("Confirm")) {
		//	createUV(row, col);
		//	changed = false;
		//}

		////display details of each tile here
		//unsigned int tileIndex{ 0 };
		//int totalNumOfTiles = row * col;
		//for (auto& min_max : tiles) 
		//{
		//	//draw image with the UVs
		//	//ImGui::SameLine();
		//	//The min max is mixed up because ImGui uses a different texel coordinate
		//	//system from openGL so we do the mixing anytime we use ImGui.
		//	//The format of the min max UV coordinates uses OpenGL texel coordinates instead

		//	//this is so nick can choose while tile he wants
		//	//ImGui::ImageButton((ImTextureID)textureID, ImVec2{ normalWidth, normalWidth }
		//	//	, ImVec2{ min_max.first.first, min_max.second.second }
		//	//, ImVec2{ min_max.second.first, min_max.first.second });

		//	//!!!Amadeus need to serialise this part
		//	//This is where nick will choose a tile
		//	//static bool isSelected = false;
		//	std::vector<bool> isSelected(totalNumOfTiles, false);
		//	Vec4<float> btncolour = isSelected.at(tileIndex) ? Vec4<float>(0.5f, 0.5f, 0.5f, 1.0f) : Vec4<float>(1.0f, 1.0f, 1.0f, 1.0f);
		//	//int chosenTile = 0;

		//	//label the tile
		//	ImGui::Text("Tile %d:", tileIndex);
		//	ImGui::PushID(tileIndex);
		//	ImGui::PushStyleColor(ImGuiCol_Button, isSelected.at(tileIndex) ? m_buttonOnColor : m_buttonOffColor);
		//	if (ImGui::ImageButton((ImTextureID)textureID, ImVec2{ normalWidth, normalWidth }
		//		, ImVec2{ min_max.first.first, min_max.second.second }
		//		, ImVec2{ min_max.second.first, min_max.first.second }))
		//	{
		//		DebuggerLogFormat("Tile %i is selected", tileIndex);
		//		if (isSelected.at(tileIndex) == true)
		//		{
		//			isSelected.at(tileIndex) = false;
		//		}
		//		else
		//		{
		//			isSelected.at(tileIndex) = true;
		//		}
		//		//isSelected.at(tileIndex) = !isSelected.at(tileIndex);
		//	}
		//	ImGui::PopStyleColor();
		//	//DebuggerLogFormat("Second: %i", tileIndex);
		//	ImGui::SameLine();
		//	ImGui::PopID();
		//	
		//	tileIndex++;
		//	//currTileIndex = tileIndex;
		//	ImGui::SameLine();
		//}
	//}

	void EditorAnimationEditor::SettingAnimation()
	{
		if (tiles.empty())
		{
			return;
		}

	}
}

