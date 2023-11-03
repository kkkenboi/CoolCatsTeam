/*!************************************************************************
 \file				Core.cpp
 \author(s)			Kenji Brannon Chong, Ang Jiawei Jarrett
 \par DP email(s):	kenjibrannon.c@digipen.edu, a.jiaweijarrett@digipen.edu
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
	 Sets the global pointer to this instance
	*************************************************************************/
	LBEngine::LBEngine()
	{
		if (!CORE)
			CORE = this;
		else
			DebuggerLogError("LitterBox engine already exist");
	}

	/*!***********************************************************************
	 \brief
	 Initalises the Engine by running it and initialises all its systems too
	*************************************************************************/
	void LBEngine::Initialise()
	{
		m_running = true;

		for (unsigned i = 0; i < m_systems.size(); ++i)
			m_systems[i]->Initialize();
	}

	/*!***********************************************************************
	 \brief
	 The game loop where all the layers, systems and time updates
	*************************************************************************/
	void LBEngine::GameLoop()
	{
		while (m_running)
		{
			TIME->LBFrameStart();
			{
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
	*************************************************************************/
	void LBEngine::AddSystem(std::shared_ptr<ISystem> system)
	{
		//Add a system to the core to be updated every frame
		m_systems.push_back(system);
	}

	/*!***********************************************************************
	 \brief
	 Destroy all the systems attached to the systems in reverse order
	*************************************************************************/
	void LBEngine::DestroySystems()
	{
		for (unsigned i = 0; i < m_systems.size(); ++i)
		{
			m_systems[m_systems.size() - i - 1]->Destroy();
			//m_systems[m_systems.size() - i - 1].reset();
			//delete m_systems[m_systems.size() - i - 1];
		}
	}

	//-----------------------------Setters and Getters for states-----------------------------

	/*!***********************************************************************
	 \brief
	 Returns true if the engine is running
	*************************************************************************/
	bool LBEngine::IsRunning() const
	{
		return m_running;
	}

	/*!***********************************************************************
	 \brief
	 Returns true if the game is in editor mode, m_editorMode is a bool 
	 to check if the game is in the editor view
	*************************************************************************/
	bool LBEngine::IsEditorMode() const
	{
		return m_editorMode;
	}

	/*!***********************************************************************
	 \brief
	 Flips the state of m_editorMode which is a bool to check if the game
	 is in the editor view
	*************************************************************************/
	void LBEngine::ToggleEditorMode()
	{
		SetEditorMode(!m_editorMode);
	}

	/*!***********************************************************************
	 \brief
	 Sets the state of m_editorMode to the given state, m_editorMode is a bool 
	 to check if the game is in the editor view
	*************************************************************************/
	void LBEngine::SetEditorMode(bool newState)
	{
		m_editorMode = newState;
		onEditorModeToggle.Invoke(m_editorMode);
	}

	/*!***********************************************************************
	 \brief
	 Returns true if the actual game is playing
	*************************************************************************/
	bool LBEngine::IsPlaying() const
	{
		return m_isPlaying;
	}

	/*!***********************************************************************
	 \brief
	 Flips the m_isPlaying state
	*************************************************************************/
	void LBEngine::TogglePlaying()
	{
		SetPlayingMode(!m_isPlaying);
	}

	/*!***********************************************************************
	 \brief
	 Sets the m_isPlaying state to the given one, m_isPlaying checks if the
	 game is actually running (not editor)
	*************************************************************************/
	void LBEngine::SetPlayingMode(bool newState)
	{
		m_isPlaying = newState;
		onPlayingModeToggle.Invoke(m_isPlaying);
	}

	/*!***********************************************************************
	 \brief
	 Returns true if the editor ImGUI has initialized and launched
	*************************************************************************/
	bool LBEngine::IsEditorLaunched() const
	{
		return m_editorLaunch;
	}

	/*!***********************************************************************
	 \brief
	 Sets the m_editorLaunch flag to the given state, this checks if the
	 editor ImGUI has initialized and launched
	*************************************************************************/
	void LBEngine::SetEditorLaunched(bool newState)
	{
		m_editorLaunch = newState;
	}
}