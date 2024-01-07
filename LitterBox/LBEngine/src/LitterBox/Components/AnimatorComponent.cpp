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
		numOfSidesOnFrame = 4; //4 corners of the UV frames
		anim_frames = 
			std::vector
			<std::vector<LB::Vec2<float>>>(
				numOfSidesOnFrame, std::vector<LB::Vec2<float>>(NumOfFramesWithFourPoints())); //x,y
		//numOfFrames, std::vector<LB::Vec2<float>>(numOfSidesOnFrame));
	}

	/*!***********************************************************************
	\brief
	 Update
	*************************************************************************/
	void CPAnimator::Update()
	{
		//empty for now
		anim_frames.resize(numOfFrames); //resizing the number of frames due to what is set?


	}

	/*!***********************************************************************
	\brief
	 Destroy
	*************************************************************************/
	void CPAnimator::Destroy()
	{
		//empty for now
	}

	bool Serialize(Value& data, Document::AllocatorType& alloc)
	{
		//apparently i cant right now 7/1/2024
		return false;
	}

	bool Deserialize(const Value& data)
	{
		//apparently i cant right now 7/1/2024
		return false;
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
	 Getter function for num Of animations
	*************************************************************************/
	int CPAnimator::NumOfAnim() //Row of the spritesheet
	{
		return numOfAnim;
	}

	/*!***********************************************************************
	\brief
	 Getter function for num of Frames
	*************************************************************************/
	int CPAnimator::NumOfFramesWithFourPoints() //Columns of the spritesheet
	{
		return numOfAnim * numOfAnim; //e.g. 31
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

	/*!***********************************************************************
	\brief
	 Starting of the animation
	*************************************************************************/
	void CPAnimator::SetAnimation(const std::string& animationName, float speedOfAnim, int numOfFrame)
	{
		//(void)animationName;
		if (ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap[animationName]) != LB::ASSETMANAGER->Textures.end()) {

			float x_inc{ 1.f / 4.f };

			for (int y{ 0 }; y < 4; ++y)
			{
				for (int x{ 0 }; x < 4; ++x) 
				{
					anim_frames[x + y * 4].at(0) = { x * x_inc, 1.f - (y + 1) * x_inc };//bottom left
					anim_frames[x + y * 4].at(1) = { (x + 1) * x_inc, 1.f - (y + 1) * x_inc };//bottom right
					anim_frames[x + y * 4].at(2) = { (x + 1) * x_inc, 1.f - y * x_inc };//top right
					anim_frames[x + y * 4].at(3) = { x * x_inc, 1.f - y * x_inc };//top left
				}

			}

			//Renderer::GRAPHICS->init_anim(animationName, anim_frames.data(), speedOfAnim, numOfFrame);
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