/*!************************************************************************
 \file				AnimationManager.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				01-02-2024
 \brief

 This header file


  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "LitterBox/Core/System.h"
#include "LitterBox/Components/AnimatorComponent.h"

namespace LB
{
	class AnimationManager : public ISystem, public Singleton<AnimationManager>
	{
	public:
		void Initialize() override;

		void Update() override;

		void Destroy() override;

		void AddAnimator(CPAnimator* newAnimator);

		void ClearAnimators();

	private:
		std::vector<CPAnimator*> m_animators;
	};
}
