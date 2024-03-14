/*!************************************************************************
 \file				TransformCommands.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				22/11/2023
 \brief

 This file contains the command for manipulating transforms 
 (Translate/Scale/Rotate)

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
		/*!************************************************************************
		 * \brief Construct a new Move Command object
		 * 
		 * \param transform Transform to manipulate (Position)
		 * \param newPos New position to set the transform to
		**************************************************************************/
		MoveCommand(CPTransform* transform, Vec2<float> newPos) : m_transform{ transform }, m_oldPos{ transform->GetLocalPosition() }, m_newPos { newPos } { }

		/*!************************************************************************
		 * \brief Executes the move postion command
		**************************************************************************/
		void Execute() override
		{
			m_transform->SetPosition(m_newPos);
		}

		/*!************************************************************************
		 * \brief Undos the move position command
		**************************************************************************/
		void Undo() override
		{
			m_transform->SetPosition(m_oldPos);
		}

		/*!************************************************************************
		 * \brief Merges the move position commands in the command history together
		 * \param incomingCommand Incomming command to check against
		**************************************************************************/
		bool Merge(std::shared_ptr<ICommand> incomingCommand) override
		{
			std::shared_ptr<MoveCommand> incomingMoveCommand = std::dynamic_pointer_cast<MoveCommand>(incomingCommand);
			if (incomingMoveCommand->m_transform == m_transform)
			{
				m_newPos = incomingMoveCommand->m_newPos;
				return true;
			}
			return false;
		}

		/*!************************************************************************
		 * \brief Get the command type, used for merging
		**************************************************************************/
		CommandType GetType() override
		{
			return CommandType::MOVE;
		}

		/*!************************************************************************
		 * \brief What should be done when the command is removed from the manager
		**************************************************************************/
		void OnRemove() override { }

	private:
		CPTransform* m_transform;
		Vec2<float> m_oldPos, m_newPos;
	};

	class ScaleCommand : public ICommand
	{
	public:
		/*!************************************************************************
		 * \brief Construct a new Scale Command object
		 * \param transform Transform scale to manipulate
		 * \param newScale New scale to set to
		**************************************************************************/
		ScaleCommand(CPTransform* transform, Vec2<float> newScale) : m_transform{ transform }, m_oldScale{ transform->GetLocalScale() }, m_newScale{ newScale } { }

		/*!************************************************************************
		 * \brief Executes the scale command
		**************************************************************************/
		void Execute() override
		{
			m_transform->SetScale(m_newScale);
		}

		/*!************************************************************************
		 * \brief Undos the scale command
		**************************************************************************/
		void Undo() override
		{
			m_transform->SetScale(m_oldScale);
		}

		/*!************************************************************************
		 * \brief Merges the scale command
		 * \param incomingCommand Command to check
		**************************************************************************/
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

		/*!************************************************************************
		 * \brief Get the Type object
		 * \return CommandType Scale command
		**************************************************************************/
		CommandType GetType() override
		{
			return CommandType::SCALE;
		}

		/*!************************************************************************
		 * \brief What should be done when the command is removed from the manager
		**************************************************************************/
		void OnRemove() override { }

	private:
		CPTransform* m_transform;
		Vec2<float> m_oldScale, m_newScale;
	};

	class RotateCommand : public ICommand
	{
	public:
		/*!************************************************************************
		 * \brief Construct a new Rotate Command object
		 * \param transform Transform rotate to manipulate
		 * \param newRotation New rotation to set to
		**************************************************************************/
		RotateCommand(CPTransform* transform, float newRotation) : m_transform{ transform }, m_oldRotation{ transform->GetLocalRotation() }, m_newRotation{ newRotation } { }

		/*!************************************************************************
		 * \brief Executes the rotate command
		**************************************************************************/
		void Execute() override
		{
			m_transform->SetRotation(m_newRotation);
		}

		/*!************************************************************************
		 * \brief Undos the rotate command
		**************************************************************************/
		void Undo() override
		{
			m_transform->SetRotation(m_oldRotation);
		}

		/*!************************************************************************
		 * \brief Merges the rotate commands
		 * \param incomingCommand Command to check and possibly merge with
		**************************************************************************/
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

		/*!************************************************************************
		 * \brief Get the Type object
		 * \return CommandType Rotate command
		**************************************************************************/
		CommandType GetType() override
		{
			return CommandType::ROTATE;
		}

		/*!************************************************************************
		 * \brief What should be done when the command is removed from the manager
		**************************************************************************/
		void OnRemove() override { }

	private:
		CPTransform* m_transform;
		float m_oldRotation, m_newRotation;
	};
}
