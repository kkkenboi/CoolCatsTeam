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
	TileMap::TileMap(int row, int columns, int textureRows, int textureColumns, 
					std::string texture, std::initializer_list<int> gridvalues) :
		rows{row}, cols{columns}, uvrows{textureRows}, uvcols{textureColumns},
		textureName{texture}, grid{gridvalues}
	{
	}

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
		> TileMap::minMaxGrid()
	{
		Vec2<float> default_uv{ 0.f,0.f };
		//return value
		std::vector<
			std::pair<Vec2<float>,
			Vec2<float>>
			> minMax{};
		//resize the vector for easier manipulation
		minMax.resize(grid.size());
		//we want to loop through to get the min max values of each tile in the grid
		for(int y{0}; y < rows; ++y)
			for (int x{ 0 }; x < cols; ++x)
			{
				//NOTE GRID VALUES START FROM 1 SO WE SPLIT UP THE TEXTURE AS SUCH
				//get the grid value
				int gridval{ grid.at(x + y * cols) };

				std::cout << gridval << " " << std::endl;

				//-if grid value is 0 that means it's an empty tile so we give it 0 UV
				//-if the grid has an actual value then we get the min max uv 
				//min is bottom left of tile max is top right
				if (gridval)
				{
					//NOTE GRID VALUES START FROM 1 SO WE SPLIT UP THE TEXTURE AS SUCH
					//get which column, of the split up texture, the tile wants
					int texcol{ gridval % uvcols };
					//figuring out which row, of the split up texture, the tile wants
					int texrow{ 1 };
					for (; texrow * uvcols < gridval; ++texrow) {};
					//we decrement to not overshoot the gridvalue in the array
					--texrow;

					//NOTE GRID VALUES START FROM 1 SO WE SPLIT UP THE TEXTURE AS SUCH
					minMax.at(x + y * cols) = std::make_pair
					(
						Vec2<float>{(1.f / uvcols)* static_cast<float>(texcol - 1), 1.f - (1.f / uvrows) * static_cast<float>(texrow + 1)}, //min
						Vec2<float>{(1.f / uvcols)* static_cast<float>(texcol), 1.f - (1.f / uvrows) * static_cast<float>(texrow)}  //max
					);
					
					if ((x + y * cols) == 0)
					{
						std::cout << "min: " << minMax[0].first.x << ", " << minMax[0].first.y << std::endl;
						std::cout << "max: " << minMax[0].second.x << ", " << minMax[0].second.y << std::endl;
					}
				}
				else
				{
					minMax.at(x + y * cols) = std::make_pair(default_uv, default_uv);
				}
			}

		return minMax;
	}
}