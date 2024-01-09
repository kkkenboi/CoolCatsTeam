/*!************************************************************************
 \file				SpriteSheet.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				7-1-2024
 \brief


  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include <string>
#include "LitterBox/Serialization/Serializer.h"

namespace LB
{
	//will need to show it in inspector
	class SpriteSheetData //storing spritesheet data
	{
	public:
		SpriteSheetData(std::string name, int rows, int cols) : m_name{ name }, m_rows{ rows }, m_cols{ cols } {};

		std::string m_name; //PNG name
		int m_rows; //number of animation
		int m_cols; //number of frames

		//used by serializer
		bool Serialize(Value& data, Document::AllocatorType& alloc); //to save 
		bool Deserialize(const Value& data); //to load
	};
}