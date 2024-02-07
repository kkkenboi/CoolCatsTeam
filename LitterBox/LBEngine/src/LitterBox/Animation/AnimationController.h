/*!************************************************************************
 \file				AnimationController.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				7-1-2024
 \brief

 This header file


  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "LitterBox/GameLogic/StateMachine.h"
#include "AnimationState.h"

namespace LB
{
	

	class AnimationController
	{
	public:
		void Initialize();

		void Update();

		void SetState(std::string const& stateName);

		void Play(std::string const& stateName);
		void Play();

		void Stop();

		std::string const& GetName() const;
		void SetName(std::string const& name);

		bool Serialize(Value& data, Document::AllocatorType& alloc); //to save 
		bool Deserialize(const Value& data); //to load

	private:
		std::string m_name;

		AnimationState* m_current;

		std::vector<AnimationState> m_states;
	};
}
