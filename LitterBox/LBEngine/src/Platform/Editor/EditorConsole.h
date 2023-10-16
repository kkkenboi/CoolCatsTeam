/*!************************************************************************
 \file				EditorConsole.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This header file

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
		EditorConsole(std::string layerName);

		void UpdateLayer() override;

		void AddLogMessage(std::string const& log);
		void AddWarningMessage(std::string const& warning);
		void AddErrorMessage(std::string const& error);

	private:
		std::map<EditorConsoleMsgType, ImVec4> m_messageColors;
		std::vector<ConsoleMessage> m_messages{};
		ImGuiTextFilter m_messageFilter;

		bool m_pauseOnError;
	};

	extern EditorConsole* EDITORCONSOLE;
}
