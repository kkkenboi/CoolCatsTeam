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

		void OnRemove() override { }

	private:
		CPTransform* m_transform;
		Vec2<float> m_oldPos, m_newPos;
	};

	class ScaleCommand : public ICommand
	{
	public:
		ScaleCommand(CPTransform* transform, Vec2<float> newScale) : m_transform{ transform }, m_oldScale{ transform->GetScale() }, m_newScale{ newScale } { }

		void Execute() override
		{
			m_transform->SetScale(m_newScale);
		}

		void Undo() override
		{
			m_transform->SetScale(m_oldScale);
		}

		bool Merge(std::shared_ptr<ICommand> incomingCommand) override
		{
			std::shared_ptr<ScaleCommand> incomingScaleCommand = std::dynamic_pointer_cast<ScaleCommand>(incomingCommand);
			if (incomingScaleCommand->m_transform == m_transform && incomingScaleCommand->m_oldScale == m_newScale)
			{
				m_newScale = incomingScaleCommand->m_newScale;
				return true;
			}
			return false;
		}

		CommandType GetType() override
		{
			return CommandType::SCALE;
		}

		void OnRemove() override { }

	private:
		CPTransform* m_transform;
		Vec2<float> m_oldScale, m_newScale;
	};

	class RotateCommand : public ICommand
	{
	public:
		RotateCommand(CPTransform* transform, float newRotation) : m_transform{ transform }, m_oldRotation{ transform->GetRotation() }, m_newRotation{ newRotation } { }

		void Execute() override
		{
			m_transform->SetRotation(m_newRotation);
		}

		void Undo() override
		{
			m_transform->SetRotation(m_oldRotation);
		}

		bool Merge(std::shared_ptr<ICommand> incomingCommand) override
		{
			std::shared_ptr<RotateCommand> incomingRotateCommand = std::dynamic_pointer_cast<RotateCommand>(incomingCommand);
			if (incomingRotateCommand->m_transform == m_transform && fabs(incomingRotateCommand->m_oldRotation - m_newRotation) < EPSILON_F)
			{
				m_newRotation = incomingRotateCommand->m_newRotation;
				return true;
			}
			return false;
		}

		CommandType GetType() override
		{
			return CommandType::ROTATE;
		}

		void OnRemove() override { }

	private:
		CPTransform* m_transform;
		float m_oldRotation, m_newRotation;
	};
}
