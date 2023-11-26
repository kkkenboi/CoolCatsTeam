#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "PauseHowToPlayButton.h"

namespace LB
{
	void PauseMenuHowToPlayButton::Start()
	{
		std::vector<GameObject*> GOs = GOMANAGER->GetGameObjects();
		for (GameObject* GO : GOs) {
			if (GO->GetName() == "PauseMenuHowToPlayTextureObject") 
			{
				HowToPlayTexture = GO;
			}
			if (GO->GetName() == "PauseMenuHowToPlayPreviousMenuButton")
			{
				HowToPlayBackButton = GO;
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

	void PauseMenuHowToPlayButton::Update()
	{
		Vec2<float> mouse_pos = INPUT->GetMousePos();
		if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
		{
			Vec2<float> current_pos = GameObj->GetComponent<CPTransform>()->GetPosition();


			mouse_pos.y = mouse_pos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();

			mouse_pos.y *= 1080.f / (float)WINDOWSSYSTEM->GetHeight();
			mouse_pos.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();

			std::vector<CPCollider*> vec_colliders = COLLIDERS->OverlapCircle(mouse_pos, 1.0f);

			// Loop through the colliders found on mouse click
			for (size_t i = 0; i < vec_colliders.size(); ++i)
			{
				if (vec_colliders[i] == mCollider)
				{
					// If we found the block
					// Move all the Pause Menu Objects away
					PauseMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					PauseMenuResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					PauseMenuHowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					PauseMenuExitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

					// Move all the How To Play Objects into scene
					HowToPlayTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{800.66f, 449.39f});
					HowToPlayTexture->GetComponent<CPTransform>()->SetScale(Vec2<float>{16.10f, 9.18f});

					HowToPlayBackButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{161.75f, 72.12f});
					HowToPlayBackButton->GetComponent<CPCollider>()->SetWidthHeightRadius(218.f, 165.f, 50.f);
				}
			}
		}
	}

	void PauseMenuHowToPlayButton::Destroy()
	{

	}
}