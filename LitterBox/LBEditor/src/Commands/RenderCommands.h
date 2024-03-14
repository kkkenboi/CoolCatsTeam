/*!************************************************************************
 \file				RenderCommands.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				14/03/2024
 \brief

 This file contains the command for manipulating the render component

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Utils/Command.h"
#include "LitterBox/Utils/Math.h"
#include "LitterBox/Components/RenderComponent.h"

namespace LB
{
	/*!************************************************************************
	\brief Command to change the width, height, z-axis of a render component
	**************************************************************************/
	class RenderPosCommand : public ICommand
	{
	public:
		/*!************************************************************************
		\brief Construct a new Render Position Command object
		**************************************************************************/
		RenderPosCommand(CPRender* render, Vec3<float> const& newPos) : m_render{ render }, m_newPos{ newPos } 
		{
			m_oldPos.x = render->w;
			m_oldPos.y = render->h;
			m_oldPos.z = render->z_val;
		}

		/*!************************************************************************
		\brief Executes the change render position command
		**************************************************************************/
		void Execute() override
		{
			m_render->w = m_newPos.x;
			m_render->h = m_newPos.y;
			m_render->z_val = m_newPos.z;
		}

		/*!************************************************************************
		\brief Undos the change render position command
		**************************************************************************/
		void Undo() override
		{
			m_render->w = m_oldPos.x;
			m_render->h = m_oldPos.y;
			m_render->z_val = m_oldPos.z;
		}

		/*!************************************************************************
		\brief Merges commands together, updates the final position
		**************************************************************************/
		bool Merge(std::shared_ptr<ICommand> incomingCommand) override
		{
			std::shared_ptr<RenderPosCommand> incomingRenderPosCommand = std::dynamic_pointer_cast<RenderPosCommand>(incomingCommand);
			if (incomingRenderPosCommand->m_render == m_render)
			{
				m_newPos = incomingRenderPosCommand->m_newPos;
				return true;
			}
			return false;
		}

		/*!************************************************************************
		\brief Get the command type, used for merging
		**************************************************************************/
		CommandType GetType() override
		{
			return CommandType::RENDERPOS;
		}

		/*!************************************************************************
		\brief What should be done when the command is removed from the manager
		**************************************************************************/
		void OnRemove() override { }

	private:
		CPRender* m_render;
		Vec3<float> m_oldPos, m_newPos;
	};

	class RenderImageCommand : public ICommand
	{
	public:
		/*!************************************************************************
		\brief Construct a new Render Image Command object
		**************************************************************************/
		RenderImageCommand(CPRender* render, int newImage) : m_render{ render }, m_oldImage{ render->texture }, m_newImage{ newImage } { }

		/*!************************************************************************
		\brief Executes the Render Image command
		**************************************************************************/
		void Execute() override
		{
			m_render->texture = m_newImage;
			m_render->UpdateTexture(m_newImage, static_cast<int>(m_render->w), static_cast<int>(m_render->h));
		}

		/*!************************************************************************
		\brief Undos the Render Image command
		**************************************************************************/
		void Undo() override
		{
			m_render->texture = m_oldImage;
			m_render->UpdateTexture(m_oldImage, static_cast<int>(m_render->w), static_cast<int>(m_render->h));
		}

		/*!************************************************************************
		\brief Merges commands together, but this command cannot merge
		**************************************************************************/
		bool Merge(std::shared_ptr<ICommand> incomingCommand) override
		{
			UNREFERENCED_PARAMETER(incomingCommand);
			return false;
		}

		/*!************************************************************************
		\brief Get the command type, used for merging
		**************************************************************************/
		CommandType GetType() override
		{
			return CommandType::RENDERIMAGE;
		}

		/*!************************************************************************
		\brief What should be done when the command is removed from the manager
		**************************************************************************/
		void OnRemove() override { }

	private:
		CPRender* m_render;
		int m_oldImage, m_newImage;
	};

	class RenderSpriteSheetCommand : public ICommand
	{
	public:
		/*!************************************************************************
		\brief Construct a new Render SpriteSheet Command object
		**************************************************************************/
		RenderSpriteSheetCommand(CPRender* render, std::string const& newSpriteSheet) : m_render{ render }, m_oldSpriteSheet{ render->spriteSheetName }, m_newSpriteSheet{ newSpriteSheet } { }

		/*!************************************************************************
		\brief Executes the Render SpriteSheet command
		**************************************************************************/
		void Execute() override
		{
			m_render->spriteSheetName = m_newSpriteSheet;
			if (m_render->spriteIndex >= 0)
				m_render->SetSpriteTexture(m_newSpriteSheet, m_render->spriteIndex);
			else
				m_render->UpdateTexture(-1, static_cast<int>(m_render->w), static_cast<int>(m_render->h));
		}

		/*!************************************************************************
		\brief Undos the Render SpriteSheet command
		**************************************************************************/
		void Undo() override
		{
			m_render->spriteSheetName = m_oldSpriteSheet;
			if (m_render->spriteIndex >= 0)
				m_render->SetSpriteTexture(m_oldSpriteSheet, m_render->spriteIndex);
			else
				m_render->UpdateTexture(-1, static_cast<int>(m_render->w), static_cast<int>(m_render->h));
		}

		/*!************************************************************************
		\brief Merges commands together, but this command cannot merge
		**************************************************************************/
		bool Merge(std::shared_ptr<ICommand> incomingCommand) override
		{
			UNREFERENCED_PARAMETER(incomingCommand);
			return false;
		}

		/*!************************************************************************
		\brief Get the command type, used for merging
		**************************************************************************/
		CommandType GetType() override
		{
			return CommandType::RENDERSSHEET;
		}

		/*!************************************************************************
		\brief What should be done when the command is removed from the manager
		**************************************************************************/
		void OnRemove() override { }

	private:
		CPRender* m_render;
		std::string m_oldSpriteSheet, m_newSpriteSheet;
	};

	class RenderSpriteSheetLayerCommand : public ICommand
	{
	public:
		/*!************************************************************************
		\brief Construct a new Render SpriteSheet layer Command object
		**************************************************************************/
		RenderSpriteSheetLayerCommand(CPRender* render, int newIndex) : m_render{ render }, m_oldIndex{ render->spriteIndex }, m_newIndex{ newIndex } { }

		/*!************************************************************************
		\brief Executes the Render SpriteSheet layer command
		**************************************************************************/
		void Execute() override
		{
			m_render->spriteIndex = m_newIndex;
			if (m_render->spriteIndex >= 0)
				m_render->SetSpriteTexture(m_render->spriteSheetName, m_newIndex);
			else
				m_render->UpdateTexture(-1, static_cast<int>(m_render->w), static_cast<int>(m_render->h));
		}

		/*!************************************************************************
		\brief Undos the Render SpriteSheet layer command
		**************************************************************************/
		void Undo() override
		{
			m_render->spriteIndex = m_oldIndex;
			if (m_render->spriteIndex >= 0)
				m_render->SetSpriteTexture(m_render->spriteSheetName, m_oldIndex);
			else
				m_render->UpdateTexture(-1, static_cast<int>(m_render->w), static_cast<int>(m_render->h));
		}

		/*!************************************************************************
		\brief Merges commands together, but this command cannot merge
		**************************************************************************/
		bool Merge(std::shared_ptr<ICommand> incomingCommand) override
		{
			UNREFERENCED_PARAMETER(incomingCommand);
			return false;
		}

		/*!************************************************************************
		\brief Get the command type, used for merging
		**************************************************************************/
		CommandType GetType() override
		{
			return CommandType::RENDERSSHEETLAYER;
		}

		/*!************************************************************************
		\brief What should be done when the command is removed from the manager
		**************************************************************************/
		void OnRemove() override { }

	private:
		CPRender* m_render;
		int m_oldIndex, m_newIndex;
	};
}
