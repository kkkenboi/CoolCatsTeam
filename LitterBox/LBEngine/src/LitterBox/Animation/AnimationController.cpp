/*!************************************************************************
 \file				AnimationController.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				7-1-2024
 \brief

 This source file


  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "AnimationController.h"

namespace LB
{
	void AnimationController::Initialize()
	{

	}

	void AnimationController::Update()
	{
		m_current->Update();
	}

	void AnimationController::Play()
	{
		m_current->Start();
	}

	void AnimationController::Stop()
	{
		m_current->Stop();
	}

	bool AnimationController::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		return true;
	}

	bool AnimationController::Deserialize(const Value& data)
	{
		return false;
	}
}
