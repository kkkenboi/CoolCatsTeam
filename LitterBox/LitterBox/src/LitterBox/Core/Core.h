#pragma once

#include "pch.h"
#include "System.h"
#include "LitterBox/Engine/LayerStack.h"

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

		void AddLayer(Layer* layer)
		{
			m_LayerStack.AddLayer(layer);
		}
		void RemoveLayer(Layer* layer)
		{
			m_LayerStack.RemoveLayer(layer);
		}
	private:
		LayerStack m_LayerStack;
		//Tracks all the systems the game uses
		std::vector<ISystem*> Systems;
		//Is the game running (true) or being shut down (false)?
		bool m_Running;

		double m_FPS{};
		double m_FPSInterval{ 1.0 };
	};

	//A global pointer to the instance of the core
	extern LBEngine* CORE;
}

