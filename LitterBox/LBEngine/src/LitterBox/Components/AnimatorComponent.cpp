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

	/*!***********************************************************************
	\brief
	 Update
	*************************************************************************/
	void CPAnimator::Update()
	{
		m_controller.Update();
	}

	void CPAnimator::Play()
	{
		m_controller.Play();
	}

	void CPAnimator::Stop()
	{
		m_controller.Stop();
	}

	/*!***********************************************************************
	\brief
	 Destroy
	*************************************************************************/
	void CPAnimator::Destroy()
	{
		//empty for now
	}

	bool CPAnimator::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		//apparently i cant right now 7/1/2024
		return false;
	}

	bool CPAnimator::Deserialize(const Value& data)
	{
		//apparently i cant right now 7/1/2024
		return false;
	}

	std::string const& CPAnimator::GetControllerName()
	{
		return m_name;
	}

	void CPAnimator::SetControllerName(std::string const& name)
	{
		m_name = name;
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