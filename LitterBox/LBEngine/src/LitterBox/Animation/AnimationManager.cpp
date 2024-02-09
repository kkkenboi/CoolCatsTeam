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
#include "LitterBox/Core/Core.h"

namespace LB
{
	void AnimationManager::Initialize()
	{
		CORE->onPlayingModeToggle.Subscribe(StartAnimators);
	}

	void AnimationManager::Start()
	{
		for (auto& anim : m_animators)
		{
			anim->LoadController();
		}
	}

	void AnimationManager::Update()
	{
		if (!CORE->IsPlaying()) return;

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

		if (CORE->IsPlaying())
		{
			newAnimator->LoadController();
		}
	}

	void AnimationManager::RemoveAnimator(CPAnimator* animatorToRemove)
	{
		auto anim = std::find(m_animators.begin(), m_animators.end(), animatorToRemove);
		if (anim != m_animators.end())
		{
			m_animators.erase(anim);
		}
	}

	void AnimationManager::ClearAnimators()
	{
		m_animators.clear();
	}

	void StartAnimators(bool isPlaying)
	{
		if (isPlaying)
		{
			AnimationManager::Instance()->Start();
		}
		else
		{
			AnimationManager::Instance()->ClearAnimators();
		}
	}
}
