/*!************************************************************************
 \file				AnimationState.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				31-1-2024
 \brief

 This file contains the class for the AnimationState. It is used to store
 the frame number and time to be used in the animation state.

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
	/*!***********************************************************************
	 \brief
	 The KeyFrame is used to store the frame number and time to be used
	 in the animation state
	*************************************************************************/
	struct KeyFrame
	{
		int m_frame;
		float m_time;

		/*!***********************************************************************
		 \brief
		 Serializes the KeyFrame data
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc)
		{
			data.SetObject();

			data.AddMember("Frame", m_frame, alloc);
			data.AddMember("Time", m_time, alloc);

			return true;
		}

		/*!***********************************************************************
		 \brief
		 Deserializes the KeyFrame data
		*************************************************************************/
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
		/*!***********************************************************************
		 \brief
		 Plays the animation
		*************************************************************************/
		void Start();

		/*!***********************************************************************
		 \brief
		 Updates the animation each frame
		*************************************************************************/
		void Update();

		/*!***********************************************************************
		 \brief
		 Stops the animation from playing
		*************************************************************************/
		void Stop();

		/*!***********************************************************************
		 \brief
		 Returns if the animation playing?
		*************************************************************************/
		bool IsPlaying() const;

		/*!***********************************************************************
		 \brief
		 Checks if the next frame has changed
		*************************************************************************/
		bool IsNextFrame() const;

		/*!***********************************************************************
		 \brief
		 Returns the current frame index
		*************************************************************************/
		int GetCurrentFrame() const;

		//----------------------------------------------CREATION----------------------------------------------

		/*!***********************************************************************
		 \brief
		 Returns the name of the state
		*************************************************************************/
		std::string const& GetName() const;

		/*!***********************************************************************
		 \brief
		 Sets the name of the state
		*************************************************************************/
		void SetName(std::string const& name);

		/*!***********************************************************************
		 \brief
		 Returns the name of the spritesheet the state is using
		*************************************************************************/
		std::string const& GetSpriteSheetName() const;

		/*!***********************************************************************
		 \brief
		 Sets the name of the spritesheet of the state
		*************************************************************************/
		void SetSpriteSheetName(std::string const& name);

		/*!***********************************************************************
		 \brief
		 Adds a keyframe to the state
		*************************************************************************/
		void AddFrame(KeyFrame& newFrame);

		/*!***********************************************************************
		 \brief
		 Removes a keyframe from the state
		*************************************************************************/
		void RemoveFrame(int index);

		//----------------------------------------------ACCESSORS----------------------------------------------

		/*!***********************************************************************
		 \brief
		 Gets the keyframe at the index
		*************************************************************************/
		KeyFrame& operator[](int index);

		/*!***********************************************************************
		 \brief
		 Gets the keyframe at the index  
		*************************************************************************/
		KeyFrame const& operator[](int index) const;

		/*!***********************************************************************
		 \brief
		 Gets the keyframe at the index
		*************************************************************************/
		KeyFrame& At(int index);

		/*!***********************************************************************
		 \brief
		 Gets the keyframe vector
		*************************************************************************/
		std::vector<KeyFrame>& GetFrames();

		/*!***********************************************************************
		 \brief
		 Returns the total number of frames
		*************************************************************************/
		int GetFrameCount() const;

		/*!***********************************************************************
		 \brief
		 Saves the state
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc);

		/*!***********************************************************************
		 \brief
		 Loads the state
		*************************************************************************/
		bool Deserialize(const Value& data); //to load
	private:
		bool m_playing { false };

		double m_timeElapsed{}; 
		int m_index{}, m_lastIndex{};

		std::string m_name{ "Unnamed State" }, m_spriteSheetName{ "None" };
		std::vector<KeyFrame> m_keyFrames{};

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