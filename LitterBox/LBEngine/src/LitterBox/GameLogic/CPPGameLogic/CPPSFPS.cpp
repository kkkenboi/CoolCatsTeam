#include "CPPSFPS.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Engine/Input.h"

namespace LB
{
	void CPPSFPS::Start()
	{
		for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		{
			// Find out which object is the FPS
			if (gameObj->GetName() == "FPS")
			{
				m_FPSObject = gameObj;
			}
		}

		// Based on the FPS prefab, create the object, position is already set in the prefab
		GameObject* fpsObject = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("FPS", *fpsObject);

	}

	void CPPSFPS::Update()
	{
		// Update game paused state
		if (!TIME->IsPaused())
		{
			if (INPUT->IsKeyTriggered(KeyCode::KEY_ESCAPE))
			{
				m_GamePaused = true;
			}

		}

		// If game is not paused, update the text shown based on the actual FPS
		if (!m_GamePaused)
		{
			std::stringstream truncatedFps;
			truncatedFps << std::fixed << std::setprecision(2) << (1.0 / TIME->GetUnscaledDeltaTime());
			fpsBuffer = "FPS: " + truncatedFps.str();
			strcpy_s(finalFps, sizeof(finalFps), fpsBuffer.c_str());

			m_FPSObject->GetComponent<CPText>()->set_msg(finalFps);
		}
	}

	void CPPSFPS::Destroy()
	{
		
	}
}