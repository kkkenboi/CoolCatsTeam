/*!************************************************************************
 \file				TileMap.h
 \author(s)			Ryan Tan Jian Hao
 \par DP email(s):	ryanjianhao.tan@digipen.edu
 \par Course:       CSD2401A
 \date				25/02/2024
 \brief

 This header file contains the definition of the TileMap class used for
 loading tile map data into a scene.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <string>
#include <vector>
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Renderer/ForwardDeclerators.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Renderer/Renderer.h"
#include "LitterBox/Debugging/Memory.h"
#include "LitterBox/Utils/Math.h"
#include "LitterBox/Serialization/Serializer.h"

namespace LB
{
	class TileMap
	{
		//for iteration through the grid vector and to figure out the min max of all the UVs
		int rows, cols, uvrows, uvcols;
		std::string textureName;
		std::vector<int> grid;

		std::string spriteSheetName{"None"};

	public:
		TileMap() = default;
		TileMap(int row, int columns, int textureRows, int textureColumns,
			std::string texture, std::initializer_list<int>gridvalues);

		TileMap(int row, int columns, int textureRows, int textureColumns,
			std::string const& texture, std::string const& sSheet, std::vector<int> const& gridVector);

		std::vector<std::pair<Vec2<float>, Vec2<float>>> minMaxGrid() const;
		bool Serialize(Value& data, Document::AllocatorType& alloc);
		bool Deserialize(const Value& data);

		inline int operator[](int index) const { return grid[index]; }
		inline int& operator[](int index) { return grid[index]; }

		inline int getRows() const { return rows; }
		inline int getCols() const { return cols; }
		inline std::vector<int>& getGrid() { return grid; }
		inline std::vector<int> const& getGrid() const { return grid; }
		inline const std::string& getTextureName() const { return textureName; }
		inline const std::string& getSpriteSheetName() const { return spriteSheetName; }
	};

	void LoadMap(std::vector<TileMap> const& tm);
}