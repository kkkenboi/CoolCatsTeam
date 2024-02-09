/*!************************************************************************
 \file				AnimationManager.cpp
 \author(s)			Vanessa Chua Siew Jin, Ang Jiawei Jarrett
 \par DP email(s):	vanessasiewjin@digipen.edu, a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				01-02-2024
 \brief

 This source file contains the implementation of the AnimationManager system
 that is responsible for managing all the animators in the game.

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "AnimationManager.h"
#include "LitterBox/Core/Core.h"

namespace LB
{
	/*!***********************************************************************
	 \brief
	 Initializes the AnimationManager system.
	*************************************************************************/
	void AnimationManager::Initialize()
	{
		CORE->onPlayingModeToggle.Subscribe(StartAnimators);
	}

	/*!***********************************************************************
	 \brief
	 Starts the animators when the scene starts.
	*************************************************************************/
	void AnimationManager::Start()
	{
		for (auto& anim : m_animators)
		{
			anim->LoadController();
		}
	}

	/*!***********************************************************************
	 \brief
	 Updates the animators when the game is playing.
	*************************************************************************/
	void AnimationManager::Update()
	{
		if (!CORE->IsPlaying()) return;

		for (auto& anim : m_animators)
		{
			anim->Update();
		}
	}

	/*!***********************************************************************
	 \brief
	 Destroys the AnimationManager system.
	*************************************************************************/
	void AnimationManager::Destroy()
	{
		ClearAnimators();
	}

	/*!***********************************************************************
	 \brief
	 Adds a new animator to this system for management.
	*************************************************************************/
	void AnimationManager::AddAnimator(CPAnimator* newAnimator)
	{
		m_animators.push_back(newAnimator);

		if (CORE->IsPlaying())
		{
			newAnimator->LoadController();
		}
	}

	/*!***********************************************************************
	 \brief
	 Removes an animator from this system.
	*************************************************************************/
	void AnimationManager::RemoveAnimator(CPAnimator* animatorToRemove)
	{
		auto anim = std::find(m_animators.begin(), m_animators.end(), animatorToRemove);
		if (anim != m_animators.end())
		{
			m_animators.erase(anim);
		}
	}

	/*!***********************************************************************
	 \brief
	 Removes all the animators from the game.
	*************************************************************************/
	void AnimationManager::ClearAnimators()
	{
		m_animators.clear();
	}

	/*!***********************************************************************
	 \brief
	 Global event function that starts the animators when the game is playing.
	*************************************************************************/
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
