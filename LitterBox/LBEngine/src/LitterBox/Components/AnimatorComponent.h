#pragma once
#include "LitterBox/Components/Component.h"
#include "LitterBox/Serialization/AssetManager.h"
//#include <array>

namespace LB
{

	class CPAnimator : public IComponent
	{
	public:
		//std::array<std::array<LB::Vec2<float>, animCount>, frameCount> frames[animCount];
		static ComponentTypeID GetType()
		{
			return C_CPAnimator;
		}

		//struct AnimationData
		//{
		//	unsigned int animCount, frameCount;
		//	unsigned int spriteWidth, spriteHeight;
		//	AnimationData(AssetManager* assetManager); // Getting texture from Asset Manager

		//};
		
		//load files in initialise

		void Initialise() override;

		void Update() override;

		void Destroy() override;

		void SizeOfImage(std::string spriteSheet); //getting the size of width and height

		int NumOfAnim() const;

		int NumOfFrames() const;

		void SetAnimation(const std::string animationName, int animCount, float timer);

		void StartAnimation(const std::string& animationName);
		

		bool Serialize(Value& data, Document::AllocatorType& alloc) override;

		bool Deserialize(const Value& data) override;


		//start indicates the animation playing
		//void StartAnimation(const std::string& animationName);
		//stop indicates the animation stopping
		void StopAnimation(const std::string& animationName);


	private:
		//AnimationData spriteWidth, spriteHeight;
		//AnimationData animCount, frameCount; //row, col, need slice 
		unsigned int animCount, frameCount;
		unsigned int spriteWidth, spriteHeight;
		std::string animName;
		//std::array<std::array<LB::Vec2<float>, 4>, 12>
		std::vector<std::vector<LB::Vec2<float>>> frames;
		AssetManager* assetManager;

	};
}