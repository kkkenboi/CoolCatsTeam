#pragma once
#include "pch.h"
#include "LitterBox/Engine/Message.h"

#define UNREFERENCED_PARAMETER

namespace LB
{
	///System is a pure virtual base class (which is to say, an interface) that is
	///the base class for all systems used by the game. 
	class ISystem
	{
	public:

		///Systems can receive all message send to the Core. 
		// Will be updated with Logging ----------------------------------------------------
		virtual void SendMessage(Message* message) { UNREFERENCED_PARAMETER(message); };
		// ---------------------------------------------------------------------------------

		///All systems are updated every game frame.
		virtual void Update(float timeslice) = 0;

		///All systems provide a string name for debugging.
		virtual std::string GetName() = 0;

		///Initialize the system.
		virtual void Initialize() {};

		///All systems need a virtual destructor to have their destructor called 
		virtual ~ISystem() {}
	};
}
