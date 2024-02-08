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
//#include "LitterBox/Components/Component.h"

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
		//----------------------------------------------CONTROLS----------------------------------------------
		void Start();
		void Update();
		void Stop();

		bool IsPlaying() const;
		bool IsNextFrame() const;
		int GetCurrentFrame() const;

		//----------------------------------------------CREATION----------------------------------------------
		std::string const& GetName() const;
		void SetName(std::string const& name);

		std::string const& GetSpriteSheetName() const;
		void SetSpriteSheetName(std::string const& name);

		void AddFrame(KeyFrame& newFrame);
		void RemoveFrame(int index);

		//----------------------------------------------ACCESSORS----------------------------------------------
		KeyFrame& operator[](int index);
		KeyFrame const& operator[](int index) const;

		KeyFrame& At(int index);

		std::vector<KeyFrame>& GetFrames();
		int GetFrameCount() const;

		bool Serialize(Value& data, Document::AllocatorType& alloc); //to save 
		bool Deserialize(const Value& data); //to load
	private:
		bool m_playing { false };

		double m_timeElapsed;
		int m_index, m_lastIndex;

		std::string m_name{ "Unnamed State" }, m_spriteSheetName{ "None" };
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