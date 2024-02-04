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
	void AnimationState::Initialize(IComponent* render)
	{
		m_render = render;

		//Renderer::GRAPHICS->init_anim("mage_float", m_keyFrames, 0.25f, 16);
	}

	void AnimationState::Start()
	{
		reinterpret_cast<CPRender*>(m_render)->play_repeat(m_name);
	}

	void AnimationState::Update()
	{
		m_timeElapsed += TIME->GetDeltaTime();
		//if (m_keyFrames[m_index].m_time <= m_timeElapsed)
		//{

		//}
	}

	void AnimationState::Stop()
	{
		reinterpret_cast<CPRender*>(m_render)->stop_anim();
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

	bool AnimationState::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();

		Value nameValue(m_name.c_str(), alloc);
		data.AddMember("Name", nameValue, alloc);

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
		bool HasName = data.HasMember("Name");
		bool HasFrames = data.HasMember("Frames");

		if (data.IsObject())
		{
			if (HasName)
			{
				const Value& nameValue = data["Name"];
				m_name = nameValue.GetString();
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
