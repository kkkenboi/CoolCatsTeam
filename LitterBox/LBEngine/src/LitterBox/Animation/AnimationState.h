/*!************************************************************************
 \file				AnimationState.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				7-1-2024
 \brief


  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox/Animation/SpriteSheet.h"
#include <list>

namespace LB
{
	struct KeyFrame
	{
		int m_frame;
		float m_time;
	};


	class AnimationStateClass
	{
	public:



	private:
		SpriteSheetData m_spriteSheetInfo; //a struct of all the sprite sheet info
		std::list<KeyFrame> m_keyFrames;
	};
}