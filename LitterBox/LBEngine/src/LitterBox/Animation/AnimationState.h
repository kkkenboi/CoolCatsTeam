/*!************************************************************************
 \file				AnimationState.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				31-1-2024
 \brief


  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <vector>
#include "LitterBox/Serialization/Serializer.h"

namespace LB
{
	// Forward declarations
	class CPAnimator;

	struct KeyFrame
	{
		int m_frame;
		float m_time;
	};

	class AnimationState
	{
	public:
		void Initialize(CPAnimator* animator);

		void Update();

		void AddFrame();
		void RemoveFrame();

		std::vector<KeyFrame>& GetFrames();

		bool Serialize(Value& data, Document::AllocatorType& alloc); //to save 
		bool Deserialize(const Value& data); //to load

	private:
		std::vector<KeyFrame> m_keyFrames;
		double m_timeElapsed;

		//CPAnimator* m_animator;
		//std::vector<AnimationTransition> m_transitions;
	};

	//class TransitionParameter
	//{
	//public:
	//	template<typename T>
	//	void Set(T newValue)
	//	{
	//		m_value = newValue;
	//	}

	//private:
	//	std::string m_name;
	//	int m_value;
	//};

	//class AnimationTransition
	//{
	//public:
	//	bool ShouldTransition();

	//private:
	//	AnimationState m_destination;
	//};
}