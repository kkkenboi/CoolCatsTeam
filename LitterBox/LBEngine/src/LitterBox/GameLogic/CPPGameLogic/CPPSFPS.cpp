#include "CPPSFPS.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Renderer/Renderer.h"

namespace LB
{
	void CPPSFPS::Start()
	{
		//for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		//{
		//	// Find out which object is the FPS
		//	if (gameObj->GetName() == "FPS")
		//	{
		//		m_FPSObject = gameObj;
		//	}
		//}

		// Based on the FPS prefab, create the object, position is already set in the prefab
		m_FPSObject = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("FPS", *m_FPSObject);

		// Set it to 0.f when not active ( wanted to do ToggleActive but didn't work )
		m_FPSObject->GetComponent<CPText>()->get_msg().scale = 0.0f;
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
		if (INPUT->IsKeyTriggered(KeyCode::KEY_J))
		{
			m_ShowFPS = !m_ShowFPS;
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
		// If key J is pressed, show the FPS.
		if (m_ShowFPS)
		{
			m_FPSObject->GetComponent<CPText>()->get_msg().scale = 1.0f;
			//m_FPSObject->GetComponent<CPText>()->ToggleActive(true);
		}
		else if (!m_ShowFPS)
		{
			m_FPSObject->GetComponent<CPText>()->get_msg().scale = 0.0f;
		}

	}

	void CPPSFPS::Destroy()
	{
		
	}
}