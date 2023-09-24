#pragma once
#include "pch.h"
#include "LitterBox/Engine/Message.h"

#define UNREFERENCED_PARAMETER

namespace LB
{
	// System is a pure virtual base class (which is to say, an interface) that is
	// the base class for all systems used by the game. 
	class ISystem
	{
	public:

		// Systems can receive all message send to the Core. 
		// Will be updated with Logging ----------------------------------------------------
		virtual void SendMessage(Message* message) { UNREFERENCED_PARAMETER(message); };
		// ---------------------------------------------------------------------------------

		// Update is called every game frame.
		virtual void Update() {}

		// Fixed update is called independent of frame rate at a constant pace.
		virtual void FixedUpdate() {}

		// All systems provide a string name for debugging.
		virtual std::string const& GetName() { return systemName; }

		// Initialize the system before the core game loop.
		virtual void Initialize() {}

		// Destroy the system after the core game loop.
		virtual void Destroy() {}

		// Set name of the system
		void SetSystemName(std::string newName) { systemName = newName; }

	private:
		std::string systemName{};
	};
}
