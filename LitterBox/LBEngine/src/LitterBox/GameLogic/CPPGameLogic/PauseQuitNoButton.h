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

		GameObject* ConfirmMenuTexture{ nullptr };
		GameObject* ConfirmMenuYesButton{ nullptr };
		GameObject* ConfirmMenuNoButton{ nullptr };

		GameObject* PauseMenuTexture{ nullptr };
		GameObject* PauseMenuResumeButton{ nullptr };
		GameObject* PauseMenuHowToPlayButton{ nullptr };
		GameObject* PauseMenuExitButton{ nullptr };
	};

}