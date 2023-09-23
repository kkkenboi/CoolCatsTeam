#include "pch.h"
#include "Core.h"
#include <GLFW/glfw3.h>
#include "LitterBox/Engine/Time.h"

namespace LB
{
	LBEngine* CORE;

	LBEngine::LBEngine()
	{
		m_Running = true;
		// ---- Should have a check here to determine if there is more than one engine running ----

		//-----------------------------------------------------------------------------------------
		CORE = this; //Set the global pointer
	}

	LBEngine::~LBEngine()
	{
		//Nothing for the destructor to do
	}

	void LBEngine::Initialize()
	{
		for (unsigned i = 0; i < Systems.size(); ++i)
			Systems[i]->Initialize();
	}

	void LBEngine::GameLoop()
	{
		while (m_Running)
		{
			TIME->LBFrameStart();

			// Update every system 
			for (unsigned i = 0; i < Systems.size(); ++i) 
			{
				Systems[i]->Update();
			}

			if (TIME->ShouldFixedUpdate()) 
			{
				for (unsigned i = 0; i < Systems.size(); ++i) 
				{
					Systems[i]->FixedUpdate();
				}
			}

			// Update FPS counter
			/*UpdateFPS(m_FPSInterval);
			std::cout << m_FPS << '\n';*/

			TIME->LBFrameEnd();
		}
	}

	void LBEngine::BroadcastMessage(Message* message)
	{
		//The message that tells the game to quit
		if (message->MessageId == Mid::Quit)
			m_Running = false;

		//Send the message to every system--each
		//system can figure out whether it cares
		//about a given message or not
		for (unsigned i = 0; i < Systems.size(); ++i)
			Systems[i]->SendMessage(message);
	}

	void LBEngine::AddSystem(ISystem* system)
	{
		//Add a system to the core to be updated
		//every frame
		Systems.push_back(system);
	}

	void LBEngine::DestroySystems()
	{
		//Delete all the systems in reverse order
		//that they were added in (to minimize any
		//dependency problems between systems)
		for (unsigned i = 0; i < Systems.size(); ++i)
		{
			Systems[Systems.size() - i - 1]->Destroy();
			delete Systems[Systems.size() - i - 1];
		}
	}

	bool LBEngine::IsRunning()
	{
		return m_Running ? true : false;
	}

	void LBEngine::UpdateFPS(double fpsUpdateInterval)
	{
		static double prevTime = glfwGetTime();
		double currTime = glfwGetTime();
		double m_DeltaTime = currTime - prevTime;
		prevTime = currTime;

		// fps calculations
		static double count = 0.0; // number of game loop iterations
		static double startTime = glfwGetTime();
		// get elapsed time since very beginning (in seconds) ...
		double elapsedTime = currTime - startTime;

		++count;

		// update fps at least every 10 seconds ...
		fpsUpdateInterval = (fpsUpdateInterval < 0.0) ? 0.0 : fpsUpdateInterval;
		fpsUpdateInterval = (fpsUpdateInterval > 10.0) ? 10.0 : fpsUpdateInterval;
		if (elapsedTime > fpsUpdateInterval) {
			m_FPS = count / elapsedTime;
			startTime = currTime;
			count = 0.0;
		}
	}
}