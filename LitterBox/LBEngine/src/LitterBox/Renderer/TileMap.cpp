/*!************************************************************************
 \file				TileMap.cpp
 \author(s)			Ryan Tan Jian Hao
 \par DP email(s):	ryanjianhao.tan@digipen.edu
 \par Course:       CSD2401A
 \date				25/02/2024
 \brief

 This source file contains the definitions for some of the member functions
 for the TileMap class

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "TileMap.h"

namespace LB
{
	TileMapGroup::TileMapGroup()
	{
		m_tileMaps.push_back(TileMap());
	}

	TileMapGroup::TileMapGroup(int layers)
	{
		for (int index{ 0 }; index < layers; ++index)
			m_tileMaps.push_back(TileMap());
	}

	TileMapGroup::TileMapGroup(std::vector<TileMap> const& tilemaps) : m_tileMaps{ tilemaps }
	{

	}

	bool TileMapGroup::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		////create json array
		//Value tileMapArray(rapidjson::kArrayType);
		//for (const auto& tilemap : m_tileMaps)
		//{
		//	Value tileMapValue(rapidjson::kObjectType);
		//	tilemap.Serialize(tileMapValue, alloc);
		//	tileMapArray.PushBack(tileMapValue, alloc);
		//}
		//data.AddMember("TileMaps", tileMapArray, alloc);
		return true;
	}

	bool TileMapGroup::Deserialize(const Value& data)
	{
		//bool HasTileMaps = data.HasMember("TileMaps");
		//if (data.IsObject())
		//{
		//	if (HasTileMaps)
		//	{	//we get the gridjson
		//		const Value& tileMapArray = data["TileMaps"].GetArray();
		//		for (rapidjson::SizeType i{}; i < tileMapArray.Size(); ++i)
		//		{	//deserialise then add the values into the grid
		//			TileMap tilemap;
		//			tilemap.Deserialize(tileMapArray[i]);
		//			m_tileMaps.push_back(tilemap);
		//		}
		//	}
		//	return true;
		//}
		return false;
	}

	TileMap& TileMapGroup::operator[](int index)
	{
		return m_tileMaps[index];
	}

	TileMap const& TileMapGroup::operator[](int index) const
	{
		return m_tileMaps[index];
	}

	TileMap::TileMap() : m_rows{ 10 }, m_cols{ 10 }
	{
		
	}

	/*!***********************************************************************
	  \brief
	  Constructor class for a TileMap object where we dump data into the TileMap.

	  \param row
	  The number of rows of the tile map

	  \param columns
	  The number of columns in the tile map

	  \param textureRows
	  The number of times to split the texture horizontally

	  \param textureColumns
	  The number of times to split the texture vertically

	  \param texture
	  The file name of the texture we want to use (without file extension)

	  \param gridvalues
	  Initializer list of the values in our Tile map grid
	*************************************************************************/
	TileMap::TileMap(int row, int columns) : m_rows{row}, m_cols{columns}
	{ }

	TileMap::TileMap(int row, int columns, std::vector<int> const& gridVector) :
		m_rows{ row }, m_cols{ columns }, m_grid { gridVector }
	{ }

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
	std::vector<
		std::pair<Vec2<float>, 
		Vec2<float>>
		> TileMap::minMaxGrid() const
	{
		Vec2<float> default_uv{ 0.f,0.f };
		//return value
		std::vector<
			std::pair<Vec2<float>,
			Vec2<float>>
			> minMax{};
		//resize the vector for easier manipulation
		minMax.resize(m_grid.size());
		//we want to loop through to get the min max values of each tile in the grid
		for(int y{0}; y < m_rows; ++y)
			//for (int x{ 0 }; x < cols; ++x)
			//{
			//	//NOTE GRID VALUES START FROM 1 SO WE SPLIT UP THE TEXTURE AS SUCH
			//	//get the grid value
			//	int gridval{ grid.at(x + y * cols) };

			//	//-if grid value is 0 that means it's an empty tile so we give it 0 UV
			//	//-if the grid has an actual value then we get the min max uv 
			//	//min is bottom left of tile max is top right
			//	if (gridval)
			//	{
			//		//NOTE GRID VALUES START FROM 1 SO WE SPLIT UP THE TEXTURE AS SUCH
			//		//get which column, of the split up texture, the tile wants
			//		int texcol{ gridval % uvcols };
			//		texcol = texcol ? texcol : uvcols; //we need to do this because if the gridval is a multiple of uvcols (i.e. the last tile in the row) then the column value is 0
			//		//figuring out which row, of the split up texture, the tile wants
			//		int texrow{ 1 };
			//		for (; texrow * uvcols < gridval; ++texrow) {};
			//		//we decrement to not overshoot the gridvalue in the array
			//		--texrow;

			//		//NOTE GRID VALUES START FROM 1 SO WE SPLIT UP THE TEXTURE AS SUCH
			//		minMax.at(x + y * cols) = std::make_pair
			//		(
			//			Vec2<float>{(1.f / uvcols)* static_cast<float>(texcol - 1), 1.f - (1.f / uvrows) * static_cast<float>(texrow + 1)}, //min
			//			Vec2<float>{(1.f / uvcols)* static_cast<float>(texcol), 1.f - (1.f / uvrows) * static_cast<float>(texrow)}  //max
			//		);
			//		
			//		if ((x + y * cols) == 0)
			//		{
			//			std::cout << "min: " << minMax[0].first.x << ", " << minMax[0].first.y << std::endl;
			//			std::cout << "max: " << minMax[0].second.x << ", " << minMax[0].second.y << std::endl;
			//		}
			//	}
			//	else
			//	{
			//		minMax.at(x + y * cols) = std::make_pair(default_uv, default_uv);
			//	}
			//}

		return minMax;
	}

	bool TileMap::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		//row cols uvrows uvcols
		//texturename
		//vector<int> grid
		data.AddMember("Rows", m_rows, alloc);
		data.AddMember("Cols", m_cols, alloc);
		//data.AddMember("UVRows", uvrows, alloc);
		//data.AddMember("UVCols", uvcols, alloc);
		//Value textureNameValue(textureName.c_str(), alloc);
		//data.AddMember("TextureName", textureNameValue, alloc);
		if (!m_grid.empty())
		{	//create json array
			Value gridArray(rapidjson::kArrayType);
			for (const auto& val : m_grid)
			{	//then we just add the values
				gridArray.PushBack(val, alloc);
			}
			data.AddMember("Grid", gridArray, alloc);
		}
		return true;
	}

	bool TileMap::Deserialize(const Value& data)
	{
		/*data.AddMember("Rows", rows, alloc);
		data.AddMember("Cols", cols, alloc);
		data.AddMember("UVRows", uvrows, alloc);
		data.AddMember("UVCols", uvcols, alloc);*/
		bool HasRows = data.HasMember("Rows");
		bool HasCols = data.HasMember("Cols");
		//bool HasUVRows = data.HasMember("UVRows");
		//bool HasUVCols= data.HasMember("UVCols");
		//bool HasTextureName = data.HasMember("TextureName");
		bool HasGrid = data.HasMember("Grid");
		if (data.IsObject())
		{
			if (HasRows && HasCols /*&& HasUVCols && HasUVRows*/)
			{
				m_rows = data["Rows"].GetInt();
				m_cols = data["Cols"].GetInt();
				//uvrows = data["UVRows"].GetInt();
				//uvcols = data["UVCols"].GetInt();
				//textureName = data["TextureName"].GetString();
			}
			if (HasGrid)
			{	//we get the gridjson
				const Value& gridArray = data["Grid"].GetArray();
				for (rapidjson::SizeType i{}; i < gridArray.Size(); ++i)
				{	//deserialise then add the values into the grid
					m_grid.push_back(i);
				}
			}
			return true;
		}
		return false;
	}

	/*!***********************************************************************
	  \brief
	  Function creates the game objects, for the background, based on the data
	  in a provided tile map.
	  \param tm
	  The tile map data we want to create the map from
	*************************************************************************/
	void BuildMap(TileMapGroup const& tm)
	{
		//auto minmaxs{ tm.minMaxGrid() };
		//hard coded width and height of tile
		//CHANGE THE W and H VALUES TO CHANGE THE SIZE OF THE TILE
		float w = 250.f, h = 250.f, midx, midy;
		//std::vector<GameObject*> gov{};

		GameObject* parentGO = FACTORY->SpawnGameObject();
		parentGO->SetName("Map");

		GameObject* newGO;
		for (int layer{ 0 }; layer < tm.Size(); ++layer)
		{
			for (int y{ 0 }; y < tm[layer].getRows(); ++y)
			{
				midy = (tm[layer].getRows() - y) * h - h * 0.5f; //get the y value of the tile
				for (int x{ 0 }; x < tm[layer].getCols(); ++x)
				{
					//get the min max UVs
					//auto minmax = minmaxs[x + y * tm.getCols()];
					//we skip if the tile was a default
					//if (minmax.first == minmax.second)
					//	continue;

					//we skip if the tile was a default
					if (tm[layer][x + y * tm[layer].getCols()] == -1)
						continue;

					//add the object into the vector
					newGO = FACTORY->SpawnGameObject();
					//add render component to the newly created game object
					newGO->AddComponent(ComponentTypeID::C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
					midx = w * x + w * 0.5f; //calculate the x value of the tile
					//edit the position of the transform
					newGO->GetComponent<CPTransform>()->SetPosition(Vec2<float>(midx, midy));

					//Initialise the components because stuff is whacky
					newGO->GetComponent<CPTransform>()->Initialise();
					newGO->GetComponent<CPRender>()->Initialise();

					newGO->GetComponent<CPRender>()->texture = LB::ASSETMANAGER->GetTextureUnit(tm.m_spriteSheet.GetPNGRef());
					newGO->GetComponent<CPRender>()->spriteSheetName = tm.m_spriteSheetName;
					newGO->GetComponent<CPRender>()->spriteIndex = tm[layer][x + y * tm[layer].getCols()];
					newGO->GetComponent<CPRender>()->SetSpriteTexture(tm.m_spriteSheetName, tm[layer][x + y * tm[layer].getCols()]);

					newGO->GetComponent<CPRender>()->z_val = tm.Size() - layer;

					//edit the Width and Height of the CPRender
					newGO->GetComponent<CPRender>()->w = w + 5.f;
					newGO->GetComponent<CPRender>()->h = h + 5.f;
					//swap the object types
					Renderer::GRAPHICS->swap_object_type(Renderer::Renderer_Types::RT_BACKGROUND, newGO->GetComponent<CPRender>());

					newGO->GetComponent<CPTransform>()->SetParent(parentGO->GetComponent<CPTransform>());
				}
			}
		}
	}
}