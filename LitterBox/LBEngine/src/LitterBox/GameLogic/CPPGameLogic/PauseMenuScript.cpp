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
	}

	void PauseMenuScript::Update()
	{
		DEBUG->DrawCircle(INPUT->GetMousePos(), 1.f, Vec4<float>(0.f, 0.f, 1.0f, 1.0f));
		if (!TIME->IsPaused())
		{
			if (INPUT->IsKeyTriggered(KeyCode::KEY_ESCAPE))
			{
				TIME->Pause(true);
				/*
				// Spawn the prefab
				// ===================================================================
				// Make GameObject that has Texture of Pause Menu
				MenuTextureObject = FACTORY->SpawnGameObject(std::initializer_list{ C_CPRender },
					GOSpawnType::SCENE_BOUNDED);

				


				MenuTextureObject->SetName("MenuTextureObject");

				// ===================================================================
				// Make GameObject that has Collider to allow for Resume Game
				ResumeButtonObject = FACTORY->SpawnGameObject(std::initializer_list{ C_CPCollider, C_CPScriptCPP },
					GOSpawnType::SCENE_BOUNDED);


				ResumeButtonObject->SetName("ResumeButtonObject");
				
				// ===================================================================
				// Make GameObject that has Collider to allow for How To Play
				HowToPlayButtonObject = FACTORY->SpawnGameObject(std::initializer_list{ C_CPCollider, C_CPScriptCPP },
					GOSpawnType::SCENE_BOUNDED);

				HowToPlayButtonObject->SetName("HowToPlayButtonObject");
				*/
				// ===================================================================
				// Make GameObject that has Collider to allow for Quit Game
				QuitButtonObject = FACTORY->SpawnGameObject();
				//JSONSerializer::DeserializeFromFile("PauseQuitGameObject", *QuitButtonObject);
			}
		}

		if (TIME->IsPaused())
		{
			if (INPUT->IsKeyTriggered(KeyCode::KEY_ESCAPE))
			{
				TIME->Pause(false);

				MenuTextureObject->Destroy();
				ResumeButtonObject->Destroy();
				HowToPlayButtonObject->Destroy();
				QuitButtonObject->Destroy();

			}
		}
	}

	void PauseMenuScript::Destroy()
	{

	}

}