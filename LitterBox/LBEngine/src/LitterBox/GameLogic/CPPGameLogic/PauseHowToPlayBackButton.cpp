#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Scene/SceneManager.h"
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

			mouse_pos.y = mouse_pos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();

			mouse_pos.y *= 1080.f / (float)WINDOWSSYSTEM->GetHeight();
			mouse_pos.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();
			std::vector<CPCollider*> vec_colliders = COLLIDERS->OverlapCircle(mouse_pos, 1.0f);

			// Loop through the colliders found on mouse click
			for (size_t i = 0; i < vec_colliders.size(); ++i)
			{
				if (vec_colliders[i] == mCollider)
				{
					if (PauseMenuTexture) {
						// If we found the block
						// Move all the Pause Menu Objects away
						// ==
						PauseMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.f, 529.f});
						PauseMenuTexture->GetComponent<CPTransform>()->SetScale(Vec2<float>{19.67f, 11.50f});

						PauseMenuResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.66f, 585.23f});
						PauseMenuResumeButton->GetComponent<CPCollider>()->SetWidthHeightRadius(334.f, 78.f, 50.f);

						PauseMenuHowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 487.13f});
						PauseMenuHowToPlayButton->GetComponent<CPCollider>()->SetWidthHeightRadius(332.f, 74.f, 50.f);

						PauseMenuExitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.22f, 310.78f});
						PauseMenuExitButton->GetComponent<CPCollider>()->SetWidthHeightRadius(317.f, 77.f, 50.f);

						// Move all the How To Play Objects into scene
						HowToPlayTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

						this->GameObj->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					}
					else {
						SCENEMANAGER->LoadScene("SceneMainMenu");
					}
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