#pragma once

#include "pch.h"
#include "System.h"
#include "LitterBox/Core/Message.h"

namespace LB 
{
	class LBEngine
	{
	public:
		LBEngine();
		~LBEngine();
		///Update all the systems until the game is no longer active.
		void GameLoop();
		///Destroy all systems in reverse order that they were added.
		void DestroySystems();
		bool IsRunning();
		///Broadcasts a message to all systems.
		void BroadcastMessage(Message* m);
		///Adds a new system to the game.
		void AddSystem(ISystem* system);
		///Initializes all systems in the game.
		void Initialize();
	private:
		//Tracks all the systems the game uses
		std::vector<ISystem*> Systems;
		//The last time the game was updated
		unsigned m_LastTime;
		//Is the game running (true) or being shut down (false)?
		bool m_Running;
	};

	//A global pointer to the instance of the core
	extern LBEngine* CORE;
}

