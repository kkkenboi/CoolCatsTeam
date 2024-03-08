/*!************************************************************************
 \file				AnimationController.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				7-1-2024
 \brief

 This source file contains the definition of the AnimationController class.
 It is a class that holds the logic for the animation state machine.
 It holds the current state, the states of the animation and the controls for the animation.

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "AnimationController.h"
#include "LitterBox/Serialization/AssetManager.h"

namespace LB
{
	/*!***********************************************************************
	 \brief
	 Updates the animation controller each frame
	*************************************************************************/
	void AnimationController::Update()
	{
		m_current.Update();
	}

	/*!***********************************************************************
	 \brief
	 Plays the state of the animation controller
	*************************************************************************/
	void AnimationController::Load(std::string const& stateName)
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

	/*!***********************************************************************
	 \brief
	 Adds a state to the animation controller
	*************************************************************************/
	void AnimationController::AddState(std::string const& state)
	{
		m_states.push_back(state);
	}

	/*!***********************************************************************
	 \brief
	 Removes a state from the animation controller
	*************************************************************************/
	void AnimationController::RemoveState(int index)
	{
		m_states.erase(m_states.begin() + index);
	}

	/*!***********************************************************************
	 \brief
	 Gets the state at the index
	*************************************************************************/
	std::string& AnimationController::operator[](int index)
	{
		return m_states[index];
	}

	/*!***********************************************************************
	 \brief
	 Gets the state at the index
	*************************************************************************/
	std::string const& AnimationController::operator[](int index) const
	{
		return m_states[index];
	}

	/*!***********************************************************************
	 \brief
	 Saves the animation controller
	*************************************************************************/
	bool AnimationController::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();

		Value nameValue(m_name.c_str(), alloc);
		data.AddMember("Name", nameValue, alloc);

		Value stateArray(rapidjson::kArrayType);
		for (auto& state : m_states)
		{
			Value namevalue(state.c_str(), alloc);
			stateArray.PushBack(namevalue, alloc);
		}
		data.AddMember("States", stateArray, alloc);

		return true;
	}

	/*!***********************************************************************
	 \brief
	 Loads the animation controller
	*************************************************************************/
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
