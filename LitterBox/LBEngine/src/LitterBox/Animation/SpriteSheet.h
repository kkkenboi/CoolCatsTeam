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

namespace LB
{
	//will need to show it in inspector
	struct SpriteSheetData //storing spritesheet data
	{
		std::string m_PNGName;
		int m_rows; //number of animation
		int m_cols; //number of frames
	};
}