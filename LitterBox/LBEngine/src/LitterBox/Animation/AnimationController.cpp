/*!************************************************************************
 \file				AnimationController.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				7-1-2024
 \brief

 This source file


  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "AnimationController.h"
#include "LitterBox/Serialization/AssetManager.h"

namespace LB
{
	void AnimationController::Initialize()
	{

	}

	void AnimationController::Update()
	{
		m_current->Update();
	}

	void AnimationController::SetState(std::string const& stateName)
	{
		for (auto& state : m_states)
		{
			if (state.GetName() == stateName)
			{
				m_current = &state;
				break;
			}
		}
	}

	void AnimationController::Play(std::string const& stateName)
	{
		SetState(stateName);
		Play();
	}

	void AnimationController::Play()
	{
		m_current->Start();
	}

	void AnimationController::Stop()
	{
		m_current->Stop();
	}

	std::string const& AnimationController::GetCurrentSpriteSheet()
	{
		return m_current->GetSpriteSheetName();
	}

	int AnimationController::IsNextFrame()
	{
		if (m_current->IsNextFrame())
		{
			return m_current->GetCurrentFrame();
		}
		return 0;
	}

	int AnimationController::GetStateCount() const
	{
		return m_states.size();
	}

	std::vector<AnimationState>& AnimationController::GetStates()
	{
		return m_states;
	}

	void AnimationController::AddState(AnimationState const& state)
	{
		m_states.push_back(state);
	}

	void AnimationController::RemoveState(int index)
	{
		m_states.erase(m_states.begin() + index);
	}

	AnimationState& AnimationController::operator[](int index)
	{
		return m_states[index];
	}

	AnimationState const& AnimationController::operator[](int index) const
	{
		return m_states[index];
	}

	std::string const& AnimationController::GetName() const
	{
		return m_name;
	}

	void AnimationController::SetName(std::string const& name)
	{
		m_name = name;
	}

	bool AnimationController::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();

		Value nameValue(m_name.c_str(), alloc);
		data.AddMember("Name", nameValue, alloc);

		Value stateArray(rapidjson::kArrayType);
		for (auto& state : m_states)
		{
			Value nameValue(state.GetName().c_str(), alloc);
			stateArray.PushBack(nameValue, alloc);
		}
		data.AddMember("States", stateArray, alloc);

		return true;
	}

	bool AnimationController::Deserialize(const Value& data)
	{
		bool HasName = data.HasMember("Name");
		bool HasStates = data.HasMember("States");

		if (data.IsObject())
		{
			if (HasName)
			{
				const Value& nameValue = data["Name"];
				m_name = nameValue.GetString();
			}
			if (HasStates)
			{
				const Value& statesValue = data["States"].GetArray();
				for (rapidjson::SizeType i{}; i < statesValue.Size(); ++i)
				{
					m_states.push_back(ASSETMANAGER->AnimStates[statesValue[i].GetString()]);
				}
			}
		}

		return false;
	}
}
