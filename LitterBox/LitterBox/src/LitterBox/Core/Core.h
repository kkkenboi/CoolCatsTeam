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
		bool IsRunning();
		///Update all the systems until the game is no longer active.
		void GameLoop();
		void UpdateFPS(double dt);
		///Destroy all systems in reverse order that they were added.
		void DestroySystems();
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
		float m_DeltaTime;
		//Is the game running (true) or being shut down (false)?
		bool m_Running;
		float m_FPS;
		double m_FPSInterval = 1.0;
	};

	//A global pointer to the instance of the core
	extern LBEngine* CORE;
}

