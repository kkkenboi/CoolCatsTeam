/*!************************************************************************
 \file				AnimatorComponent.h
 \author(s)			Vanessa Chua Siew Jin, Kenji Brannon Chong
 \par DP email(s):	vanessasiewjin@digipen.edu, kenjibrannon.c@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the implementation of the Animator Component.
 It is used to store the animation controller and play the animation 
 based on the name.


  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "LitterBox/Components/Component.h"
#include "LitterBox/Serialization/Serializer.h"
#include "LitterBox/Animation/AnimationController.h"
#include "LitterBox/Components/RenderComponent.h"

namespace LB
{
	/*!************************************************************************
	\brief
	 Animator Component that inherits from IComponent
	**************************************************************************/
	class CPAnimator : public IComponent
	{
	public:
		/*!************************************************************************
		\brief
		 Getting the type component
		**************************************************************************/
		static ComponentTypeID GetType()
		{
			return C_CPAnimator;
		}

		//----------------------------------------------ANIMATION CONTROLS----------------------------------------------

		/*!************************************************************************
		 \brief
		 Plays the animation and stops (and stays) at the last frame
		**************************************************************************/
		void Play(std::string const& name);

		/*!************************************************************************
		 \brief
		 Plays the animation and resets the object to before the animation began
		**************************************************************************/
		void PlayAndReset(std::string const& name);

		/*!************************************************************************
		 \brief
		 Plays the animation on repeat forever, until Stop() is called or GO is gone
		**************************************************************************/
		void PlayRepeat(std::string const& name);

		/*!************************************************************************
		 \brief
		 Adds the animation to a queue to play after the current animation.
		**************************************************************************/
		void PlayNext(std::string const& name);

		void PlayDelay(std::string const& name, float delay);
		void PlayRepeatDelay(std::string const& name, float delay);

		/*!************************************************************************
		 \brief
		 Pauses/unpauses the current animation
		**************************************************************************/
		void Pause(bool state);

		/*!************************************************************************
		 \brief
		 Stops the current animation and leaves the object at its current state
		**************************************************************************/
		void Stop();
		
		/*!************************************************************************
		 \brief
		 Stops the current animation and resets it to before the animation began
		**************************************************************************/
		void StopAndReset();

		/*!************************************************************************
		 \brief
		 Returns true if the current state is playing
		**************************************************************************/
		inline bool IsPlaying() { return m_playing; }

		/*!************************************************************************
		 \brief
		 Returns true if the state that is playing is the given state
		**************************************************************************/
		inline bool IsPlaying(std::string const& name) { return m_controller.GetCurrentState().m_name == name; }
		
		//----------------------------------------------COMPONENT FUNCTIONS----------------------------------------------

		/*!************************************************************************
		\brief
		 Initialise function where it will initialise the variables
		 load files in initialise
		**************************************************************************/
		void Initialise() override;

		/*!************************************************************************
		 \brief
		 Sets up the animator controller
		**************************************************************************/
		void LoadController();

		/*!************************************************************************
		\brief
		 Update function where it update at every frame
		**************************************************************************/
		void Update() override;

		/*!************************************************************************
		\brief
		 Destroy function
		**************************************************************************/
		void Destroy() override;

		/*!************************************************************************
		 \brief
		 Saves the animator component
		**************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc);

		/*!************************************************************************
		 \brief
		 Loads the animator component
		**************************************************************************/
		bool Deserialize(const Value& data);

		/*!************************************************************************
		 \brief
		 Gets the controller name
		**************************************************************************/
		inline std::string const& GetControllerName() { return m_controller.m_name; }

		/*!************************************************************************
		 \brief
		 Sets the controller name
		**************************************************************************/
		inline void SetControllerName(std::string const& name) { m_controller.m_name = name; }

		//// Trigger setters
		////BOOL
		//void SetBool(std::string const& triggerName, bool state);

		//bool GetBool(std::string const& triggerName);
		//
		////FLOAT
		//void SetFloat(std::string const& triggerName, float value);

		//float GetFloat(std::string const& triggerName);

		////INT
		//void SetInt(std::string const& triggerName, int value);
		//
		//int GetInt(std::string const& triggerName);

		// Modifiers
		float m_playSpeed{ 1.0f }, m_awakeDelay{ 0.0f };

		// For animating without scripts
		std::string m_defaultState{ "None" }, m_delayName{};
		bool m_playOnAwake{ false }, m_repeating{ false }, m_shouldCull{ false };

	private:
		// Save render data prior to anim for resetting
		Vec2<float> m_oldPos, m_oldScale;
		float m_oldRot;
		int m_oldID, m_oldSSIndex;
		std::string m_oldSSName;

		// Internal timer
		float m_elapsedAwakeTime{ 0.0f };
		float m_elapsedTime{ 0.0f }, m_targetTime{ 1.0f / 60.0f };
		std::vector<std::string> m_queue;

		// Flags
		bool m_playing{ false }, m_paused{ false }, m_resetAfterPlay{ false };

		CPRender* m_render{ nullptr };
		CPTransform* m_transform{ nullptr };
		AnimationController m_controller; //state machine
	};
}