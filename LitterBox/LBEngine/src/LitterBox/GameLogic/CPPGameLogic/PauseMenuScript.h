#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class PauseMenuScript : public CPPBehaviour
	{
	public:
		void Start() override;
		void Update() override;
		void Destroy() override;
	private:
		GameObject* MenuTextureObject;
		GameObject* ResumeButtonObject;
		GameObject* HowToPlayButtonObject;
		GameObject* QuitButtonObject;
	};

}