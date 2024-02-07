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
#include "LitterBox/Animation/SpriteSheet.h"
#include "LitterBox/Components/Component.h"

namespace LB
{
	struct KeyFrame
	{
		int m_frame;
		float m_time;

		bool Serialize(Value& data, Document::AllocatorType& alloc)
		{
			data.SetObject();

			data.AddMember("Frame", m_frame, alloc);
			data.AddMember("Time", m_time, alloc);

			return true;
		}

		bool Deserialize(const Value& data)
		{
			bool HasFrame = data.HasMember("Frame");
			bool HasTime = data.HasMember("Time");

			if (HasFrame && HasTime)
			{
				m_frame = data["Frame"].GetInt();
				m_time = data["Time"].GetFloat();

				return true;
			}
			return true;
		}
	};

	class AnimationState
	{
	public:
		void Initialize(IComponent* render);

		void Start();
		void Update();
		void Stop();

		std::string const& GetName() const;
		void SetName(std::string const& name);

		void AddFrame(KeyFrame& newFrame);
		void RemoveFrame(int index);

		std::vector<KeyFrame>& GetFrames();
		int GetFrameCount() const;

		bool Serialize(Value& data, Document::AllocatorType& alloc); //to save 
		bool Deserialize(const Value& data); //to load

	private:
		SpriteSheet* m_spriteSheet;
		IComponent* m_render;
		double m_timeElapsed;
		int m_index;

		std::string m_name;
		std::vector<KeyFrame> m_keyFrames;

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