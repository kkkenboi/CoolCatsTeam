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

		inline bool Empty() { return !m_keyFrames.Size(); }

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
			Vec2<float> timing { keyFrame.m_data };
			keyFrame.m_data.x ? timing.y = m_defaultValue.y : timing.x = m_defaultValue.x;
			m_keyFrames.Insert({keyFrame.m_frame, timing });
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
	template<>
	inline float LBKeyFrameGroup<float>::GetCurrentExact(int exactFrame) const
	{
		// If no keyframe or current is before first keyframe, use default
		if (m_currentIndex >= m_keyFrames.Size() || m_keyFrames[0].m_frame > exactFrame) return m_defaultValue;

		// If after last keyframe, use last keyframe
		if (m_keyFrames[m_keyFrames.Size() - 1].m_frame <= exactFrame) return m_keyFrames[m_keyFrames.Size() - 1].m_data;

		float lerp = (float)(exactFrame - m_keyFrames[m_currentIndex].m_frame) / (float)(m_keyFrames[m_nextIndex].m_frame - m_keyFrames[m_currentIndex].m_frame);

		float interpo = m_keyFrames[m_currentIndex].m_data + (m_keyFrames[m_nextIndex].m_data - m_keyFrames[m_currentIndex].m_data) * lerp;

		return interpo;
	}

	//----------------------------------------------Animation State----------------------------------------------
	class LBAnimationState
	{
	public:
		/*!***********************************************************************
		  \brief
		  Constructors, loads the animation state and its name
		*************************************************************************/
		LBAnimationState() {}
		LBAnimationState(std::string const& name) : m_name(name) {}

		/*!***********************************************************************
		  \brief
		  Starts the animation from the start frame
		*************************************************************************/
		void Start();
		/*!***********************************************************************
		  \brief
		  Starts the animation from the given frame
		*************************************************************************/
		void Start(int frame);

		/*!***********************************************************************
		  \brief
		  Checks current frame and increments key frames if needed (more efficient
		  than UpdateExact)
		*************************************************************************/
		void Update();
		/*!***********************************************************************
		  \brief
		  Checks current frames and finds the exact key frame to use (less efficient,
		  useful for jump between frames or moving backwards)
		*************************************************************************/
		void UpdateExact();

		/*!***********************************************************************
		  \brief
		  Update the end frame based on the largest frame in all animations / given
		  value
		*************************************************************************/
		void UpdateLastFrame();
		void UpdateLastFrame(int newFrame);

		/*!***********************************************************************
		  \brief
		  Checks if the current frame is the last frame in the state
		*************************************************************************/
		inline bool IsLastFrame() const { return m_currentFrame == m_endFrame; }

		/*!***********************************************************************
		 \brief
		 Serializes the Animation State data
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc);
		/*!***********************************************************************
		 \brief
		 Deserializes the Animation State data
		*************************************************************************/
		bool Deserialize(const Value& data);

		std::string m_name{"New Anim"}, m_spriteSheetName{"None"};
		int m_currentFrame{ 0 };
		int m_startFrame{ 0 }, m_endFrame{ 0 };

		LBKeyFrameGroup<bool>			m_active{ true };
		
		LBKeyFrameGroup<Vec2<float>>	m_pos;
		LBKeyFrameGroup<Vec2<float>>	m_scale{ {1.0f, 1.0f} };
		LBKeyFrameGroup<float>			m_rot;

		LBKeyFrameGroup<int>			m_sprite;

	private:
		/*!***********************************************************************
		  \brief
		  Internal update call used by Update() and UpdateExact(), does the actual
		  updating of every Key Frame Group.
		*************************************************************************/
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