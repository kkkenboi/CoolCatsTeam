/*!************************************************************************
 \file				AnimationState.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				31-1-2024
 \brief

 This source file contains the implementation of the AnimationState class
 which holds the keyframes of an animation and plays them.

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "AnimationState.h"
#include "LitterBox/Engine/Time.h"

#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Components/RenderComponent.h"

namespace LB
{
	/*!***********************************************************************
	 \brief
	 Plays the animation
	*************************************************************************/
	void AnimationState::Start()
	{
		m_playing = true;
		m_lastIndex = m_index = 0;
	}

	/*!***********************************************************************
	\brief
	Updates the animation each frame
	*************************************************************************/
	void AnimationState::Update()
	{
		if (!m_playing) return;

		m_timeElapsed += TIME->GetDeltaTime();
		if (m_timeElapsed >= m_keyFrames[m_index].m_time)
		{
			m_lastIndex = m_index;
			++m_index;
			m_timeElapsed = 0.0;

			if (m_index >= m_keyFrames.size())
			{
				m_lastIndex = m_index = 0;
				m_playing = false;
			}
		}
	}

	/*!***********************************************************************
	 \brief
	 Stops the animation from playing
	*************************************************************************/
	void AnimationState::Stop()
	{
		m_playing = false;
	}

	/*!***********************************************************************
	 \brief
	 Returns if the animation playing?
	*************************************************************************/
	bool AnimationState::IsPlaying() const
	{
		return m_playing;
	}

	/*!***********************************************************************
	 \brief
	 Checks if the next frame has changed
	*************************************************************************/
	bool AnimationState::IsNextFrame() const
	{
		return m_lastIndex != m_index;
	}

	/*!***********************************************************************
	 \brief
	 Returns the current frame index
	*************************************************************************/
	int AnimationState::GetCurrentFrame() const
	{
		return m_keyFrames[m_index].m_frame;
	}

	/*!***********************************************************************
	 \brief
	 Adds a keyframe to the state
	*************************************************************************/
	void AnimationState::AddFrame(KeyFrame& newFrame)
	{
		m_keyFrames.push_back(newFrame);
	}

	/*!***********************************************************************
	 \brief
	 Removes a keyframe from the state
	*************************************************************************/
	void AnimationState::RemoveFrame(int index)
	{
		m_keyFrames.erase(m_keyFrames.begin() + index);
	}

	/*!***********************************************************************
	 \brief
	 Gets the keyframe vector
	*************************************************************************/
	std::vector<KeyFrame>& AnimationState::GetFrames()
	{
		return m_keyFrames;
	}

	/*!***********************************************************************
	 \brief
	 Gets the keyframe at the index
	*************************************************************************/
	KeyFrame& AnimationState::operator[](int index)
	{
		return m_keyFrames[index];
	}

	/*!***********************************************************************
	 \brief
	 Gets the keyframe at the index
	*************************************************************************/
	KeyFrame const& AnimationState::operator[](int index) const
	{
		return m_keyFrames[index];
	}

	/*!***********************************************************************
	 \brief
	 Gets the keyframe at the index
	*************************************************************************/
	KeyFrame& AnimationState::At(int index)
	{
		return m_keyFrames.at(index);
	}

	/*!***********************************************************************
	 \brief
	 Returns the total number of frames
	*************************************************************************/
	int AnimationState::GetFrameCount() const
	{
		return static_cast<int>(m_keyFrames.size());
	}

	/*!***********************************************************************
	 \brief
	 Returns the name of the state
	*************************************************************************/
	std::string const& AnimationState::GetName() const
	{
		return m_name;
	}

	/*!***********************************************************************
	 \brief
	 Sets the name of the state
	*************************************************************************/
	void AnimationState::SetName(std::string const& name)
	{
		m_name = name;
	}

	/*!***********************************************************************
	 \brief
	 Returns the name of the spritesheet the state is using
	*************************************************************************/
	std::string const& AnimationState::GetSpriteSheetName() const
	{
		return m_spriteSheetName;
	}

	/*!***********************************************************************
	 \brief
	 Sets the name of the spritesheet of the state
	*************************************************************************/
	void AnimationState::SetSpriteSheetName(std::string const& name)
	{
		m_spriteSheetName = name;
	}

	/*!***********************************************************************
	 \brief
	 Saves the state
	*************************************************************************/
	bool AnimationState::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();

		Value nameValue(m_name.c_str(), alloc);
		data.AddMember("Name", nameValue, alloc);

		Value ssNameValue(m_spriteSheetName.c_str(), alloc);
		data.AddMember("SpriteSheet Name", ssNameValue, alloc);

		Value frameArray(rapidjson::kArrayType);
		for (auto& keyframe : m_keyFrames)
		{
			Value frameValue;
			if (keyframe.Serialize(frameValue, alloc))
			{
				frameArray.PushBack(frameValue, alloc);
			}
		}
		data.AddMember("Frames", frameArray, alloc);

		return true;
	}

	/*!***********************************************************************
	 \brief
	 Loads the state
	*************************************************************************/
	bool AnimationState::Deserialize(const Value& data)
	{
		m_keyFrames.clear();

		bool HasName = data.HasMember("Name");
		bool HasSSName = data.HasMember("SpriteSheet Name");
		bool HasFrames = data.HasMember("Frames");

		if (data.IsObject())
		{
			if (HasName)
			{
				const Value& nameValue = data["Name"];
				m_name = nameValue.GetString();
			}
			if (HasSSName)
			{
				const Value& ssNameValue = data["SpriteSheet Name"];
				m_spriteSheetName = ssNameValue.GetString();
			}
			if (HasFrames)
			{
				KeyFrame newFrame;
				const Value& frameArray = data["Frames"].GetArray();
				for (rapidjson::SizeType i{}; i < frameArray.Size(); ++i)
				{
					newFrame.Deserialize(frameArray[i]);
					m_keyFrames.push_back(newFrame);
				}
			}
			return true;
		}

		return false;
	}


	//----------------------------------------------Key Frame Group----------------------------------------------
	template <typename T>
	void LBKeyFrameGroup<T>::Update(int frame)
	{
		if (m_keyFrames.Size() < 2)
		{
			m_currentIndex = m_nextIndex = 0;
			return;
		}

		while (frame >= m_keyFrames[m_nextIndex].m_frame)
		{
			m_currentIndex = m_nextIndex;
			++m_nextIndex;

			if (m_nextIndex >= m_keyFrames.Size())
			{
				m_nextIndex = 0;
				return;
			}
		}
	}

	template <typename T>
	void LBKeyFrameGroup<T>::UpdateExact(int frame)
	{
		m_currentIndex = m_nextIndex = 0;
		if (m_keyFrames.Size() < 2)
		{
			return;
		}

		while (frame >= m_keyFrames[m_nextIndex].m_frame)
		{
			m_currentIndex = m_nextIndex;
			++m_nextIndex;

			if (m_nextIndex >= m_keyFrames.Size())
			{
				m_nextIndex = 0;
				return;
			}
		}
	}

	//----------------------------------------------Animation State----------------------------------------------
	void LBAnimationState::Start()
	{
		Start(m_startFrame);
	}

	void LBAnimationState::Start(int frame)
	{
		m_currentFrame = frame - 1;
		UpdateExact();
	}

	void LBAnimationState::Update()
	{
		++m_currentFrame;
		if (m_currentFrame > m_endFrame)
		{
			m_currentFrame = m_startFrame;
		}

		UpdateGroups();
	}

	void LBAnimationState::UpdateExact()
	{
		++m_currentFrame;
		if (m_currentFrame > m_endFrame)
		{
			m_currentFrame = m_startFrame;
		}

		UpdateGroupsExact();
	}

	void LBAnimationState::UpdateLastFrame()
	{
		int largestFrame = std::max(m_active.GetLargestFrame(), m_pos.GetLargestFrame());
		largestFrame = std::max(largestFrame, m_scale.GetLargestFrame());
		largestFrame = std::max(largestFrame, m_rot.GetLargestFrame());
		largestFrame = std::max(largestFrame, m_sprite.GetLargestFrame());

		if (largestFrame > m_endFrame)
		{
			m_endFrame = largestFrame;
		}
	}

	void LBAnimationState::UpdateLastFrame(int newFrame)
	{
		// Only if end frame is smaller
		if (m_endFrame < newFrame)
		{
			m_endFrame = newFrame;
		}
	}

	void LBAnimationState::UpdateGroups()
	{
		m_active.Update(m_currentFrame);
		m_pos.Update(m_currentFrame);
		m_scale.Update(m_currentFrame);
		m_rot.Update(m_currentFrame);
		m_sprite.Update(m_currentFrame);
	}

	void LBAnimationState::UpdateGroupsExact()
	{
		m_active.UpdateExact(m_currentFrame);
		m_pos.UpdateExact(m_currentFrame);
		m_scale.UpdateExact(m_currentFrame);
		m_rot.UpdateExact(m_currentFrame);
		m_sprite.UpdateExact(m_currentFrame);
	}

	bool LBAnimationState::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();

		Value nameValue(m_name.c_str(), alloc);
		data.AddMember("Name", nameValue, alloc);

		Value ssNameValue(m_spriteSheetName.c_str(), alloc);
		data.AddMember("SpriteSheet Name", ssNameValue, alloc);

		Value activeValue;
		if (m_active.Serialize(activeValue, alloc))
		{
			data.AddMember("Active", activeValue, alloc);
		}
		Value posValue;
		if (m_pos.Serialize(posValue, alloc))
		{
			data.AddMember("Position", posValue, alloc);
		}
		Value scaleValue;
		if (m_scale.Serialize(scaleValue, alloc))
		{
			data.AddMember("Scale", scaleValue, alloc);
		}
		Value rotValue;
		if (m_rot.Serialize(rotValue, alloc))
		{
			data.AddMember("Rotation", rotValue, alloc);
		}
		Value spriteValue;
		if (m_sprite.Serialize(spriteValue, alloc))
		{
			data.AddMember("Sprite", spriteValue, alloc);
		}

		data.AddMember("Start", m_startFrame, alloc);
		data.AddMember("End", m_endFrame, alloc);

		return true;
	}

	bool LBAnimationState::Deserialize(const Value& data)
	{
		m_active.Clear();
		m_pos.Clear();
		m_scale.Clear();
		m_rot.Clear();
		m_sprite.Clear();

		bool HasName = data.HasMember("Name");
		bool HasSSName = data.HasMember("SpriteSheet Name");

		bool HasActive = data.HasMember("Active");
		bool HasPos = data.HasMember("Position");
		bool HasScale = data.HasMember("Scale");
		bool HasRot = data.HasMember("Rotation");
		bool HasSprite = data.HasMember("Sprite");

		bool HasStart = data.HasMember("Start");
		bool HasEnd = data.HasMember("End");

		if (data.IsObject())
		{
			if (HasName)
			{
				const Value& nameValue = data["Name"];
				m_name = nameValue.GetString();
			}
			if (HasSSName)
			{
				const Value& ssNameValue = data["SpriteSheet Name"];
				m_spriteSheetName = ssNameValue.GetString();
			}

			if (HasActive)
			{
				const Value& activeValue = data["Active"];
				m_active.Deserialize(activeValue);
			}
			if (HasPos)
			{
				const Value& posValue = data["Position"];
				m_pos.Deserialize(posValue);
			}
			if (HasScale)
			{
				const Value& scaleValue = data["Scale"];
				m_scale.Deserialize(scaleValue);
			}
			if (HasRot)
			{
				const Value& rotValue = data["Rotation"];
				m_rot.Deserialize(rotValue);
			}
			if (HasSprite)
			{
				const Value& spriteValue = data["Sprite"];
				m_sprite.Deserialize(spriteValue);
			}

			if (HasStart)
			{
				const Value& startValue = data["Start"];
				m_startFrame = startValue.GetInt();
			}
			if (HasEnd)
			{
				const Value& endValue = data["End"];
				m_endFrame = endValue.GetInt();
			}

			return true;
		}
		return false;
	}
}
