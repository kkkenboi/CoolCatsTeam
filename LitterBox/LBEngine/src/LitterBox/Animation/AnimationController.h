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
		AnimationController() {}
		AnimationController(std::string const& name) : m_name{ name } {}

		//----------------------------------------------CONTROLS----------------------------------------------
		/*!***********************************************************************
		 \brief
		 Initializes the animation controller
		*************************************************************************/
		void Initialize();

		/*!***********************************************************************
		 \brief
		 Updates the animation controller each frame
		*************************************************************************/
		void Update();

		/*!***********************************************************************
		 \brief
		 Sets the current state of the animation controller
		*************************************************************************/
		void SetState(std::string const& stateName);

		/*!***********************************************************************
		 \brief
		 Plays the state of the animation controller
		*************************************************************************/
		void Play(std::string const& stateName);

		/*!***********************************************************************
		 \brief
		 Plays the current state of the animation controller
		*************************************************************************/
		void Play();

		/*!***********************************************************************
		 \brief
		 Stops the current state of the animation controller
		*************************************************************************/
		void Stop();

		/*!***********************************************************************
		 \brief
		 Gets the current sprite sheet name
		*************************************************************************/
		std::string const& GetCurrentSpriteSheet();

		/*!***********************************************************************
		 \brief
		 Checks if the next frame of the animation is playing
		*************************************************************************/
		int IsNextFrame() const;

		/*!***********************************************************************
		 \brief
		 Checks if the animation is playing
		*************************************************************************/
		bool IsPlaying() const;

		//----------------------------------------------CREATION----------------------------------------------
		/*!***********************************************************************
		 \brief
		 Gets the number of states
		*************************************************************************/
		int GetStateCount() const;

		/*!***********************************************************************
		 \brief
		 Gets the actual vector of states
		*************************************************************************/
		std::vector<std::string>& GetStates();

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

		/*!***********************************************************************
		 \brief
		 Gets the state at the index
		*************************************************************************/
		std::string const& operator[](int index) const;

		/*!***********************************************************************
		 \brief
		 Gets the name of the animation controller
		*************************************************************************/
		std::string const& GetName() const;

		/*!***********************************************************************
		 \brief
		 Sets the name of the animation controller
		*************************************************************************/
		void SetName(std::string const& name);

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

	private:
		LBAnimationState m_current; // The current state loaded

		std::string m_name { "No controller" };
		std::vector<std::string> m_states;
	};
}
