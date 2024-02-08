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
		m_current.Update();
	}

	void AnimationController::SetState(std::string const& stateName)
	{
		for (auto& state : m_states)
		{
			if (state == stateName)
			{
				m_current = ASSETMANAGER->AnimStates[state];
				return;
			}
		}
		DebuggerLogErrorFormat("Animator Controller : %s does not have a state named: %s!", m_name.c_str(), stateName.c_str());
	}

	void AnimationController::Play(std::string const& stateName)
	{
		if (m_current.GetName() != stateName)
		{
			SetState(stateName);
		}
		Play();
	}

	void AnimationController::Play()
	{
		m_current.Start();
	}

	void AnimationController::Stop()
	{
		m_current.Stop();
	}

	std::string const& AnimationController::GetCurrentSpriteSheet()
	{
		return m_current.GetSpriteSheetName();
	}

	bool AnimationController::IsPlaying() const
	{
		return m_current.IsPlaying();
	}

	int AnimationController::IsNextFrame() const
	{
		if (m_current.IsNextFrame())
		{
			return m_current.GetCurrentFrame();
		}
		return 0;
	}

	int AnimationController::GetStateCount() const
	{
		return m_states.size();
	}

	std::vector<std::string>& AnimationController::GetStates()
	{
		return m_states;
	}

	void AnimationController::AddState(std::string const& state)
	{
		m_states.push_back(state);
	}

	void AnimationController::RemoveState(int index)
	{
		m_states.erase(m_states.begin() + index);
	}

	std::string& AnimationController::operator[](int index)
	{
		return m_states[index];
	}

	std::string const& AnimationController::operator[](int index) const
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
			Value nameValue(state.c_str(), alloc);
			stateArray.PushBack(nameValue, alloc);
		}
		data.AddMember("States", stateArray, alloc);

		return true;
	}

	bool AnimationController::Deserialize(const Value& data)
	{
		m_states.clear();

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
					m_states.push_back(statesValue[i].GetString());
				}
			}
		}

		return false;
	}
}
