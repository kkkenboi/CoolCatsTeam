#include "LitterBox/Components/AnimatorComponent.h"

namespace LB
{
	//Getting the size of the spriteSheet

	void CPAnimator::Initialise()
	{
		//frames.resize(animCount, std::vector<LB::Vec2<float>>(frameCount));
	}

	void CPAnimator::Update()
	{
		//frames.resize(animCount, std::vector<LB::Vec2<float>>(frameCount));
	}

	void CPAnimator::Destroy()
	{

	}

	void CPAnimator::SizeOfImage(std::string spriteSheet) //getting the size of the SpriteSheet
	{
		spriteWidth = LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap[spriteSheet])->second.first->width;
		spriteHeight = LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap[spriteSheet])->second.first->height;
	}

	int CPAnimator::NumOfAnim() const //Row of the spritesheet
	{
		return animCount;
	}

	int CPAnimator::NumOfFrames() const //Columns of the spritesheet
	{
		return frameCount;
	}


	//std::array<std::array<LB::Vec2<float>, animCount>, frameCount> frames[animCount];
	//for now I just allow user to set their own frame and string, in the future I might want to create where user will just need to add the
	//animation name and the frame will just increment when then add by itself, so they do not need to remember it ya know
	//so right maybe I will need to have to create a counter where it incrememnt the frames as the user creates, and if he deletes, he delete and
	//I decrement the counter
	void CPAnimator::SetAnimation(const std::string animationName, int animCount, float timer) //I want it to allow the user to set the name of the animation
	{
		frames.resize(animCount, std::vector<LB::Vec2<float>>(frameCount));
		std::vector<LB::Vec2<float>*> pointers(frames.size());

		for (size_t i = 0; i < frames.size(); ++i) 
		{
			pointers[i] = frames[i].data();  // Get the pointer to the data of each inner vector
		}

		Renderer::GRAPHICS->init_anim(animationName, pointers.data(), timer, frameCount);
	}

	void CPAnimator::StartAnimation(const std::string& animationName)
	{
		float x_inc{ (float)spriteWidth / (animCount * (float)spriteWidth) };
		float y_inc{ (float)spriteHeight / (frameCount * (float)spriteHeight) };

		for (int y{ 0 }; y < animCount; ++y)
			for (int x{ 0 }; x < frameCount; ++x) 
			{
				frames[y][x] = { x * x_inc, y * y_inc };//bottom left
				frames[y][x] = { (x + 1) * x_inc, y * y_inc };//bottom right
				frames[y][x] = { (x + 1) * x_inc, (y + 1) * y_inc };//top right
				frames[y][x] = { x * x_inc, (y + 1) * y_inc };//top left
			}
	}



}