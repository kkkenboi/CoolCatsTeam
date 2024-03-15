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
		/*!***********************************************************************
		  \brief
		  Constructor class for a TileMap object where we dump data into the TileMap.

		  \param row
		  The number of rows of the tile map

		  \param columns
		  The number of columns in the tile map
		*************************************************************************/
		TileMap();
		/*!***********************************************************************
		  \brief
		  Constructor class for a TileMap object where we dump data into the TileMap.

		  \param row
		  The number of rows of the tile map

		  \param columns
		  The number of columns in the tile map
		*************************************************************************/
		TileMap(int row, int columns);
		/*!***********************************************************************
		  \brief
		  Constructor class for a TileMap object where we dump data into the TileMap.

		  \param row
		  The number of rows of the tile map

		  \param columns
		  The number of columns in the tile map

		  \param gridVector
		  Initializer list of the values in our Tile map grid
		*************************************************************************/
		TileMap(int row, int columns, std::vector<int> const& gridVector);

		/*!***********************************************************************
		  \brief
		  Function calculates a list of the min max UVs for each tile in the Tile
		  Map and returns a vector container with the data.
		  \return
		  vector of Min Max UV coordinates corresponding to the appropriate tile
		  in the Tile Map.

		  (Note: UV coordinates represented as [0,1] for both axis and 0,0 is
		  bottom left for openGL. For ImGui top left is 0,0.

		  Additionally, the first tile is at the top left of the map
		*************************************************************************/
		std::vector<std::pair<Vec2<float>, Vec2<float>>> minMaxGrid() const;

		/*!***********************************************************************
		  \brief
		  Function saves map data into a json file for the engine to utilize later.
		  \param
		  data The tile map data to save
		  \param alloc
		  The allocator for storing the data in the json file
		  \return
		  Whether the saving was successfull or not
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc);
		/*!***********************************************************************
		  \brief
		  Function loads map data into the engine to utilize.
		  \param
		  data The tile map data to load
		  \return
		  Whether the loading was successfull or not
		*************************************************************************/
		bool Deserialize(const Value& data);

		/*!***********************************************************************
		  \brief
		  Subscript operator to access specific tile.

		  \param index
		  The specific tile we want to access. Tile index start from 0

		  \return
		  The tile value in the tile map
		*************************************************************************/
		inline int operator[](int index) const { return m_grid[index]; }

		/*!***********************************************************************
		  \brief
		  Subscript operator to access specific tile.

		  \param index
		  The specific tile we want to access. Tile index start from 0

		  \return
		  The tile value in the tile map
		*************************************************************************/
		inline int& operator[](int index) { return m_grid[index]; }

		/*!***********************************************************************
		  \brief
		  Getter function returns the number of tiles in the tile map.

		  \return
		  The number of tiles in the tile map.
		*************************************************************************/
		inline int Size() const { return static_cast<int>(m_grid.size()); }

		/*!***********************************************************************
		  \brief
		  Getter function returns the number of rows in the tile map.

		  \return
		  The number of rows in the tile map.
		*************************************************************************/
		inline int getRows() const { return m_rows; }
		/*!***********************************************************************
		  \brief
		  Getter function returns the number of rows in the tile map.

		  \return
		  The number of rows in the tile map.
		*************************************************************************/
		inline int& getRows() { return m_rows; }

		/*!***********************************************************************
		  \brief
		  Getter function returns the number of columns in the tile map.

		  \return
		  The number of columns in the tile map.
		*************************************************************************/
		inline int getCols() const { return m_cols; }
		/*!***********************************************************************
		  \brief
		  Getter function returns the number of columns in the tile map.

		  \return
		  The number of columns in the tile map.
		*************************************************************************/
		inline int& getCols() { return m_cols; }

		/*!***********************************************************************
		  \brief
		  Getter function returns the entire tile map as a vector.

		  \return
		  The entire tile map as a vector.
		*************************************************************************/
		inline std::vector<int>& getGrid() { return m_grid; }
		/*!***********************************************************************
		  \brief
		  Getter function returns the entire tile map as a vector.

		  \return
		  The entire tile map as a vector.
		*************************************************************************/
		inline std::vector<int> const& getGrid() const { return m_grid; }
	};

	class TileMapGroup
	{
	public:
		/*!***********************************************************************
		  \brief
		  Constructor class for a TileMapGroup object.
		*************************************************************************/
		TileMapGroup();
		/*!***********************************************************************
		  \brief
		  Constructor class for a TileMapGroup object.

		  \param layers
		  The number of tile map layers we want to create
		*************************************************************************/
		TileMapGroup(int layers);
		/*!***********************************************************************
		  \brief
		  Single argument Constructor class for a TileMapGroup object.

		  \param tilemaps
		  The pre-existing tilemap already created.
		*************************************************************************/
		TileMapGroup(std::vector<TileMap> const& tileMaps);

		/*!***********************************************************************
		  \brief
		  Saves the data of a group of tilemaps.

		  \param data
		  All the tile map data to save

		  \param alloc
		  The allocator for storing the data in the json file

		  \return
		  Whether the saving was successfull or not
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc);
		/*!***********************************************************************
		  \brief
		  Loading a group of tilemap data.

		  \param data
		  All the tile map data to load

		  \param alloc
		  The allocator for loading the data from the json file

		  \return
		  Whether the loading was successfull or not
		*************************************************************************/
		bool Deserialize(const Value& data);

		/*!***********************************************************************
		  \brief
		  Subscript operator to access specific tilemap at a layer.

		  \param index
		  The specific layer we want to access. Layer index start from 0

		  \return
		  Reference to the tilemap we want.
		*************************************************************************/
		TileMap& operator[](int index);
		/*!***********************************************************************
		  \brief
		  Subscript operator to access specific tilemap at a layer.

		  \param index
		  The specific layer we want to access. Layer index start from 0

		  \return
		  Reference to the tilemap we want.
		*************************************************************************/
		TileMap const& operator[](int index) const;

		/*!***********************************************************************
		  \brief
		  Getter function returns the number of layers the tile map group has.

		  \return
		  The number of layers in the tile map group.
		*************************************************************************/
		inline int Size() const { return static_cast<int>(m_tileMaps.size()); }

		inline std::vector<TileMap>& GetTileMaps() { return m_tileMaps; }

		std::string m_name{"NewMap"};
		std::string m_spriteSheetName{ "None" }; // For serializing
		SpriteSheet m_spriteSheet;

	private:
		std::vector<TileMap> m_tileMaps;
	};

	void BuildMap(TileMapGroup const& tm);
}