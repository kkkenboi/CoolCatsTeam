/*!************************************************************************
 \file				AnimationManager.h
 \author(s)			Vanessa Chua Siew Jin, Ang Jiawei Jarrett
 \par DP email(s):	vanessasiewjin@digipen.edu, a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				01-02-2024
 \brief

 This header file contains the AnimationManager system that is responsible
 for managing all the animators in the game.

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "LitterBox/Core/System.h"
#include "LitterBox/Components/AnimatorComponent.h"
#include "LitterBox/GameLogic/CPPGameLogic/CPPSCameraFollow.h"
#include "LitterBox/Scene/SceneManager.h"

namespace LB
{

	/*!***********************************************************************
	 \brief
	 This system is responsible for managing all the animators in the game.
	 It will update all the animators when the game is playing.
	*************************************************************************/
	class AnimationManager : public ISystem, public Singleton<AnimationManager>
	{
	public:
		/*!***********************************************************************
		 \brief
		 Initializes the AnimationManager system.
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		 \brief
		 Starts the animators when the scene starts.
		*************************************************************************/
		void Start();

		/*!***********************************************************************
		 \brief
		 Updates the animators when the game is playing.
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		 \brief
		 Destroys the AnimationManager system.
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		 \brief
		 Adds a new animator to this system for management.
		*************************************************************************/
		void AddAnimator(CPAnimator* newAnimator);

		/*!***********************************************************************
		 \brief
		 Removes an animator from this system.
		*************************************************************************/
		void RemoveAnimator(CPAnimator* animatorToRemove);

		/*!***********************************************************************
		 \brief
		 Removes all the animators from the game.
		*************************************************************************/
		void ClearAnimators();
		void ClearCameraFollow(std::string const& name);

	private:
		bool m_shouldCheckCulling{ false };
		CPPSCameraFollow* m_cam{ nullptr };
		std::vector<CPAnimator*> m_animators;
	};

	/*!***********************************************************************
	 \brief
	 Global event function that starts the animators when the game is playing.
	*************************************************************************/
	void StartAnimators(bool isPlaying);
	void NewSceneAnimators(Scene* newScene);
}
