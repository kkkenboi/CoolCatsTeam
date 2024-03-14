/*!************************************************************************
 \file				CommandManager.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				22/11/2023
 \brief

 This file implements the command manager class which will handle all the
 commands done during the editor runtime. Any commands done must be
 added to the command manager to be tracked.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox/Scene/SceneManager.h"
#include "LitterBox/Engine/Input.h"
#include "CommandManager.h"
#include "LitterBox/Core/Core.h"

namespace LB
{
	CommandManager* COMMAND = nullptr;

	/*!***********************************************************************
	 \brief
	 Constructor, sets up the CommandManager and subscribes to the key presses
	 to enable undoing and redoing.
	*************************************************************************/
	CommandManager::CommandManager() : history{}, undoHistory{}, m_savedCommandIndex{ 0 }
	{
		if (!COMMAND)
			COMMAND = this;
		else
			DebuggerLogError("Command System already exists!");

		// How many commands to store
		ResizeHistory(20);

		// Input event for undo/redo
		INPUT->SubscribeToKey(CheckUndo, KeyCode::KEY_Z, KeyEvent::TRIGGERED, KeyTriggerType::NONPAUSABLE);
		INPUT->SubscribeToKey(CheckRedo, KeyCode::KEY_Y, KeyEvent::TRIGGERED, KeyTriggerType::NONPAUSABLE);

		// Scene events
		SCENEMANAGER->onSceneSaved.Subscribe(LB::UpdateCommandsSaved);
		CORE->onPlayingModeToggle.Subscribe(LB::ClearHistory, 10);
	}

	/*!***********************************************************************
	 \brief
	 Whenever a command (action) is done, the command is added to the manager
	 through this function.
	*************************************************************************/
	void CommandManager::AddCommand(std::shared_ptr<ICommand> newCommand)
	{
		// If in play mode, don't register commands
		if (CORE->IsPlaying()) return;

		m_savedCommandIndex = 0; // Reset the saved command index

		newCommand->Execute();

		if (!history.empty() && history.front()->GetType() == newCommand->GetType())
		{
			if (!history.front()->Merge(newCommand)) 
			{
				AddToHistory(newCommand);
			}
		}
		else
		{
			AddToHistory(newCommand);
		}

		ClearRedoHistory();
	}

	/*!***********************************************************************
	 \brief
	 Internal function for command manager to add new commands, if the size of
	 commands stored exceeds the capacity, the command manager will remove
	 the oldest one.
	*************************************************************************/
	void CommandManager::AddToHistory(std::shared_ptr<ICommand> newCommand)
	{
		// If command history capacity is reached, remove oldest command
		if (history.size() > m_historyCapacity)
		{
			history.pop_back();
			history.push_front(newCommand);
		}
		else
		{
			history.push_front(newCommand);
		}
	}

	/*!***********************************************************************
	 \brief
	 Upon the undo key press, the last command given will be undone and it
	 will be stored on the redo history.
	*************************************************************************/
	void CommandManager::Undo()
	{
		// Commands only for editor mode
		if (CORE->IsPlaying())
		{
			DebuggerLogWarning("Commands enabled for Editor mode only!");
			return;
		}
		
		if (history.empty()) return;

		// Execute the undo of the command
		history.front()->Undo();

		// Add command to redo history and remove from history
		undoHistory.push_front(history.front());
		history.pop_front();
	}

	/*!***********************************************************************
	 \brief
	 If there are any commands undone, upon the redo key press, the command
	 will be executed again.
	*************************************************************************/
	void CommandManager::Redo()
	{
		// Commands only for editor mode
		if (CORE->IsPlaying())
		{
			DebuggerLogWarning("Commands enabled for Editor mode only!");
			return;
		}

		if (undoHistory.empty()) return;

		// Execute the command again
		undoHistory.front()->Execute();

		// Add command to history and remove from redo history
		AddToHistory(undoHistory.front());
		undoHistory.pop_front();
	}

	/*!***********************************************************************
	 \brief
	 Changes how many commands to store in memory based on the newSize given,
	 if the newSize is smaller than the current size, the oldest commands
	 will be deleted.
	*************************************************************************/
	void CommandManager::ResizeHistory(int newSize)
	{
		m_historyCapacity = newSize;
	}

	/*!***********************************************************************
	 \brief
	 Clears any command saved.
	*************************************************************************/
	void CommandManager::ClearHistory()
	{
		for (auto& command : history) 
		{
			command->OnRemove();
		}

		history.clear();
		m_savedCommandIndex = 0;
	}

	/*!***********************************************************************
	 \brief
	 Clears any commands undone (Saved to redo history).
	*************************************************************************/
	void CommandManager::ClearRedoHistory()
	{
		for (auto& command : undoHistory)
		{
			command->OnRemove();
		}

		undoHistory.clear();
	}

	/*!***********************************************************************
	 \brief
	 Check if there are there any command changes to the current scene
	 that are not saved.
	*************************************************************************/
	bool CommandManager::UpToDate()
	{
		return m_savedCommandIndex == history.size();
	}

	/*!***********************************************************************
	 \brief
	 For the command manager to check if any commands are not saved, the index
	 of the latest command needs to be updated here.
	*************************************************************************/
	void CommandManager::UpdateCommandsSaved()
	{
		m_savedCommandIndex = static_cast<int>(history.size());
	}

	/*!***********************************************************************
	 \brief
	 Returns the last command send to the command manager.
	*************************************************************************/
	std::shared_ptr<ICommand> CommandManager::GetLastCommand()
	{
		return history.front();
	}

	/*!***********************************************************************
	 \brief
	 Returns all the 'undone' commands currently stored in the redo history.
	*************************************************************************/
	std::deque<std::shared_ptr<ICommand>>& CommandManager::GetRedoHistory()
	{
		return undoHistory;
	}

	/*!***********************************************************************
	 \brief
	 For event subscriptions, these global functions call the same function
	 in the command manager.
	*************************************************************************/
	void CheckUndo()
	{
		if (INPUT->IsKeyPressed(KeyCode::KEY_LEFTCONTROL))
		{
			COMMAND->Undo();
		}
	}
	void CheckRedo()
	{
		if (INPUT->IsKeyPressed(KeyCode::KEY_LEFTCONTROL))
		{
			COMMAND->Redo();
		}
	}
	void UpdateCommandsSaved()
	{
		COMMAND->UpdateCommandsSaved();
	}
	void ClearHistory(bool isPlaying)
	{
		if (isPlaying)
			COMMAND->ClearHistory();
	}
	/************************************************************************/
}
