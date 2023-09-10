#include "pch.h"
#include "Core.h"
#include <GLFW/glfw3.h>

namespace LB
{
	LBEngine* CORE;

	LBEngine::LBEngine()
	{
		m_LastTime = 0;
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
		//Initialize the last time variable so our first frame
		//is "zero" seconds (and not some huge unknown number)

		m_LastTime = glfwGetTime();

		while (m_Running)
		{

			//Get the current time in milliseconds
			unsigned currenttime = glfwGetTime();
			//Convert it to the time passed since the last frame (in seconds)
			float dt = (currenttime - m_LastTime) / 1000.0f;
			//Update the when the last update started
			m_LastTime = currenttime;

			//Update every system and tell each one how much
			//time has passed since the last update
			for (unsigned i = 0; i < Systems.size(); ++i)
				Systems[i]->Update(dt);
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
			delete Systems[Systems.size() - i - 1];
		}
	}

	bool LBEngine::IsRunning()
	{
		return m_Running ? true : false;
	}


}