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
#include "LitterBox/Utils/SortedVector.h"

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



	//----------------------------------------------Key Frame----------------------------------------------
	template<typename T>
	struct LBKeyFrame
	{
		LBKeyFrame() {}

		LBKeyFrame(int frame) : m_frame(frame) {}

		LBKeyFrame(int frame, T const& data) : m_frame(frame), m_data(data) {}

		/*!***********************************************************************
		 \brief
		 Compares the frame number of the KeyFrame, used for sorting
		*************************************************************************/
		bool operator<(const LBKeyFrame& rhs) const
		{
			return m_frame < rhs.m_frame;
		}

		bool operator==(const LBKeyFrame& rhs) const
		{
			return m_frame == rhs.m_frame;
		}

		/*!***********************************************************************
		 \brief
		 Serializes the KeyFrame data
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc)
		{
			data.SetObject();

			data.AddMember("Frame", m_frame, alloc);
			if constexpr (std::is_fundamental_v<T>)
			{
				data.AddMember("Data", m_data, alloc);
			}
			else
			{
				Value DataValue;
				if (m_data.Serialize(DataValue, alloc))
				{
					data.AddMember("Data", DataValue, alloc);
				}
			}
			return true;
		}

		/*!***********************************************************************
		 \brief
		 Deserializes the KeyFrame data
		*************************************************************************/
		bool Deserialize(const Value& data)
		{
			bool HasData = data.HasMember("Data");
			bool HasTime = data.HasMember("Frame");

			if (data.IsObject())
			{
				if (HasTime)
				{
					m_frame = data["Frame"].GetInt();
				}
				if (HasData)
				{
					if constexpr (std::is_fundamental_v<T>)
					{
						m_data = data["Data"].Get<T>();
					}
					else
					{
						Value const& dataValue = data["Data"];
						m_data.Deserialize(dataValue);
					}
				}
				return true;
			}
			return false;
		}

		int m_frame{};
		T	m_data{};
	};

	//----------------------------------------------Key Frame Group----------------------------------------------
	template <typename T>
	class LBKeyFrameGroup
	{
	public:
		LBKeyFrameGroup() {}
		LBKeyFrameGroup(T const defaultValue ) : m_defaultValue(defaultValue) {}

		void Update(int frame);

		void UpdateExact(int frame);

		T GetCurrent() const
		{
			if (m_currentIndex >= m_keyFrames.Size()) return m_defaultValue;

			return m_keyFrames[m_currentIndex].m_data;
		}

		T GetCurrentExact(int exactFrame) const
		{
			// If no keyframe or current is before first keyframe, use default
			if (m_currentIndex >= m_keyFrames.Size() || m_keyFrames[0].m_frame > exactFrame) return m_defaultValue;

			// If after last keyframe, use last keyframe
			if (m_keyFrames[m_keyFrames.Size() - 1].m_frame < exactFrame) return m_keyFrames[m_keyFrames.Size() - 1].m_data;

			return m_keyFrames[m_currentIndex].m_data;
		}

		void Insert(LBKeyFrame<T> const& keyFrame)
		{
			LBKeyFrame<T>* existingFrame = m_keyFrames.Find(keyFrame);
			if (existingFrame)
			{
				*existingFrame = keyFrame;
			}
			else
			{
				m_keyFrames.Insert(keyFrame);
			}
		}

		inline void Remove(LBKeyFrame<T> const& value) { m_keyFrames.Remove(value); }

		inline void Clear() { m_keyFrames.Clear(); }

		inline SortedVector<LBKeyFrame<T>>& GetData() { return m_keyFrames; }

		inline int GetLargestFrame() { return m_keyFrames.Size() > 0 ? m_keyFrames[m_keyFrames.Size() - 1].m_frame : 0; }

		/*!***********************************************************************
		 \brief
		 Serializes the KeyFrame data
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc)
		{
			data.SetObject();

			if constexpr (std::is_fundamental_v<T>)
			{
				data.AddMember("Default", m_defaultValue, alloc);
			}
			else
			{
				Value defaultValue;
				if (m_defaultValue.Serialize(defaultValue, alloc))
				{
					data.AddMember("Default", defaultValue, alloc);
				}
			}

			Value frameArray(rapidjson::kArrayType);
			for (auto& keyframe : m_keyFrames.GetData())
			{
				Value frameValue;
				if (keyframe.Serialize(frameValue, alloc))
				{
					frameArray.PushBack(frameValue, alloc);
				}
			}
			data.AddMember("Frame Group", frameArray, alloc);
			
			return true;
		}

		/*!***********************************************************************
		 \brief
		 Deserializes the KeyFrame data
		*************************************************************************/
		bool Deserialize(const Value& data)
		{
			bool HasDefault = data.HasMember("Default");
			bool HasFrames = data.HasMember("Frame Group");

			if (data.IsObject())
			{
				if (HasDefault)
				{
					if constexpr (std::is_fundamental_v<T>)
					{
						m_defaultValue = data["Default"].Get<T>();
					}
					else
					{
						Value const& defaultValue = data["Default"];
						m_defaultValue.Deserialize(defaultValue);
					}
				}
				if (HasFrames)
				{
					Value const& frameArray = data["Frame Group"];
					for (SizeType i = 0; i < frameArray.Size(); ++i)
					{
						LBKeyFrame<T> newFrame;
						if (newFrame.Deserialize(frameArray[i]))
						{
							m_keyFrames.Insert(newFrame);
						}
					}

					m_currentIndex = m_nextIndex = 0;
					if (m_keyFrames.Size())
					{
						m_nextIndex = (m_currentIndex + 1) % m_keyFrames.Size();
					}
				}
				return true;
			}
			return false;
		}
		int m_currentIndex{ 0 }, m_nextIndex{ 0 };

	private:
		T m_defaultValue{};
		SortedVector<LBKeyFrame<T>> m_keyFrames;
	};
	// If type is a vector 2, don't fully replace insert and interpolate valyes
	template<>
	inline void LBKeyFrameGroup<Vec2<float>>::Insert(LBKeyFrame<Vec2<float>> const& keyFrame)
	{
		LBKeyFrame<Vec2<float>>* existingFrame = m_keyFrames.Find(keyFrame);
		if (existingFrame)
		{
			if (keyFrame.m_data.x)
			{
				existingFrame->m_data.x = keyFrame.m_data.x;
			}
			else
			{
				existingFrame->m_data.y = keyFrame.m_data.y;
			}
		}
		else
		{
			m_keyFrames.Insert(keyFrame);
		}
	}
	template<>
	inline Vec2<float> LBKeyFrameGroup<Vec2<float>>::GetCurrentExact(int exactFrame) const
	{
		// If no keyframe or current is before first keyframe, use default
		if (m_currentIndex >= m_keyFrames.Size() || m_keyFrames[0].m_frame > exactFrame) return m_defaultValue;

		// If after last keyframe, use last keyframe
		if (m_keyFrames[m_keyFrames.Size() - 1].m_frame <= exactFrame) return m_keyFrames[m_keyFrames.Size() - 1].m_data;

		float lerp = (float)(exactFrame - m_keyFrames[m_currentIndex].m_frame) / (float)(m_keyFrames[m_nextIndex].m_frame - m_keyFrames[m_currentIndex].m_frame);

		Vec2<float> interpo = m_keyFrames[m_currentIndex].m_data + (m_keyFrames[m_nextIndex].m_data - m_keyFrames[m_currentIndex].m_data) * lerp;

		return interpo;
	}

	//----------------------------------------------Animation State----------------------------------------------
	class LBAnimationState
	{
	public:
		LBAnimationState() {}
		LBAnimationState(std::string const& name) : m_name(name) {}

		void Start();
		void Start(int frame);

		void Update();
		void UpdateExact();

		void UpdateLastFrame();
		void UpdateLastFrame(int newFrame);

		bool Serialize(Value& data, Document::AllocatorType& alloc);

		bool Deserialize(const Value& data);

		inline bool IsLastFrame() const { m_currentFrame == m_endFrame; }

		std::string m_name{"New Anim"}, m_spriteSheetName{"None"};
		int m_currentFrame{ 0 };
		int m_startFrame{ 0 }, m_endFrame{ 0 };

		LBKeyFrameGroup<bool>			m_active{ true };
		
		LBKeyFrameGroup<Vec2<float>>	m_pos;
		LBKeyFrameGroup<Vec2<float>>	m_scale{ {1.0f, 1.0f} };
		LBKeyFrameGroup<float>			m_rot;

		LBKeyFrameGroup<int>			m_sprite;

	private:
		void UpdateGroups();
		void UpdateGroupsExact();
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