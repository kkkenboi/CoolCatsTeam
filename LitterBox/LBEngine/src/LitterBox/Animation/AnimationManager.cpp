/*!************************************************************************
 \file				AnimationManager.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				01-02-2024
 \brief

 This source file


  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "AnimationManager.h"

namespace LB
{
	void AnimationManager::Initialize()
	{

	}

	void AnimationManager::Update()
	{
		for (auto& anim : m_animators)
		{
			anim->Update();
		}
	}

	void AnimationManager::Destroy()
	{
		ClearAnimators();
	}

	void AnimationManager::AddAnimator(CPAnimator* newAnimator)
	{
		m_animators.push_back(newAnimator);
	}

	void AnimationManager::ClearAnimators()
	{

	}
}
