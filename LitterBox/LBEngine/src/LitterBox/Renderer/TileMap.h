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
		int m_rows, m_cols;
		std::vector<int> m_grid;

	public:
		TileMap();
		TileMap(int row, int columns);
		TileMap(int row, int columns, std::vector<int> const& gridVector);

		std::vector<std::pair<Vec2<float>, Vec2<float>>> minMaxGrid() const;

		bool Serialize(Value& data, Document::AllocatorType& alloc);
		bool Deserialize(const Value& data);

		inline int operator[](int index) const { return m_grid[index]; }
		inline int& operator[](int index) { return m_grid[index]; }

		inline int Size() const { return m_grid.size(); }

		inline int getRows() const { return m_rows; }
		inline int& getRows() { return m_rows; }

		inline int getCols() const { return m_cols; }
		inline int& getCols() { return m_cols; }

		inline std::vector<int>& getGrid() { return m_grid; }
		inline std::vector<int> const& getGrid() const { return m_grid; }
	};

	class TileMapGroup
	{
	public:
		TileMapGroup();
		TileMapGroup(int layers);
		TileMapGroup(std::vector<TileMap> const& tileMaps);

		bool Serialize(Value& data, Document::AllocatorType& alloc);
		bool Deserialize(const Value& data);

		TileMap& operator[](int index);
		TileMap const& operator[](int index) const;

		inline int Size() const { return m_tileMaps.size(); }

		inline std::vector<TileMap>& GetTileMaps() { return m_tileMaps; }

		std::string m_name{"NewMap"};
		std::string m_spriteSheetName{ "None" }; // For serializing
		SpriteSheet m_spriteSheet;

	private:
		std::vector<TileMap> m_tileMaps;
	};

	void BuildMap(TileMapGroup const& tm);
}