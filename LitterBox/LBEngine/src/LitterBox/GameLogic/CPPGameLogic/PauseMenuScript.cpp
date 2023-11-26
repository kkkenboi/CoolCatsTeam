#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Engine/Time.h"
#include "PauseMenuScript.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Serialization/AssetManager.h"

namespace LB
{
	void PauseMenuScript::Start()
	{
		//mIsPaused = TIME->IsPaused();
		//TIME->Pause(false);

		// Grab All The GOs associated with the Pause Menu
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

	void PauseMenuScript::Update()
	{
		//DEBUG->DrawCircle(INPUT->GetMousePos(), 1.f, Vec4<float>(0.f, 0.f, 1.0f, 1.0f));
		if (!TIME->IsPaused())
		{
			if (INPUT->IsKeyTriggered(KeyCode::KEY_ESCAPE))
			{
				
				TIME->Pause(true);
				// Move all the GOs to the Front of the Screen

				MenuTextureObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.f, 529.f});
				MenuTextureObject->GetComponent<CPTransform>()->SetScale(Vec2<float>{19.67f, 11.50f});

				ResumeButtonObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.66f, 585.23f});
				ResumeButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(334.f, 78.f, 50.f);

				HowToPlayButtonObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 487.13f});
				HowToPlayButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(332.f, 74.f, 50.f);

				QuitButtonObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.22f, 310.78f});
				QuitButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(317.f, 77.f, 50.f);

			}
		}

		else
		{
			if (INPUT->IsKeyTriggered(KeyCode::KEY_ESCAPE))
			{
				TIME->Pause(false);
				
				MenuTextureObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
				//MenuTextureObject->GetComponent<CPTransform>()->SetScale(Vec2<float>{1.f, 1.f});

				ResumeButtonObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
				//ResumeButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);
				
				HowToPlayButtonObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
				//HowToPlayButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);
				
				QuitButtonObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
				//QuitButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

			}
		}
	}

	void PauseMenuScript::Destroy()
	{

	}

}