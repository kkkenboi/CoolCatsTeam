/*!************************************************************************
 \file				EditorConsole.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This header file contains functions declarations for the console layer of the
 Editor. This is to show the logs and any errors that may arise during the runtime
 of the Editor.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Platform/Editor/Editor.h"
#include "Litterbox/Engine/Layer.h"

namespace LB
{
	/*!***********************************************************************
	  \brief
	  Types of messages
	*************************************************************************/
	enum class EditorConsoleMsgType
	{
		DEBUG,
		WARNING,
		ERROR
	};

	/*!***********************************************************************
	  \brief
	  Contains a message and its type (warning, deug, error)
	*************************************************************************/
	struct ConsoleMessage
	{
		std::string msg;
		EditorConsoleMsgType type;
	};

	class EditorConsole : public Layer
	{
	public:
		/*!***********************************************************************
		  \brief
		  Constructor for the EditorConsole class.
		*************************************************************************/
		EditorConsole(std::string layerName);

		/*!***********************************************************************
		  \brief
		  Updates the EditorConsole layer, renders the various messages
		*************************************************************************/
		void UpdateLayer() override;

		/*!***********************************************************************
		  \brief
		  Destroys the EditorConsole layer.
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		  \brief
		  Add a log message to the console.
		*************************************************************************/
		void AddLogMessage(std::string const& log);

		/*!***********************************************************************
		  \brief
		  Add a warning message to the console.
		*************************************************************************/
		void AddWarningMessage(std::string const& warning);

		/*!***********************************************************************
		  \brief
		  Add an error message to the console.
		*************************************************************************/
		void AddErrorMessage(std::string const& error);

	private:
		std::map<EditorConsoleMsgType, ImVec4> m_messageColors; // Colors for each message type
		std::vector<ConsoleMessage> m_messages{};				// All the messages logged
		ImGuiTextFilter m_messageFilter;						// Filter for all log messages

		bool m_pauseOnError;									// Should pause the game when error is logged?
	};

	extern EditorConsole* EDITORCONSOLE;
}
