/*!************************************************************************
 \file				GameObjectCommands.cpp
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
		RemoveObjectCommand(GameObject* GOToRemove) : m_removedGO{ GOToRemove }, m_removedGOParent{ m_removedGO->GetComponent<CPTransform>()->GetParent() }, m_lastPos{m_removedGO->GetComponent<CPTransform>()->GetPosition()} { }

		void Execute() override
		{
			GOMANAGER->DetachGameObject(m_removedGO);

			// TODO: Refactor when SetActive for GameObject is implemented
			if (m_removedGO->HasComponent<CPRender>())
				m_removedGO->GetComponent<CPRender>()->set_active();
			m_removedGO->GetComponent<CPTransform>()->GetParent()->RemoveChild(m_removedGO->GetComponent<CPTransform>());
			m_removedGO->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
		}

		void Undo() override
		{
			GOMANAGER->AddGameObject(m_removedGO);

			// TODO: Refactor when SetActive for GameObject is implemented
			if (m_removedGO->HasComponent<CPRender>())
				m_removedGO->GetComponent<CPRender>()->set_active();
			m_removedGOParent->AddChild(m_removedGO->GetComponent<CPTransform>());
			m_removedGO->GetComponent<CPTransform>()->SetPosition(Vec2<float>{m_lastPos});
		}

		bool Merge(std::shared_ptr<ICommand> incomingCommand) override
		{
			UNREFERENCED_PARAMETER(incomingCommand);
			return false; // This command can never merge
		}

		CommandType GetType() override
		{
			return DELETEGO;
		}

		~RemoveObjectCommand() override
		{
			if (!GOMANAGER->IsGameObjectInScene(m_removedGO))
			{
				m_removedGO->Destroy();
			}
		}

	private:
		GameObject* m_removedGO;
		CPTransform* m_removedGOParent;

		Vec2<float> m_lastPos;
	};

	class SpawnObjectCommand : public ICommand
	{
	public:
		SpawnObjectCommand() { }

		SpawnObjectCommand(std::string const& prefabData, Vec2<float> const& spawnPos) : m_prefabData{ prefabData }, m_spawnPos{ spawnPos } { }

		void Execute() override
		{
			// Different spawn criteria
			if (!m_prefabData.empty())
				m_spawnedGO = ASSETMANAGER->SpawnGameObject(m_prefabData, m_spawnPos);
			else
				m_spawnedGO = FACTORY->SpawnGameObject();
		}

		void Undo() override
		{
			GOMANAGER->RemoveGameObject(m_spawnedGO);
		}

		bool Merge(std::shared_ptr<ICommand> incomingCommand) override
		{
			UNREFERENCED_PARAMETER(incomingCommand);
			return false; // This command can never merge
		}

		CommandType GetType() override
		{
			return SPAWNGO;
		}

		~SpawnObjectCommand() override
		{
			if (!GOMANAGER->IsGameObjectInScene(m_spawnedGO))
			{
				m_spawnedGO->Destroy();
			}
		}

	private:
		GameObject* m_spawnedGO{ nullptr };

		// Details about the new GO
		std::string m_prefabData {};
		Vec2<float> m_spawnPos {};
	};
}
