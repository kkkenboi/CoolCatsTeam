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
		GameObject* HowToPlayTexture;

		// Pause Menu Objects
		GameObject* PauseMenuTexture;
		GameObject* PauseMenuResumeButton;
		GameObject* PauseMenuHowToPlayButton;
		GameObject* PauseMenuExitButton;
	};

}