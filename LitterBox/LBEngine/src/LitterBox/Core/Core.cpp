/*!************************************************************************
 \file				Core.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief

 This file contains functions defintions of the LBEngine class that controls
 the engine and systems. Has a layer system to allow for ImGui implementation
 later on.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "Core.h"
#include <GLFW/glfw3.h>
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Debugging/Profiler.h"

namespace LB
{
	LBEngine* CORE;

	/*!***********************************************************************
	 \brief
	 Initalises the Engine by running it and initialises all its systems too

	 \return
	 Nothing
	*************************************************************************/
	void LBEngine::Initialise()
	{
		m_running = true;
		if (!CORE)
		{
			CORE = this;
		}
		else
		{
			std::cerr << "LitterBox engine already exist\n";
		}

		for (unsigned i = 0; i < m_systems.size(); ++i)
			m_systems[i]->Initialize();
	}

	/*!***********************************************************************
	 \brief
	 The game loop where all the layers, systems and time updates

	 \return
	 Nothing
	*************************************************************************/
	void LBEngine::GameLoop()
	{
		while (m_running)
		{
			TIME->LBFrameStart();
			{
				//Profiler frameProfiler{ "Total Frame Time", ProfileResult::MANAGER, ProfileMap::SYSTEMS };

				// Update every system every fixed timestep (Usually for physics)
				TIME->AccumulateFixedUpdate();
				while (TIME->ShouldFixedUpdate()) 
				{
					for (unsigned i = 0; i < m_systems.size(); ++i) 
					{
						Profiler systemProfiler{ m_systems[i]->GetName().c_str(), ProfileResult::MANAGER, ProfileMap::SYSTEMS, false };
						m_systems[i]->FixedUpdate();
					}
				}

				// Update every system every frame
				for (unsigned i = 0; i < m_systems.size(); ++i) 
				{
					Profiler systemProfiler{ m_systems[i]->GetName().c_str(), ProfileResult::MANAGER, ProfileMap::SYSTEMS };
					m_systems[i]->Update();
				}
			}

			TIME->LBFrameEnd();
		}
	}

	/*!***********************************************************************
	 \brief
	 Sends a message to all of the systems

	 \return
	 Nothing
	*************************************************************************/
	void LBEngine::BroadcastMessage(Message* message)
	{
		//The message that tells the game to quit
		if (message->MessageID == Quit)
			m_running = false;

		//Send the message to every system
		for (unsigned i = 0; i < m_systems.size(); ++i)
			m_systems[i]->SendMessage(message);
	}

	/*!***********************************************************************
	 \brief
	 Adds a system into the engine

	 \return
	 Nothing
	*************************************************************************/
	void LBEngine::AddSystem(std::unique_ptr<ISystem> system)
	{
		//Add a system to the core to be updated every frame
		m_systems.push_back(std::move(system));
	}

	/*!***********************************************************************
	 \brief
	 Destroy all the systems attached to the systems in reverse order

	 \return
	 Nothing
	*************************************************************************/
	void LBEngine::DestroySystems()
	{
		for (unsigned i = 0; i < m_systems.size(); ++i)
		{
			m_systems[m_systems.size() - i - 1]->Destroy();
			//delete m_systems[m_systems.size() - i - 1];
		}
	}

	/*!***********************************************************************
	 \brief
	 Checks if the engine is running

	 \return
	 True if running, else false
	*************************************************************************/
	bool LBEngine::IsRunning() const
	{
		return m_running ? true : false;
	}
}