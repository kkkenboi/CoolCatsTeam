#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "PauseHowToPlayBackButton.h"

namespace LB
{
	void PauseMenuHowToPlayBackButton::Start()
	{
		std::vector<GameObject*> GOs = GOMANAGER->GetGameObjects();
		for (GameObject* GO : GOs) {
			if (GO->GetName() == "PauseMenuHowToPlayTextureObject")
			{
				HowToPlayTexture = GO;
			}
			if (GO->GetName() == "PauseMenuTextureObject")
			{
				PauseMenuTexture = GO;
			}
			if (GO->GetName() == "PauseMenuResumeObject")
			{
				PauseMenuResumeButton = GO;
			}
			if (GO->GetName() == "PauseMenuHowToPlayObject")
			{
				PauseMenuHowToPlayButton = GO;
			}
			if (GO->GetName() == "PauseMenuQuitObject")
			{
				PauseMenuExitButton = GO;
			}
		}

		mCollider = this->GameObj->GetComponent<CPCollider>();
	}

	void PauseMenuHowToPlayBackButton::Update()
	{
		Vec2<float> mouse_pos = INPUT->GetMousePos();
		if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
		{
			Vec2<float> current_pos = GameObj->GetComponent<CPTransform>()->GetPosition();

			std::vector<CPCollider*> vec_colliders = COLLIDERS->OverlapCircle(mouse_pos, 1.0f);

			// Loop through the colliders found on mouse click
			for (size_t i = 0; i < vec_colliders.size(); ++i)
			{
				if (vec_colliders[i] == mCollider)
				{
					// If we found the block
					// Move all the Pause Menu Objects away
					PauseMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{800.f, 450.f});
					PauseMenuTexture->GetComponent<CPTransform>()->SetScale(Vec2<float>{16.37f, 10.80f});

					PauseMenuResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{807.f, 507.1f});
					PauseMenuResumeButton->GetComponent<CPCollider>()->SetWidthHeightRadius(270.f, 78.f, 50.f);

					PauseMenuHowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{805.f, 412.f});
					PauseMenuHowToPlayButton->GetComponent<CPCollider>()->SetWidthHeightRadius(270.f, 74.f, 50.f);

					PauseMenuExitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{804.f, 238.f});
					PauseMenuExitButton->GetComponent<CPCollider>()->SetWidthHeightRadius(266.f, 77.f, 50.f);

					// Move all the How To Play Objects into scene
					HowToPlayTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

					this->GameObj->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
				}
			}
		}
	}

	void PauseMenuHowToPlayBackButton::Destroy()
	{

	}

	CPCollider* PauseMenuHowToPlayBackButton::GetCollider() 
	{
		return mCollider;
	}
}