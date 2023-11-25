/*!************************************************************************
 \file				System.h
 \author(s)			Kenji Brannon Chong, Jarrett Ang Jiawei
 \par DP email(s):	kenjibrannon.c@digipen.edu, a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				29/09/23
 \brief				
 
 This file contains functions of the ISystem class which acts as an interface 
 for other systems to build off.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
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
	 the base class for all systems used by the engine. 
	*************************************************************************/
	class ISystem
	{
	public:

		/*!***********************************************************************
		 \brief
		 Systems can receive all message sent to the Engine's CORE. 
		*************************************************************************/
		virtual void SendMessage(Message* message) { UNREFERENCED_PARAMETER(message); }

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
		 Destroy the system after the core game loop
		*************************************************************************/
		virtual void Destroy() {}
1
		virtual ~ISystem() {}

		/*!***********************************************************************
		 \brief
		 Set name of the system
		*************************************************************************/
		void SetSystemName(std::string newName) { systemName = newName; }

	private:
		std::string systemName{};
	};
}

// Whenever a system includes this header,
// Add debugging functionality by default
#include "LitterBox/Debugging/Debug.h"

// Add LB-side memory allocation/ deallocation
#include "LitterBox/Debugging/Memory.h"
