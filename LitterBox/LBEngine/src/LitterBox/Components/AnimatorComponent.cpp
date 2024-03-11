/*!************************************************************************
 \file				AnimatorComponent.cpp
 \author(s)			Vanessa Chua Siew Jin, Kenji Brannon Chong
 \par DP email(s):	vanessasiewjin@digipen.edu, kenjibrannon.c@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the implementation of the Animator Component (WIP)
 It is used to store the animation controller and play the animation
 based on the name.

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox/Components/AnimatorComponent.h"
#include "LitterBox/Animation/AnimationManager.h"
#include "LitterBox/Serialization/AssetManager.h"

namespace LB
{
	//Getting the size of the spriteSheet

	/*!***********************************************************************
	\brief
	Initialise (initialise all variables)
	*************************************************************************/
	void CPAnimator::Initialise()
	{
		AnimationManager::Instance()->AddAnimator(this);
	}

	/*!************************************************************************
	 \brief
	 Sets up the animator controller
	**************************************************************************/
	void CPAnimator::LoadController()
	{
		if (gameObj->HasComponent<CPRender>()) m_render = gameObj->GetComponent<CPRender>();
		m_transform = gameObj->GetComponent<CPTransform>();
		m_controller = ASSETMANAGER->AnimControllers[m_controller.m_name];

		// If set to play on awake and a default state is set, play the default state
		if (m_playOnAwake)
		{
			m_repeating ? PlayRepeat(m_defaultState) : Play(m_defaultState);
		}
	}

	/*!************************************************************************
	\brief
	 Update function where it update at every frame
	**************************************************************************/
	void CPAnimator::Update()
	{
		// If no state is playing, just return
		if (!m_playing || m_paused) return;

		// If the animation has reached the end and not repeating, stop the animation
		if (m_controller.IsLastFrame())
		{
			if (!m_repeating)
			{
				m_resetAfterPlay ? StopAndReset() : Stop();
			}
			else
			{
				m_controller.GetCurrentState().m_currentFrame = m_controller.GetCurrentState().m_startFrame;
			}
			return;
		}

		// Otherwise, update the animation
		m_elapsedTime += static_cast<float>(TIME->GetDeltaTime() * m_playSpeed);
		if (m_elapsedTime >= m_targetTime)
		{
			m_elapsedTime -= m_targetTime;

			LBAnimationState& currentState = m_controller.GetCurrentState();

			// Update active
			if (!currentState.m_active.Empty())
			{
				bool shouldBeActive = currentState.m_active.GetCurrentExact(currentState.m_currentFrame);
				if (gameObj->IsActive() != shouldBeActive)
				{
					gameObj->SetActive(shouldBeActive);
				}
			}
			// Update pos
			if (!currentState.m_pos.Empty())
			{
				Vec2<float> animPos = currentState.m_pos.GetCurrentExact(currentState.m_currentFrame);
				GetComponent<CPTransform>()->SetPosition(animPos);
			}
			// Update scale
			if (!currentState.m_scale.Empty())
			{
				Vec2<float> animScale = currentState.m_scale.GetCurrentExact(currentState.m_currentFrame);
				GetComponent<CPTransform>()->SetScale(animScale);
			}
			// Update rotation
			if (!currentState.m_rot.Empty())
			{
				float animRot = currentState.m_rot.GetCurrentExact(currentState.m_currentFrame);
				GetComponent<CPTransform>()->SetRotation(animRot);
			}
			// Update image
			if (!currentState.m_sprite.Empty())
			{
				int spriteIndex = currentState.m_sprite.GetCurrentExact(currentState.m_currentFrame);
				m_render->SetSpriteTexture(currentState.m_spriteSheetName, spriteIndex);
			}

			m_controller.Update();
		}
	}

	/*!************************************************************************
	 \brief
	 Plays the animation and stops (and stays) at the last frame
	**************************************************************************/
	void CPAnimator::Play(std::string const& name)
	{
		// TODO : REFACTOR THIS LOGIC, MAY CAUSE BUGS IN THE FUTURE
		if (m_playing)
		{
			m_resetAfterPlay ? StopAndReset() : Stop();
			// If a queue is still present, add to queue instead
			if (m_playing)
			{
				m_queue.push_back(name);
				return;
			}
		}

		m_controller.Load(name);

		// Save old data
		LBAnimationState& currentState = m_controller.GetCurrentState();
		if (!currentState.m_sprite.Empty())
		{
			if (m_render->spriteIndex < 0)
			{
				m_oldID = m_render->texture;
			}
			else
			{
				m_oldSSName = m_render->spriteSheetName;
				m_oldSSIndex = m_render->spriteIndex;
			}
		}
		if (!currentState.m_pos.Empty())
		{
			m_oldPos = m_transform->GetLocalPosition();
		}
		if (!currentState.m_scale.Empty())
		{
			m_oldScale = m_transform->GetLocalScale();
		}
		if (!currentState.m_rot.Empty())
		{
			m_oldRot = m_transform->GetLocalRotation();
		}

		m_playing = true;
	}

	/*!************************************************************************
	 \brief
	 Plays the animation and resets the object to before the animation began
	**************************************************************************/
	void CPAnimator::PlayAndReset(std::string const& name)
	{
		Play(name);
		m_resetAfterPlay = true;
	}

	/*!************************************************************************
	 \brief
	 Plays the animation on repeat forever, until Stop() is called or GO is gone
	**************************************************************************/
	void CPAnimator::PlayRepeat(std::string const& name)
	{
		Play(name);
		m_repeating = true;
	}

	void CPAnimator::PlayNext(std::string const& name)
	{
		if (!m_playing)
		{
			Play(name);
		}
		else
		{
			m_queue.push_back(name);
		}
	}

	/*!************************************************************************
	 \brief
	 Pauses/unpauses the current animation
	**************************************************************************/
	void CPAnimator::Pause(bool state)
	{
		m_paused = state;
	}

	/*!************************************************************************
	 \brief
	 Stops the current animation and leaves the object at its current state
	**************************************************************************/
	void CPAnimator::Stop()
	{
		m_playing = m_repeating = false;
		if (!m_queue.empty())
		{
			Play(m_queue.front());
			m_queue.erase(m_queue.begin());
		}
	}

	/*!************************************************************************
	 \brief
	 Stops the current animation and resets it to before the animation began
	**************************************************************************/
	void CPAnimator::StopAndReset()
	{
		m_resetAfterPlay = false;

		LBAnimationState& currentState = m_controller.GetCurrentState();
		// Reset the texture to the original before the animation began
		if (!currentState.m_sprite.Empty())
		{
			if (m_oldSSName != "None")
			{
				m_render->SetSpriteTexture(m_controller.GetCurrentSpriteSheet(), m_oldSSIndex);
			}
			else
			{
				m_render->spriteSheetName = "None";
				m_render->texture = m_oldID;
				m_render->UpdateTexture(m_oldID, static_cast<int>(m_render->w), static_cast<int>(m_render->h));
			}
		}
		if (!currentState.m_pos.Empty())
		{
			m_transform->SetPosition(m_oldPos);
		}
		if (!currentState.m_scale.Empty())
		{
			m_transform->SetScale(m_oldScale);
		}
		if (!currentState.m_rot.Empty())
		{
			m_transform->SetRotation(m_oldRot);
		}
		Stop();
	}

	/*!************************************************************************
	\brief
	 Destroy function
	**************************************************************************/
	void CPAnimator::Destroy()
	{
		AnimationManager::Instance()->RemoveAnimator(this);
	}

	/*!************************************************************************
	 \brief
	 Saves the animator component
	**************************************************************************/
	bool CPAnimator::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();

		Value defaultStateValue(m_defaultState.c_str(), alloc);
		data.AddMember("Default State", defaultStateValue, alloc);

		data.AddMember("Play On Awake", m_playOnAwake, alloc);
		data.AddMember("Repeating", m_repeating, alloc);

		Value controllerValue(m_controller.m_name.c_str(), alloc);
		data.AddMember("Controller", controllerValue, alloc);

		return true;
	}

	/*!************************************************************************
	 \brief
	 Loads the animator component
	**************************************************************************/
	bool CPAnimator::Deserialize(const Value& data)
	{
		bool HasDefaultState = data.HasMember("Default State");
		bool HasPlayOnAwake = data.HasMember("Play On Awake");
		bool HasRepeating = data.HasMember("Repeating");
		bool HasController = data.HasMember("Controller");

		if (data.IsObject())
		{
			if (HasDefaultState)
			{
				const Value& defaultStateValue = data["Default State"];
				m_defaultState = defaultStateValue.GetString();
			}
			if (HasPlayOnAwake)
			{
				m_playOnAwake = data["Play On Awake"].GetBool();
			}
			if (HasRepeating)
			{
				m_repeating = data["Repeating"].GetBool();
			}
			if (HasController)
			{
				const Value& nameValue = data["Controller"];
				SetControllerName(nameValue.GetString());
			}
			return true;
		}

		return false;
	}
}