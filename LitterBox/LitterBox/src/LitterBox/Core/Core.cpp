/*!************************************************************************
 \file				Core.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				This file contains functions definitions of the
					LBEngine class that controls the engine and systems.
					Has a layer system to allow for ImGui implementation
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
		m_Running = true;
		if (!CORE)
		{
			CORE = this;
		}
		else
		{
			std::cerr << "LitterBox engine already exist\n";
		}

		for (unsigned i = 0; i < Systems.size(); ++i)
			Systems[i]->Initialize();
	}

	/*!***********************************************************************
	 \brief
	 The game loop where all the layers, systems and time updates

	 \return
	 Nothing
	*************************************************************************/
	void LBEngine::GameLoop()
	{
		while (m_Running)
		{
			TIME->LBFrameStart();
			{
				Profiler frameProfiler{ "Total Frame Time", ProfileResult::MANAGER, ProfileMap::SYSTEMS };

				for (Layer* layer : m_LayerStack)
				{
					layer->UpdateLayer();
				}

				// Update every system 
				for (unsigned i = 0; i < Systems.size(); ++i) 
				{
					Profiler systemProfiler{ Systems[i]->GetName().c_str(), ProfileResult::MANAGER, ProfileMap::SYSTEMS };
					Systems[i]->Update();
				}

				TIME->AccumulateFixedUpdate();
				while (TIME->ShouldFixedUpdate()) 
				{
					std::cout << "RAN!\n";

					for (unsigned i = 0; i < Systems.size(); ++i) 
					{
						Profiler systemProfiler{ Systems[i]->GetName().c_str(), ProfileResult::MANAGER, ProfileMap::SYSTEMS, false };
						Systems[i]->FixedUpdate();
					}
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
			m_Running = false;

		//Send the message to every system
		for (unsigned i = 0; i < Systems.size(); ++i)
			Systems[i]->SendMessage(message);
	}

	/*!***********************************************************************
	 \brief
	 Adds a system into the engine

	 \return
	 Nothing
	*************************************************************************/
	void LBEngine::AddSystem(ISystem* system)
	{
		//Add a system to the core to be updated every frame
		Systems.push_back(system);
	}

	/*!***********************************************************************
	 \brief
	 Destroy all the systems attached to the systems in reverse order

	 \return
	 Nothing
	*************************************************************************/
	void LBEngine::DestroySystems()
	{
		for (unsigned i = 0; i < Systems.size(); ++i)
		{
			Systems[Systems.size() - i - 1]->Destroy();
			delete Systems[Systems.size() - i - 1];
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
		return m_Running ? true : false;
	}
	
	/*!***********************************************************************
	 \brief
	 Adds a layer from the layerStack
	*************************************************************************/
	void LBEngine::AddLayer(Layer* layer)
	{
		m_LayerStack.AddLayer(layer);
	}

	/*!***********************************************************************
	 \brief
	 Removes a layer from the layerStack
	*************************************************************************/
	void LBEngine::RemoveLayer(Layer* layer)
	{
		m_LayerStack.RemoveLayer(layer);
	}

}