/*!************************************************************************
 \file				EditorTMEditor.h
 \author(s)			Ryan Tan Jian Hao
 \par DP email(s):	ryanjianhao.tan@digipen.edu
 \par Course:       CSD2401A
 \date				12/12/2023
 \brief

 This header file contains functions declarations for the Tile map editor
 window. Which allows the user to select the background texture and set
 the UVs for each tile label.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "Editor.h"
#include "LitterBox/Engine/Layer.h"
#include <vector>

namespace LB {
	class EditorTMEditor : public Layer
	{
		//textureID is for actually drawing the texture in ImGui
		//slotID is just so the name prints properly in the ImGui dropdown textbox
		int textureID, slotID;

		//these values are for displaying the texture properly in the editor inspector
		float textureAspect;
		ImVec2 textureSize;

		//store the min max UV for each tile respectively
		std::vector<
			std::pair<
				std::pair<float, float>,
				std::pair<float, float>
			>
		> tiles;

		//function to generate all the uv data for the tilemap
		void createUV(int rows, int cols);

		//function for generating appropriate size preview
		void calPreviewSize(float width, float height, float previewLimit);
	public:
		EditorTMEditor(std::string layerName);

		void UpdateLayer() override;

		void Destroy() {}

		inline unsigned int getNumOfTiles() { return tiles.size(); }
		inline int getTextureID() { return textureID; }
		inline auto getMMUV(int index) { return tiles.at(index); }
	};

	extern EditorTMEditor* EDITORTMEDITOR;
}