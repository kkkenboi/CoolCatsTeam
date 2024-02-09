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
		/*!***********************************************************************
		  \brief
		  Calculates the minimum and maximum UVs required for splitting the texture
		  given.

		  \param rows
		  The number of times to split the texture horizontally
		  \param cols
		  The number of times to split the texture vertically
		*************************************************************************/
		void createUV(int rows, int cols);

		//function for generating appropriate size preview
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
		void calPreviewSize(float width, float height, float previewLimit);
	public:
		/*!***********************************************************************
		  \brief
		  Constructor for the EditorTMEditor class.
		  \param
		  The string identifier for the Layering system to know which window this is.
		*************************************************************************/
		EditorTMEditor(std::string layerName);

		/*!***********************************************************************
		  \brief
		  Update function that is called every frame.
		*************************************************************************/
		void UpdateLayer() override;

		/*!***********************************************************************
		  \brief
		  An inherited Deconstructor
		*************************************************************************/
		void Destroy() {}

		/*!***********************************************************************
		  \brief
		  Getter function to return the number of tiles created after splitting
		  the texture
		  \return
		  Number of tiles that texture has been split into
		*************************************************************************/
		inline size_t getNumOfTiles() { return tiles.size(); }

		/*!***********************************************************************
		  \brief
		  Gets the Texture ID of the texture for ImGui to use.
		  
		  NOTE: THIS IS TEXTURE ID AND NOT TEXTURE UNIT THAT TEXTURE IS BOUND TO

		  \return
		  The ID of the loaded texture in openGL
		*************************************************************************/
		inline int getTextureID() { return textureID; }

		/*!***********************************************************************
		  \brief
		  Gets the minimum and maximum UVs of a specific tile
		  \return
		  Shoult be a pair of pairs (I know its not vec2 leave me alone)
		*************************************************************************/
		inline auto getMMUV(int index) { return tiles.at(index); }
	};

	extern EditorTMEditor* EDITORTMEDITOR;
}