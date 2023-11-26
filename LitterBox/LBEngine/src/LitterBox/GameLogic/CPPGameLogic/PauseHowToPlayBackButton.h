#pragma once

#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class PauseMenuHowToPlayBackButton : public CPPBehaviour
	{
	public:
		void Start() override;
		void Update() override;
		void Destroy() override;

		CPCollider* GetCollider();
	private:
		CPCollider* mCollider;

		// How To Play Objects
		GameObject* HowToPlayTexture{ nullptr };

		// Pause Menu Objects
		GameObject* PauseMenuTexture{ nullptr };
		GameObject* PauseMenuResumeButton{ nullptr };
		GameObject* PauseMenuHowToPlayButton{ nullptr };
		GameObject* PauseMenuExitButton{ nullptr };
	};

}