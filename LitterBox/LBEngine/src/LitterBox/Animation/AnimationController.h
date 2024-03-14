/*!************************************************************************
 \file				AnimationController.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				7-1-2024
 \brief

 This header file contains the declaration of the AnimationController class.
 It is a class that holds the logic for the animation state machine.
 It holds the current state, the states of the animation and the controls for the animation.

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
		/*!***********************************************************************
		 \brief
		 Animation controller constructors, overloads for setting a name
		*************************************************************************/
		AnimationController() {}
		AnimationController(std::string const& name) : m_name{ name } {}

		//----------------------------------------------PLAYING----------------------------------------------
		/*!***********************************************************************
		 \brief
		 Updates the animation controller each frame
		*************************************************************************/
		void Update();

		/*!***********************************************************************
		 \brief
		 Sets the current state of the animation controller
		*************************************************************************/
		void Load(std::string const& stateName);

		/*!***********************************************************************
		 \brief
		 Returns true if the current animation is on the last frame
		*************************************************************************/
		inline bool IsLastFrame() { return m_current.IsLastFrame(); }

		/*!***********************************************************************
		 \brief
		 Returns the current animation state
		*************************************************************************/
		inline LBAnimationState& GetCurrentState() { return m_current; }

		/*!***********************************************************************
		 \brief
		 Gets the current sprite sheet name
		*************************************************************************/
		inline std::string const& GetCurrentSpriteSheet() { return m_current.m_spriteSheetName; }

		//----------------------------------------------CREATION----------------------------------------------
		/*!***********************************************************************
		 \brief
		 Gets the number of states
		*************************************************************************/
		inline int Count() { return static_cast<int>(m_states.size()); }

		/*!***********************************************************************
		 \brief
		 Gets the actual vector of states
		*************************************************************************/
		inline std::vector<std::string>& GetStates() { return m_states; }

		/*!***********************************************************************
		 \brief
		 Adds a state to the animation controller
		*************************************************************************/
		void AddState(std::string const& state);

		/*!***********************************************************************
		 \brief
		 Removes a state from the animation controller
		*************************************************************************/
		void RemoveState(int index);

		//----------------------------------------------ACCESSORS----------------------------------------------
		/*!***********************************************************************
		 \brief
		 Gets the state at the index
		*************************************************************************/
		std::string& operator[](int index);
		std::string const& operator[](int index) const;

		/*!***********************************************************************
		 \brief
		 Saves the animation controller
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc);

		/*!***********************************************************************
		 \brief
		 Loads the animation controller
		*************************************************************************/
		bool Deserialize(const Value& data);

		std::string m_name { "No controller" };

	private:
		LBAnimationState m_current; // The current state loaded
		std::vector<std::string> m_states;
	};
}
