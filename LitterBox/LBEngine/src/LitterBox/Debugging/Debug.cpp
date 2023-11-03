/*!************************************************************************
 \file				Debug.cpp
 \author(s)			Ang Jiawei Jarrett, Ryan Tan Jian Hao
 \par DP email(s):	a.jiaweijarrett@digipen.edu, ryanjianhao.tan\@digipen.edu
 \par Course:		CSD2401A
 \date				18/09/23
 \brief

 This file contains functions to log messages and errors to the console and files.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox/Engine/Input.h"
#include "LitterBox/Engine/Time.h"
#include "Debug.h"

#include "Platform/Editor/EditorConsole.h"		// For logging to ImGUI console
#include <sstream>								// For formatting the message string

#include <csignal>								// For getting crash signals
#include "spdlog/spdlog.h"						// For logging information to files
#include "spdlog/sinks/basic_file_sink.h"		// File sink
#include "spdlog/sinks/stdout_color_sinks.h"	// Console sink

namespace LB 
{
	//------------------File logger--------------------
	std::shared_ptr<spdlog::logger> debugInfoLogger;
	std::shared_ptr<spdlog::logger> crashInfoLogger;
	std::shared_ptr<spdlog::logger> consoleLogger;

	//-------------------Singleton---------------------
	Debugger* DEBUG = nullptr;

	/*!***********************************************************************
	\brief
	 Debugger class constructor
	*************************************************************************/
	Debugger::Debugger() : ibo{}, shader{}, vao{}, vbo{}
	{
		if (!DEBUG)
			DEBUG = this;
		else
			DebuggerLogError("Debug System already exist");

		// Toggle debug mode on key press
		m_debugToggleKey = KeyCode::KEY_J;
		m_stepPhysicsKey = KeyCode::KEY_I;

		//--------------------Crash signal Setup---------------------
		// Dump the crash log on...
		signal(SIGSEGV, FlushCrashLog); // Segmentation fault
		signal(SIGABRT, FlushCrashLog); // Application terminate / abort
	}

	/*!***********************************************************************
	 \brief
	 Initializes the drawing settings
	*************************************************************************/
	void Debugger::Initialize() {
		SetSystemName("Debug System");

		InitializeDrawer();
	}

	/*!***********************************************************************
	\brief
	 Decides whether the debug information should draw depending on whether
	 debug mode is on
	*************************************************************************/
	void Debugger::Update() {
		// If stepped, pause again since it was paused before
		if (m_stepped)
		{
			m_stepped = false;
			TIME->Pause(true);
		}

		// Don't do any rendering if debug mode is off
		if (!IsDebugOn()) return;

		UpdateDrawer();
	}

	/*!***********************************************************************
	\brief
	 Sets up the file loggers (Debug & Crash)
	*************************************************************************/
	void InitializeLoggers()
	{
		//--------------------Loggers Setup---------------------
		debugInfoLogger = spdlog::basic_logger_mt("DEBUG LOGGER", "Logs/DebugLog.txt");
		debugInfoLogger->set_pattern("[%L] %v");
		debugInfoLogger->set_level(spdlog::level::debug);

		crashInfoLogger = spdlog::basic_logger_mt("CRASH LOGGER", "Logs/CrashLog.txt");
		crashInfoLogger->set_pattern("%v");
		crashInfoLogger->set_level(spdlog::level::err);

		consoleLogger = spdlog::stdout_color_mt("CONSOLE LOGGER");
		consoleLogger->set_pattern("%^[%L] %v%$");
		consoleLogger->set_level(spdlog::level::debug);

		// Clear the debug log for logging
		std::ofstream logFile("Logs/DebugLog.txt", std::ios::trunc);
		logFile.close();
	}

	/*!***********************************************************************
	\brief
	 Prints the information stored in the debug log logger
	*************************************************************************/
	void FlushDebugLog()
	{
		// Log all debug info on exit
		debugInfoLogger->flush();
	}

	/*!***********************************************************************
	\brief
	 Prints the information stored in the crash log & all loggers
	*************************************************************************/
	void FlushCrashLog(int signal)
	{
		// Flush the debug log as well
		FlushDebugLog();

		// Clear the old crash log
		std::ofstream logFile("Logs/CrashLog.txt", std::ios::trunc);
		logFile.close();

		crashInfoLogger->error("[{}] Unexpected application crash! Signal: {}", DEBUG->GetCurrentTimeStamp(), signal);
		crashInfoLogger->flush();
	}

	/*!***********************************************************************
	 \brief
	 Returns the current time (when this function is called) in the format
	 HH:MM:SS
	*************************************************************************/
	std::string Debugger::GetCurrentTimeStamp()
	{
		std::time_t now = std::time(nullptr);
		std::tm timeStamp = *std::localtime(&now);

		std::ostringstream timeStampString;
		timeStampString << std::put_time(&timeStamp, "%H:%M:%S");
		return timeStampString.str();
	}

	/*!***********************************************************************
	 \brief
	 Log prints a given message and the file that called it and from which line.
	*************************************************************************/
	void Debugger::Log(const char* file, int line, std::string const& message)
	{
		std::ostringstream output;
		output << "[" << GetCurrentTimeStamp() << "] " << "[" << file << ":" << line << "] " << message;

		// Print to ImGUI console
		if (EDITORCONSOLE)
			EDITORCONSOLE->AddLogMessage(output.str());

		// Save to debug file and flush it
		debugInfoLogger->debug(output.str());	
		FlushDebugLog();
	}

	// Overload of the previous log function
	void Debugger::LogFormat(const char* file, int line, const char* format, ...)
	{
		va_list args;
		va_start(args, format);

		char cStrBuffer[512];
		vsnprintf(cStrBuffer, 512, format, args);

		va_end(args);

		Log(file, line, std::string(cStrBuffer));
	}

	/*!***********************************************************************
	 \brief
	 LogWarning does the same thing as log but this time prints an additional
	 word WARNING! so you know it's serious.
	*************************************************************************/
	void Debugger::LogWarning(const char* file, int line, std::string const& message)
	{
		std::ostringstream output;
		output << "[" << GetCurrentTimeStamp() << "] " << "[" << file << ":" << line << "] " << message;

		// Print to ImGUI console
		if (EDITORCONSOLE)
			EDITORCONSOLE->AddWarningMessage(output.str());

		// Save to debug file and flush it
		debugInfoLogger->warn(output.str());
		FlushDebugLog();
	}

	// Overload of the previous log function
	void Debugger::LogWarningFormat(const char* file, int line, const char* format, ...)
	{
		va_list args;
		va_start(args, format);

		char cStrBuffer[512];
		vsnprintf(cStrBuffer, 512, format, args);

		va_end(args);

		LogWarning(file, line, std::string(cStrBuffer));
	}

	/*!***********************************************************************
	 \brief
	 LogError does the same thing as LogWarning but this time prints an additional
	 word ERROR!! so you know it's even more serious.
	*************************************************************************/
	void Debugger::LogError(const char* file, int line, std::string const& message)
	{
		std::ostringstream output;
		output << "[" << GetCurrentTimeStamp() << "] " << "[" << file << ":" << line << "] " << message;

		// Print errors to console
		consoleLogger->error(output.str());

		// Print to ImGUI console
		if (EDITORCONSOLE)
			EDITORCONSOLE->AddErrorMessage(output.str());

		// Save to debug file and flush it
		debugInfoLogger->error(output.str());
		crashInfoLogger->error(output.str());
		FlushDebugLog();
	}

	// Overload of the previous log function
	void Debugger::LogErrorFormat(const char* file, int line, const char* format, ...)
	{
		va_list args;
		va_start(args, format);

		char cStrBuffer[512];
		vsnprintf(cStrBuffer, 512, format, args);

		va_end(args);

		LogError(file, line, std::string(cStrBuffer));

		// TODO: Add an option to pause on error
	}

	/*!***********************************************************************
	 \brief
	 Assert prints out a debug line and stops the program if a specific condition is not met.
	*************************************************************************/
	void Debugger::Assert(const char* file, int line, bool expectedCondition, std::string const& message)
	{
		if (!expectedCondition)
		{
			std::ostringstream output;
			output << "[" << GetCurrentTimeStamp() << "] " << "[" << file << ":" << line << "] " << message;

			// Print errors to console
			consoleLogger->error(output.str());

			// Save to debug file and flush it
			debugInfoLogger->error(output.str());
			crashInfoLogger->error(output.str());
			FlushDebugLog();
			
			std::terminate();
		}
	}

	// Overload of the previous log function
	void Debugger::AssertFormat(const char* file, int line, bool expectedCondition, const char* format, ...)
	{
		va_list args;
		va_start(args, format);

		char cStrBuffer[512];
		vsnprintf(cStrBuffer, 512, format, args);

		va_end(args);

		Assert(file, line, expectedCondition, std::string(cStrBuffer));
	}

	/*!***********************************************************************
	\brief
	 Returns true if debug mode is on
	*************************************************************************/
	bool Debugger::IsDebugOn()
	{
		return m_debugModeOn;
	}

	/*!***********************************************************************
	\brief
	 Flips the state of debug mode on/off
	*************************************************************************/
	void Debugger::ToggleDebugMode()
	{
		m_debugModeOn = !m_debugModeOn;
	}

	// TODO: Refactor so that it uses a different pause
	/*!***********************************************************************
	\brief
	 If game is currently paused, runs the simulation for 1 frame then pauses
	*************************************************************************/
	void Debugger::StepPhysics()
	{
		if (!TIME->IsPaused()) return;
		m_stepped = true;
		TIME->Pause(false);
		TIME->StepFixedDeltaTime();
	}

	/*!***********************************************************************
	\brief
	Steps the physics by 1 frame (Used for event subscription)
	*************************************************************************/
	void StepPhysics()
	{
		DEBUG->StepPhysics();
	}

	/*!***********************************************************************
	\brief
	 Sets the debug mode true (Used for event subscription)
	*************************************************************************/
	void ToggleDebugOn()
	{
		DEBUG->ToggleDebugMode();
	}
}
