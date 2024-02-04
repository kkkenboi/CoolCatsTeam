/*!************************************************************************
 \file				AnimationState.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				31-1-2024
 \brief


  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "AnimationState.h"
#include "LitterBox/Engine/Time.h"

namespace LB
{
	void AnimationState::Initialize(CPAnimator* animator)
	{
		//m_animator = animator;
	}

	void AnimationState::Update()
	{
		m_timeElapsed += TIME->GetDeltaTime();
		//if (m_keyFrames[m_index].m_time <= m_timeElapsed)
		//{

		//}
	}

	void AnimationState::AddFrame()
	{

	}

	void AnimationState::RemoveFrame()
	{

	}

	std::vector<KeyFrame>& AnimationState::GetFrames()
	{

	}

	bool AnimationState::Serialize(Value& data, Document::AllocatorType& alloc)
	{

	}

	bool AnimationState::Deserialize(const Value& data)
	{

	}
}
