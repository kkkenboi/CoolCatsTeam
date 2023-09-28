/*!************************************************************************
 \file
 \author(s)
 \par DP email(s):
 \par Course:		CSD2401A
 \date
 \brief

**************************************************************************/

#pragma once
#include "pch.h"
#include "LitterBox/Engine/Message.h"

#define UNREFERENCED_PARAMETER

namespace LB
{
	/*!***********************************************************************
	 \brief
	 System is a pure virtual base class (which is to say, an interface) that is
	 the base class for all systems used by the game. 

	*************************************************************************/
	class ISystem
	{
	public:

		/*!***********************************************************************
		 \brief
		 Systems can receive all message send to the Core. 
		 Will be updated with Logging 
		*************************************************************************/
		// ---------------------------------------------------------------------------------
		virtual void SendMessage(Message* message) { UNREFERENCED_PARAMETER(message); };
		// ---------------------------------------------------------------------------------

		/*!***********************************************************************
		 \brief
		 Update is called every game frame.

		*************************************************************************/
		virtual void Update() {}

		/*!***********************************************************************
		 \brief
		 Fixed update is called independent of frame rate at a constant pace.
		 
		*************************************************************************/
		virtual void FixedUpdate() {}

		/*!***********************************************************************
		 \brief
		 All systems provide a string name for debugging.

		*************************************************************************/
		virtual std::string const& GetName() { return systemName; }

		/*!***********************************************************************
		 \brief
		Initialize the system before the core game loop.

		*************************************************************************/
		virtual void Initialize() {}

		/*!***********************************************************************
		 \brief
		 Destroy the system after the core game loop.

		*************************************************************************/
		virtual void Destroy() {}

		/*!***********************************************************************
		 \brief
		 Set name of the system

		*************************************************************************/
		void SetSystemName(std::string newName) { systemName = newName; }

	private:
		/*!***********************************************************************
		 \brief

		*************************************************************************/
		std::string systemName{};
	};
}

// Whenever a system includes this header,
// Add debugging functionality by default
#include "LitterBox/Debugging/Debug.h"

// Add LB-side memory allocation/ deallocation
#include "LitterBox/Debugging/Memory.h"
