/*!************************************************************************
 \file				EditorTMGridView.h
 \author(s)			Ryan Tan Jian Hao
 \par DP email(s):	ryanjianhao.tan@digipen.edu
 \par Course:       CSD2401A
 \date				06/12/2023
 \brief

 This header file contains functions declarations for the Tile map grid view
 in the editor. This will show how to map will look like overall.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Editor.h"
#include "Litterbox/Engine/Layer.h"
#include "LitterBox/Renderer/TileMap.h"
#include "LitterBox/Animation/SpriteSheet.h"
#include <vector>

namespace LB {
	class EditorTMGridView : public Layer
	{
		TileMapGroup m_tiles{3};

		int m_tileSelected{ -1 };
		int m_layer{ 0 };
		
		int m_tmpRow{ m_tiles[m_layer].getRows() }, m_tmpCol{ m_tiles[m_layer].getCols() };

		ImVec2 defaultUV{ 0.f,0.f };
	public:
		/*!***********************************************************************
		  \brief
		  Constructor for the EditorTMGridView class.
		  \return
		  Nothing.
		*************************************************************************/
		EditorTMGridView(std::string layerName);
		/*!***********************************************************************
		  \brief
		  Initializes the EditorTMGridView layer.
		  \return
		  Nothing.
		*************************************************************************/
		void UpdateLayer() override;

		/*!***********************************************************************
		  \brief
		  An inherited Deconstructor
		*************************************************************************/
		void Destroy() {}

		void ResizeGrid(int newRow, int newCol);

		void LoadMap(std::string const& mapName);

		void SaveMap();
	};

	extern EditorTMGridView* EDITORTMGRIDVIEW;
}