/*!************************************************************************
 \file				CommandManager.cpp
 \author(s)
 \par DP email(s):
 \par Course:       CSD2401A
 \date				22/11/2023
 \brief

 This file contains

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox/Scene/SceneManager.h"
#include "LitterBox/Engine/Input.h"
#include "CommandManager.h"

namespace LB
{
	CommandManager* COMMAND = nullptr;

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
		INPUT->SubscribeToKey(CheckRedo, KeyCode::KEY_R, KeyEvent::TRIGGERED, KeyTriggerType::NONPAUSABLE);

		// Scene events
		SCENEMANAGER->onSceneSaved.Subscribe(LB::UpdateCommandsSaved);
		SCENEMANAGER->onNewSceneLoad.Subscribe(LB::ClearHistory);
	}

	void CommandManager::AddCommand(std::shared_ptr<ICommand> newCommand)
	{
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

	void CommandManager::Undo()
	{
		if (history.empty()) return;

		// Execute the undo of the command
		history.front()->Undo();

		// Add command to redo history and remove from history
		undoHistory.push_front(history.front());
		history.pop_front();
	}

	void CommandManager::Redo()
	{
		if (undoHistory.empty()) return;

		// Execute the command again
		undoHistory.front()->Execute();

		// Add command to history and remove from redo history
		AddToHistory(undoHistory.front());
		undoHistory.pop_front();
	}

	void CommandManager::ResizeHistory(int newSize)
	{
		m_historyCapacity = newSize;
	}

	void CommandManager::ClearHistory()
	{
		for (auto& command : history) 
		{
			command->OnRemove();
		}

		history.clear();
		m_savedCommandIndex = 0;
	}

	void CommandManager::ClearRedoHistory()
	{
		for (auto& command : undoHistory)
		{
			command->OnRemove();
		}

		undoHistory.clear();
	}

	bool CommandManager::UpToDate()
	{
		return m_savedCommandIndex == history.size();
	}

	void CommandManager::UpdateCommandsSaved()
	{
		m_savedCommandIndex = history.size();
	}

	std::shared_ptr<ICommand> CommandManager::GetLastCommand()
	{
		return history.front();
	}

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

	void ClearHistory(Scene* newScene)
	{
		UNREFERENCED_PARAMETER(newScene);
		COMMAND->ClearHistory();
	}
}
