/*!************************************************************************
 \file				GameObjectCommands.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				22/11/2023
 \brief

 This file contains the commands for manipulating game objects (removing/creating)

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <memory>
#include "Utils/Command.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Components/RenderComponent.h"

namespace LB
{
	class RemoveObjectCommand : public ICommand
	{
	public:
		/*!************************************************************************
		* \brief Construct a new Remove Object Command
		* \param GOToRemove GameObject to be removed
		**************************************************************************/
		RemoveObjectCommand(GameObject* GOToRemove) : m_removedGO{ GOToRemove }, m_removedGOParent{ m_removedGO->GetComponent<CPTransform>()->GetParent() }, m_lastPos{m_removedGO->GetComponent<CPTransform>()->GetPosition()} { }

		/*!************************************************************************
		 * \brief Executes the remove gameobject command
		**************************************************************************/
		void Execute() override
		{
			GOMANAGER->DetachGameObject(m_removedGO);

			// Remove object from inspection if inspected
			if (InspectorGameObject::Instance()->IsGOInspected() && InspectorGameObject::Instance()->GetInspectedGO() == m_removedGO)
			{
				EDITORHIERACHY->onNewObjectSelected.Invoke(nullptr);
			}

			// TODO: Refactor when SetActive for GameObject is implemented
			if (m_removedGO->HasComponent<CPRender>())
				m_removedGO->GetComponent<CPRender>()->set_active();
			m_removedGO->GetComponent<CPTransform>()->GetParent()->RemoveChild(m_removedGO->GetComponent<CPTransform>());
			m_removedGO->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
		}

		/*!************************************************************************
		 * \brief Undoes the remove gameobject command
		**************************************************************************/
		void Undo() override
		{
			GOMANAGER->AddGameObject(m_removedGO);

			// TODO: Refactor when SetActive for GameObject is implemented
			if (m_removedGO->HasComponent<CPRender>())
				m_removedGO->GetComponent<CPRender>()->set_active();
			m_removedGOParent->AddChild(m_removedGO->GetComponent<CPTransform>());
			m_removedGO->GetComponent<CPTransform>()->SetPosition(Vec2<float>{m_lastPos});
		}

		/*!************************************************************************
		 * \brief Merges commands together, but this command in particular cannot
		 * merge.
		**************************************************************************/
		bool Merge(std::shared_ptr<ICommand> incomingCommand) override
		{
			UNREFERENCED_PARAMETER(incomingCommand);
			return false; // This command can never merge
		}

		/*!************************************************************************
		 * \brief Get the command type, used for merging
		**************************************************************************/
		CommandType GetType() override
		{
			return DELETEGO;
		}

		/*!************************************************************************
		 * \brief What should be done when the command is removed from the manager
		**************************************************************************/
		void OnRemove() override
		{
			// If the most recent command was this, don't destroy! 
			// (E.g. Delete -> Undo -> Delete) This current command on the redo stack will try to delete the same GO
			auto lastCommand = COMMAND->GetLastCommand();
			if (lastCommand->GetType() == GetType())
			{
				std::shared_ptr<RemoveObjectCommand> deleteCommand = std::dynamic_pointer_cast<RemoveObjectCommand>(lastCommand);
				if (deleteCommand->m_removedGO == m_removedGO)
					return;
			}

			if (m_removedGO && !GOMANAGER->IsGameObjectInScene(m_removedGO))
			{
				m_removedGO->Destroy();
			}
		}

		GameObject* m_removedGO;

	private:
		CPTransform* m_removedGOParent;

		Vec2<float> m_lastPos;
	};

	class SpawnObjectCommand : public ICommand
	{
	public:
		/*!************************************************************************
		 * \brief Construct a new Spawn Object Command object
		**************************************************************************/
		SpawnObjectCommand() { }

		/*!************************************************************************
		 * \brief Construct a new Spawn Object Command object
		 * \param prefabData prefab to spawn
		 * \param spawnPos position of prefab to spawn 
		**************************************************************************/
		SpawnObjectCommand(std::string const& prefabData, Vec2<float> const& spawnPos) : m_prefabData{ prefabData }, m_spawnPos{ spawnPos } { }

		/*!************************************************************************
		 * \brief Executes the spawn gameobject command
		**************************************************************************/
		void Execute() override
		{
			// If the object already exists (Meaning the command has undoed and redoed)
			if (m_spawnedGO)
			{
				GOMANAGER->AddGameObject(m_spawnedGO);

				// TODO: Refactor when SetActive for GameObject is implemented
				if (m_spawnedGO->HasComponent<CPRender>())
					m_spawnedGO->GetComponent<CPRender>()->set_active();
				m_spawnedGOParent->AddChild(m_spawnedGO->GetComponent<CPTransform>());
				m_spawnedGO->GetComponent<CPTransform>()->SetPosition(Vec2<float>{m_spawnPos});
				return;
			}

			// Different spawn criteria
			if (!m_prefabData.empty())
				m_spawnedGO = ASSETMANAGER->SpawnGameObject(m_prefabData, m_spawnPos);
			else
				m_spawnedGO = FACTORY->SpawnGameObject();

			m_spawnedGOParent = m_spawnedGO->GetComponent<CPTransform>()->GetParent();
		}
		
		/*!************************************************************************
		 * \brief Undos the spawn gameobject command
		**************************************************************************/
		void Undo() override
		{
			// Remove object from inspection if inspected
			if (InspectorGameObject::Instance()->IsGOInspected() && InspectorGameObject::Instance()->GetInspectedGO() == m_spawnedGO)
			{
				EDITORHIERACHY->onNewObjectSelected.Invoke(nullptr);
			}

			// TODO: Refactor when SetActive for GameObject is implemented
			if (m_spawnedGO->HasComponent<CPRender>())
				m_spawnedGO->GetComponent<CPRender>()->set_active();
			m_spawnedGO->GetComponent<CPTransform>()->GetParent()->RemoveChild(m_spawnedGO->GetComponent<CPTransform>());
			m_spawnedGO->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

			GOMANAGER->DetachGameObject(m_spawnedGO);
		}

		/*!************************************************************************
		 * \brief Merges commands together, but this command in particular cannot
		 * merge.
		**************************************************************************/
		bool Merge(std::shared_ptr<ICommand> incomingCommand) override
		{
			UNREFERENCED_PARAMETER(incomingCommand);
			return false; // This command can never merge
		}

		/*!************************************************************************
		 * \brief Get the command type, used for merging
		**************************************************************************/
		CommandType GetType() override
		{
			return SPAWNGO;
		}

		/*!************************************************************************
		 * \brief What should be done when the command is removed from the manager
		**************************************************************************/
		void OnRemove() override
		{
			// TODO: Refactor spawn and delete logic
			auto& redoHist = COMMAND->GetRedoHistory();
			for (auto& command : redoHist)
			{
				if (command->GetType() == DELETEGO)
				{
					std::shared_ptr<RemoveObjectCommand> deleteCommand = std::dynamic_pointer_cast<RemoveObjectCommand>(command);
					if (deleteCommand->m_removedGO == m_spawnedGO)
						deleteCommand->m_removedGO = nullptr;
				}
			}

			if (!GOMANAGER->IsGameObjectInScene(m_spawnedGO))
			{
				m_spawnedGO->Destroy();
			}
		}

	private:
		GameObject* m_spawnedGO{ nullptr };
		CPTransform* m_spawnedGOParent{ nullptr };

		// Details about the new GO
		std::string m_prefabData {};
		Vec2<float> m_spawnPos {};
	};
}
