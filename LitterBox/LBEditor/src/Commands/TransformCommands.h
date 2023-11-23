/*!************************************************************************
 \file				TransformCommands.h
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
#include "LitterBox/Utils/Math.h"
#include "LitterBox/Components/TransformComponent.h"

namespace LB
{
	class MoveCommand : public ICommand
	{
	public:
		MoveCommand(CPTransform* transform, Vec2<float> newPos) : m_transform{ transform }, m_oldPos{ transform->GetPosition() }, m_newPos { newPos } { }

		void Execute() override
		{
			m_transform->SetPosition(m_newPos);
		}

		void Undo() override
		{
			m_transform->SetPosition(m_oldPos);
		}

		bool Merge(std::shared_ptr<ICommand> incomingCommand) override
		{
			std::shared_ptr<MoveCommand> incomingMoveCommand = std::dynamic_pointer_cast<MoveCommand>(incomingCommand);
			if (incomingMoveCommand->m_transform == m_transform && incomingMoveCommand->m_oldPos == m_newPos)
			{
				m_newPos = incomingMoveCommand->m_newPos;
				return true;
			}
			return false;
		}

		CommandType GetType() override
		{
			return CommandType::MOVE;
		}

	private:
		CPTransform* m_transform;
		Vec2<float> m_oldPos, m_newPos;
	};
}
