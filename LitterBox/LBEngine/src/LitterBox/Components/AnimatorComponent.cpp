/*!************************************************************************
 \file				AnimatorComponent.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the implementation of the Animator Component (WIP)

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox/Components/AnimatorComponent.h"

namespace LB
{
	//Getting the size of the spriteSheet

	/*!***********************************************************************
	\brief
	Initialise (initialise all variables)
	*************************************************************************/
	void CPAnimator::Initialise()
	{
		//empty for now
	}

	/*!***********************************************************************
	\brief
	 Update
	*************************************************************************/
	void CPAnimator::Update()
	{
		//empty for now
	}

	/*!***********************************************************************
	\brief
	 Destroy
	*************************************************************************/
	void CPAnimator::Destroy()
	{
		//empty for now
	}

	/*!***********************************************************************
	\brief
	 Getting the size of the sprite sheet
	*************************************************************************/
	void CPAnimator::SizeOfImage(std::string spriteSheet) //getting the size of the SpriteSheet
	{
		spriteWidth = LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap[spriteSheet])->second.first->width;
		spriteHeight = LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap[spriteSheet])->second.first->height;
	}

	/*!***********************************************************************
	\brief
	 Getter function for num Of animations
	*************************************************************************/
	int CPAnimator::NumOfAnim() const //Row of the spritesheet
	{
		return animCount;
	}

	/*!***********************************************************************
	\brief
	 Getter function for num of Frames
	*************************************************************************/
	int CPAnimator::NumOfFrames() const //Columns of the spritesheet
	{
		return frameCount;
	}

	/*!***********************************************************************
	\brief
	 Thihs function will stop the animation
	*************************************************************************/
	void CPAnimator::StopAnimation(const std::string& animationName)
	{
		(void)animationName;
	}

	/*!***********************************************************************
	\brief
	 This function sets the animation
	*************************************************************************/
	void CPAnimator::SetAnimation(const std::string animationName, int count, float timer) //I want it to allow the user to set the name of the animation
	{
		(void)timer;
		frames.resize(count, std::vector<LB::Vec2<float>>(frameCount));
		std::vector<LB::Vec2<float>*> pointers(frames.size());

		for (size_t i = 0; i < frames.size(); ++i) 
		{
			pointers[i] = frames[i].data();  // Get the pointer to the data of each inner vector
		}

		//Renderer::GRAPHICS->init_anim(animationName, pointers.data(), timer, frameCount);
	}

	/*!***********************************************************************
	\brief
	 Starting of the animation
	*************************************************************************/
	void CPAnimator::StartAnimation(const std::string& animationName)
	{
		(void)animationName;
		float x_inc{ (float)spriteWidth / (animCount * (float)spriteWidth) };
		float y_inc{ (float)spriteHeight / (frameCount * (float)spriteHeight) };

		for (unsigned int y{ 0 }; y < animCount; ++y)
			for (unsigned int x{ 0 }; x < frameCount; ++x)
			{
				frames[y][x] = { x * x_inc, y * y_inc };//bottom left
				frames[y][x] = { (x + 1) * x_inc, y * y_inc };//bottom right
				frames[y][x] = { (x + 1) * x_inc, (y + 1) * y_inc };//top right
				frames[y][x] = { x * x_inc, (y + 1) * y_inc };//top left
			}
	}

	//serialisation, for images
	//bool CPAnimator::Serialize(Value& data, Document::AllocatorType& alloc)
	//{
	//	return false;
	//}

	////deserialisation, for images
	//bool CPAnimator::Deserialize(const Value& data)
	//{
	//	return false;
	//}
}