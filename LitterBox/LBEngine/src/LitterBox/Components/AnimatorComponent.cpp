/*!************************************************************************
 \file				AnimatorComponent.cpp
 \author(s)			Vanessa Chua Siew Jin, Kenji Brannon Chong
 \par DP email(s):	vanessasiewjin@digipen.edu, kenjibrannon.c@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the implementation of the Animator Component (WIP)

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

	void CPAnimator::LoadController()
	{
		m_render = gameObj->GetComponent<CPRender>();
		m_controller = ASSETMANAGER->AnimControllers[m_controller.GetName()];
	}

	/*!***********************************************************************
	\brief
	 Update
	*************************************************************************/
	void CPAnimator::Update()
	{
		if (!m_controller.IsPlaying())
		{
			if (m_repeating)
			{
				m_controller.Play();
			}
			else
			{
				if (m_playing)
				{
					// Reset the texture to the original before the animation began
					if (m_oldSSName != "None")
					{
						m_render->SetSpriteTexture(m_controller.GetCurrentSpriteSheet(), m_controller.IsNextFrame());
					}
					else
					{
						m_render->spriteSheetName = "None";
						m_render->texture = m_oldSSIndex;
						m_render->UpdateTexture(m_oldID, m_render->w, m_render->h);
					}
					m_playing = false;
				}
				return;
			}
		}

		m_controller.Update();
		if (m_controller.IsNextFrame())
		{
			m_render->SetSpriteTexture(m_controller.GetCurrentSpriteSheet(), m_controller.IsNextFrame());
		}
	}

	void CPAnimator::Play(std::string const& name)
	{
		// Save old data
		if (m_render->spriteSheetName == "None")
		{
			m_oldID = m_render->texture;
		}
		else
		{
			m_oldSSName = m_render->spriteSheetName;
			m_oldSSIndex = m_render->spriteIndex;
		}
		m_playing = true;
		m_controller.Play(name);
	}

	void CPAnimator::PlayRepeat(std::string const& name)
	{
		m_repeating = true;
		Play(name);
	}

	void CPAnimator::Stop()
	{
		m_playing = m_repeating = false;
		m_controller.Stop();

		// Reset the texture to the original before the animation began
		if (m_oldSSName != "None")
		{
			m_render->SetSpriteTexture(m_controller.GetCurrentSpriteSheet(), m_controller.IsNextFrame());
		}
		else
		{
			m_render->spriteSheetName = "None";
			m_render->texture = m_oldSSIndex;
			m_render->UpdateTexture(m_oldID, m_render->w, m_render->h);
		}
	}

	/*!***********************************************************************
	\brief
	 Destroy
	*************************************************************************/
	void CPAnimator::Destroy()
	{
		AnimationManager::Instance()->RemoveAnimator(this);
	}

	bool CPAnimator::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();

		Value controllerValue(m_controller.GetName().c_str(), alloc);
		data.AddMember("Controller", controllerValue, alloc);

		return true;
	}

	bool CPAnimator::Deserialize(const Value& data)
	{
		bool HasController = data.HasMember("Controller");

		if (data.IsObject())
		{
			if (HasController)
			{
				const Value& nameValue = data["Controller"];
				SetControllerName(nameValue.GetString());
			}
			return true;
		}

		return false;
	}

	std::string const& CPAnimator::GetControllerName()
	{
		return m_controller.GetName();
	}

	void CPAnimator::SetControllerName(std::string const& name)
	{
		m_controller.SetName(name);
	}

	///*!***********************************************************************
	//\brief
	// Getting the size of the sprite sheet
	//*************************************************************************/
	//void CPAnimator::SizeOfImage(std::string spriteSheet) //getting the size of the SpriteSheet
	//{
	//	spriteWidth = LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap[spriteSheet])->second.first->width;
	//	spriteHeight = LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap[spriteSheet])->second.first->height;
	//}

	/*!***********************************************************************
	\brief
	 This function sets the animation
	*************************************************************************/
	//void CPAnimator::SetAnimation(const std::string animationName, int count, float timer) //I want it to allow the user to set the name of the animation
	//{
	//	(void)timer;
	//	//frames.resize(count, std::vector<LB::Vec2<float>>(frameCount));
	//	std::vector<LB::Vec2<float>*> pointers(frames.size());

	//	for (size_t i = 0; i < frames.size(); ++i) 
	//	{
	//		pointers[i] = frames[i].data();  // Get the pointer to the data of each inner vector
	//	}

	//	//Renderer::GRAPHICS->init_anim(animationName, pointers.data(), timer, frameCount);
	//}

	///*!***********************************************************************
	//\brief
	// Starting of the animation
	//*************************************************************************/
	//void CPAnimator::SetAnimation(const std::string& animationName, float speedOfAnim, int numOfFrame)
	//{
	//	//(void)animationName;
	//	if (ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap[animationName]) != LB::ASSETMANAGER->Textures.end()) {

	//		float x_inc{ 1.f / 4.f };

	//		for (int y{ 0 }; y < 4; ++y)
	//		{
	//			for (int x{ 0 }; x < 4; ++x) 
	//			{
	//				anim_frames[x + y * 4].at(0) = { x * x_inc, 1.f - (y + 1) * x_inc };//bottom left
	//				anim_frames[x + y * 4].at(1) = { (x + 1) * x_inc, 1.f - (y + 1) * x_inc };//bottom right
	//				anim_frames[x + y * 4].at(2) = { (x + 1) * x_inc, 1.f - y * x_inc };//top right
	//				anim_frames[x + y * 4].at(3) = { x * x_inc, 1.f - y * x_inc };//top left
	//			}

	//		}

	//		//Renderer::GRAPHICS->init_anim(animationName, anim_frames.data(), speedOfAnim, numOfFrame);
	//	}
	//}
}