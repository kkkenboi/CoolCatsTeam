#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class PauseQuitNoButton : public CPPBehaviour
	{
	public:
		void Start() override;
		void Update() override;
		void Destroy() override;

		CPCollider* GetCollider();
	private:
		CPCollider* mCollider;

		GameObject* ConfirmMenuTexture;
		GameObject* ConfirmMenuYesButton;
		GameObject* ConfirmMenuNoButton;

		GameObject* PauseMenuTexture;
		GameObject* PauseMenuResumeButton;
		GameObject* PauseMenuHowToPlayButton;
		GameObject* PauseMenuExitButton;
	};

}