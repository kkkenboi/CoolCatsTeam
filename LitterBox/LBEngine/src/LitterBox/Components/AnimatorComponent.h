/*!************************************************************************
 \file				AnimatorComponent.h
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
#pragma once
#include "LitterBox/Components/Component.h"
#include "LitterBox/Serialization/AssetManager.h"
//#include <array>

namespace LB
{
	/*!************************************************************************
	 * Animator Component that inherits from IComponent
	 *
	**************************************************************************/
	class CPAnimator : public IComponent
	{
	public:
		//std::array<std::array<LB::Vec2<float>, animCount>, frameCount> frames[animCount];
		static ComponentTypeID GetType()
		{
			return C_CPAnimator;
		}
		
		//load files in initialise
		void Initialise() override;

		void Update() override;

		void Destroy() override;

		void SizeOfImage(std::string spriteSheet); //getting the size of width and height

		int NumOfAnim() const; //Number of animation

		int NumOfFrames() const; //Number of frames

		////serialisation, for images
		//bool Serialize(Value& data, Document::AllocatorType& alloc) override;

		////deserialisation, for images
		//bool Deserialize(const Value& data) override;

		//to set the animation
		void SetAnimation(const std::string animationName, int animCount, float timer);

		//start the animation
		void StartAnimation(const std::string& animationName);

		//start indicates the animation playing
		//void StartAnimation(const std::string& animationName);
		//stop indicates the animation stopping
		void StopAnimation(const std::string& animationName);


	private:
		unsigned int animCount, frameCount; //rows, cols
		unsigned int spriteWidth, spriteHeight; //size of sprite
		std::string animName; //animations name
		//std::array<std::array<LB::Vec2<float>, 4>, 12>
		std::vector<std::vector<LB::Vec2<float>>> frames;
		AssetManager* assetManager;

	};
}