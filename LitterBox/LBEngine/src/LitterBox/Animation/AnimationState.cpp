/*!************************************************************************
 \file				AnimationState.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				31-1-2024
 \brief


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
	void AnimationState::Start()
	{
		m_playing = true;
		m_lastIndex = m_index = 0;
	}

	void AnimationState::Update()
	{
		if (!m_playing) return;

		m_timeElapsed += TIME->GetDeltaTime();
		if (m_keyFrames[m_index].m_time >= m_timeElapsed)
		{
			m_lastIndex = m_index;
			m_index = (m_index + 1 % m_keyFrames.size());
			m_timeElapsed = 0.0;
		}
	}

	void AnimationState::Stop()
	{
		m_playing = false;
	}

	bool AnimationState::IsPlaying() const
	{
		return m_playing;
	}

	bool AnimationState::IsNextFrame() const
	{
		return m_lastIndex != m_index;
	}

	int AnimationState::GetCurrentFrame() const
	{
		return m_index;
	}

	void AnimationState::AddFrame(KeyFrame& newFrame)
	{
		m_keyFrames.push_back(newFrame);
	}

	void AnimationState::RemoveFrame(int index)
	{
		m_keyFrames.erase(m_keyFrames.begin() + index);
	}

	std::vector<KeyFrame>& AnimationState::GetFrames()
	{
		return m_keyFrames;
	}

	KeyFrame& AnimationState::operator[](int index)
	{
		return m_keyFrames[index];
	}

	KeyFrame const& AnimationState::operator[](int index) const
	{
		return m_keyFrames[index];
	}

	KeyFrame& AnimationState::At(int index)
	{
		return m_keyFrames.at(index);
	}

	int AnimationState::GetFrameCount() const
	{
		return m_keyFrames.size();
	}

	std::string const& AnimationState::GetName() const
	{
		return m_name;
	}

	void AnimationState::SetName(std::string const& name)
	{
		m_name = name;
	}

	std::string const& AnimationState::GetSpriteSheetName() const
	{
		return m_spriteSheetName;
	}

	void AnimationState::SetSpriteSheetName(std::string const& name)
	{
		m_spriteSheetName = name;
	}

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
}
