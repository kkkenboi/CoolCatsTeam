#include "Platform/Windows/Windows.h"
#include "PauseQuitButton.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Core/Core.h"

namespace LB 
{
	void PauseQuitButton::Start() 
	{

		// Grab All The GOs associated with the Pause Menu
		for (GameObject* GO : GOMANAGER->GetGameObjects())
		{
			// Confirm Menu
			if (GO->GetName() == "PauseMenuQuitConfirmTexture")
			{
				ConfirmMenuTexture = GO;
			}
			if (GO->GetName() == "PauseMenuQuitConfirmYesObject")
			{
				ConfirmMenuYesButton = GO;
			}
			if (GO->GetName() == "PauseMenuQuitConfirmNoObject")
			{
				ConfirmMenuNoButton = GO;
			}

			// Pause Menu
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

		if (GameObj->HasComponent<CPCollider>())
		{
			mCollider = GameObj->GetComponent<CPCollider>();
		}
		else
		{
			mCollider = nullptr;
			return;
		}
	}

	void PauseQuitButton::Update()
	{
		Vec2<float> mouse_pos = INPUT->GetMousePos();
		if (INPUT->IsKeyPressed(KeyCode::KEY_MOUSE_1))
		{
			Vec2<float> current_pos = GameObj->GetComponent<CPTransform>()->GetPosition();

			mouse_pos.y = mouse_pos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();

			mouse_pos.y *= 900.f / (float)WINDOWSSYSTEM->GetHeight();
			mouse_pos.x *= 1600.f / (float)WINDOWSSYSTEM->GetWidth();
			std::vector<CPCollider*> vec_colliders = COLLIDERS->OverlapCircle(mouse_pos, 1.0f);

			// Loop through the colliders found on mouse click
			for (size_t i = 0; i < vec_colliders.size(); ++i)
			{
				// If we found the block
				if (vec_colliders[i] == mCollider)
				{
					PauseMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					PauseMenuResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					PauseMenuHowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					PauseMenuExitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

					ConfirmMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{800.f, 450.f});
					ConfirmMenuYesButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{590.52f, 350.69f});
					ConfirmMenuNoButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{998.f, 347.f});
				}
			}
		}
	}

	void PauseQuitButton::Destroy()
	{

	}

	CPCollider* PauseQuitButton::GetCollider() 
	{
		return mCollider;
	}
}