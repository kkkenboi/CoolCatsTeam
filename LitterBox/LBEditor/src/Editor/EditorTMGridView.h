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
#include <vector>

namespace LB {
	class EditorTMGridView : public Layer
	{
		int colNum, rowNum, tileSelected, tmpRow, tmpCol;
		std::vector<int> tiles;
		std::vector<char*> tiles_names;

		ImVec2 defaultUV{ 0.f,0.f };
	public:
		EditorTMGridView(std::string layerName);

		void UpdateLayer() override;

		void Destroy() {}
	};

	extern EditorTMGridView* EDITORTMGRIDVIEW;
}