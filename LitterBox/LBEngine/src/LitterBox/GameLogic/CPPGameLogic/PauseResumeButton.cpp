#include "Platform/Windows/Windows.h"
#include "PauseResumeButton.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Physics/ColliderManager.h"

namespace LB
{
	void PauseResumeButton::Start()
	{
		for (GameObject* GO : GOMANAGER->GetGameObjects())
		{
			if (GO->GetName() == "PauseMenuTextureObject")
			{
				MenuTextureObject = GO;
			}
			if (GO->GetName() == "PauseMenuResumeObject")
			{
				ResumeButtonObject = GO;
			}
			if (GO->GetName() == "PauseMenuHowToPlayObject")
			{
				HowToPlayButtonObject = GO;
			}
			if (GO->GetName() == "PauseMenuQuitObject")
			{
				QuitButtonObject = GO;
			}
		}
	}

	void PauseResumeButton::Update()
	{
		if (TIME->IsPaused())
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
					if (vec_colliders[i] == GameObj->GetComponent<CPCollider>())
					{
						TIME->Pause(false);

						MenuTextureObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						MenuTextureObject->GetComponent<CPTransform>()->SetScale(Vec2<float>{1.f, 1.f});

						ResumeButtonObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						ResumeButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

						HowToPlayButtonObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						HowToPlayButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

						QuitButtonObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						QuitButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

					}
				}
			}
		}
	}

	void PauseResumeButton::Destroy()
	{

	}
}