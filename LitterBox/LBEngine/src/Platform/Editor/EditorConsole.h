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
	enum class EditorConsoleMsgType
	{
		DEBUG,
		WARNING,
		ERROR
	};

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
		  \return
		  Nothing.
		*************************************************************************/
		EditorConsole(std::string layerName);

		/*!***********************************************************************
		  \brief
		  Updates the EditorConsole layer.
		  \return
		  Nothing.
		*************************************************************************/
		void UpdateLayer() override;

		/*!***********************************************************************
		  \brief
		  Destroys the EditorConsole layer.
		  \return
		  Nothing.
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		  \brief
		  Add a log message to the console.
		  \return
		  Nothing.
		*************************************************************************/
		void AddLogMessage(std::string const& log);

		/*!***********************************************************************
		  \brief
		  Add a warning message to the console.
		  \return
		  Nothing.
		*************************************************************************/
		void AddWarningMessage(std::string const& warning);

		/*!***********************************************************************
		  \brief
		  Add an error message to the console.
		  \return
		  Nothing.
		*************************************************************************/
		void AddErrorMessage(std::string const& error);

	private:
		std::map<EditorConsoleMsgType, ImVec4> m_messageColors;
		std::vector<ConsoleMessage> m_messages{};
		ImGuiTextFilter m_messageFilter;

		bool m_pauseOnError;
	};

	extern EditorConsole* EDITORCONSOLE;
}
