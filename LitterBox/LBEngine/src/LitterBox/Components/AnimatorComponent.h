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
		 Plays the animation based on the name
		**************************************************************************/
		void Play(std::string const& name);

		/*!************************************************************************
		 \brief
		 Plays the animation based on the name on loop
		**************************************************************************/
		void PlayRepeat(std::string const& name);

		/*!************************************************************************
		 \brief
		 Stops the current animation playing
		**************************************************************************/
		void Stop();

		/*!************************************************************************
		 \brief
		 Gets the controller name
		**************************************************************************/
		std::string const& GetControllerName();

		/*!************************************************************************
		 \brief
		 Sets the controller name
		**************************************************************************/
		void SetControllerName(std::string const& name);

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

	private:
		// Save data prior to anim
		int m_oldID, m_oldSSIndex;
		std::string m_oldSSName;

		bool m_playing{ false }, m_repeating{ false };

		CPRender* m_render;
		AnimationController m_controller; //state machine
	};
}