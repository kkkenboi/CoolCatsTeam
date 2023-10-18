/*!************************************************************************
 \file				Debug.cpp
 \author(s)			Ang Jiawei Jarrett, Ryan Tan Jian Hao
 \par DP email(s):	a.jiaweijarrett@digipen.edu, ryanjianhao.tan\@digipen.edu
 \par Course:		CSD2401A
 \date				18/09/23
 \brief

 This file contains functions to log messages and errors to the console and files,
 and draw boxes and lines (for physics).

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox/Renderer/Renderer.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Engine/Time.h"
#include "Debug.h"
#include "Platform/Editor/EditorConsole.h"		// For logging to ImGUI console
#include <sstream>

#include <csignal>								// For getting crash signals

#include <Windows.h>							// For back tracing on crash
#include <DbgHelp.h>							// Also for back tracing on crash
#pragma comment(lib, "Dbghelp.lib")

#include "spdlog/spdlog.h"						// For logging information to files
#include "spdlog/sinks/basic_file_sink.h"		// File sink
#include "spdlog/sinks/stdout_color_sinks.h"	// Console sink

//-----------------Pre-defines------------------------------
constexpr int CIRCLE_LINES{ 20 };
constexpr float INCREMENT{ 2.f * (float)PI / (float)CIRCLE_LINES };

float wid_div;		// { 1.f / (LB::WINDOWSSYSTEM->GetWidth() * 0.5f) };
float height_div;	// { 1.f / (LB::WINDOWSSYSTEM->GetHeight() * 0.5f) };
//-----------------Pre-defines------------------------------

namespace LB 
{
	//------------------File logger--------------------
	std::shared_ptr<spdlog::logger> debugInfoLogger;
	std::shared_ptr<spdlog::logger> crashInfoLogger;
	std::shared_ptr<spdlog::logger> consoleLogger;

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
			std::cerr << "Debug System already exist" << std::endl;

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
	 Vertex required for a line end point
	*************************************************************************/
	struct debug_vertex {
		Vec2<float> pos;
		Vec4<float> col;
	};

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

		// TO DO: serialize max depth

		//int traceDepth = 20;
		//void** stackTrace = new void* [traceDepth];

		//// WORD is basically unsigned short and 
		//WORD frames = CaptureStackBackTrace(0, traceDepth, stackTrace, nullptr);

		//SYMBOL_INFO symbol = { sizeof(SYMBOL_INFO), 255 };

		//for (WORD index{ 0 }; index < frames; ++index) {
		//	SymFromAddr(GetCurrentProcess(), (DWORD64)(stackTrace[index]), 0, &symbol);

		//	std::cout << "ADDRESS " << stackTrace[index] << " NAME " << symbol.Name << "\n";

		//	std::ostringstream message;
		//	message << "[" << index << "]" << symbol.Name;
		//	crashInfoLogger->error(message.str());
		//}

		//delete[] stackTrace;

		crashInfoLogger->error("Unexpected application crash! Signal: {}", signal);
		crashInfoLogger->flush();
	}

	//TODO modulate the vertex size
	//vertex size should = min 3000 x 4 (number of quads in render object)
	/*!***********************************************************************
	\brief
	 Debugger class Initializer that loads all the VAO, VBO and IBO necessary
	 to start drawing things on the screen.
	*************************************************************************/
	void Debugger::Initialize() {
		if (!DEBUG)
			DEBUG = this;
		else
			DebuggerLogError("Debug System already exist!");

		SetSystemName("Debug System");

		//--------------------Debugging input registering---------------------
		INPUT->SubscribeToKey(ToggleDebugOn, m_debugToggleKey, KeyEvent::TRIGGERED, KeyTriggerType::NONPAUSABLE);
		INPUT->SubscribeToKey(LB::StepPhysics, m_stepPhysicsKey, KeyEvent::TRIGGERED, KeyTriggerType::NONPAUSABLE);

		//--------------------Drawing Setup---------------------
		wid_div = { 1.f / (LB::WINDOWSSYSTEM->GetWidth() * 0.5f) };
		height_div = { 1.f / (LB::WINDOWSSYSTEM->GetHeight() * 0.5f) };

		//assume we have one index per vertex
		idx.resize(12000);

		glCreateBuffers(1, &vbo);
		glNamedBufferStorage(vbo,
			sizeof(debug_vertex) * 12000,
			nullptr,
			GL_DYNAMIC_STORAGE_BIT);

		//create vao
		glCreateVertexArrays(1, &vao);
		//attribute 0 will be position data
		glEnableVertexArrayAttrib(vao, 0);
		glVertexArrayVertexBuffer(vao, 1, vbo, 0, sizeof(debug_vertex));
		glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(vao, 0, 1);
		//color coordinates
		glEnableVertexArrayAttrib(vao, 2);
		glVertexArrayVertexBuffer(vao, 3, vbo, sizeof(Vec2<float>), sizeof(debug_vertex));
		glVertexArrayAttribFormat(vao, 2, 4, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(vao, 2, 3);

		glCreateBuffers(1, &ibo);
		glNamedBufferStorage(ibo, idx.capacity() * sizeof(unsigned short),
			nullptr, GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayElementBuffer(vao, ibo);

		shader_source shd_pgm{ shader_parser("Assets/Shaders/debug.shader") };
		shader = create_shader(shd_pgm.vtx_shd.c_str(), shd_pgm.frg_shd.c_str());

		glLineWidth(5.f);
	}

	//loop through stack
	//if line object then do matrix projection on both start and end points
	//send the points to the gpu
	//edit the index in the index vector
	//send the index to gpu
	/*!***********************************************************************
	\brief
	 Sends the data of a debug object to the GPU to be drawn.
	*************************************************************************/
	void Debugger::line_update(Debug_Object& obj, const size_t& index) {
		//-----------------Matrix projection of start point--------------
		
		glm::vec4 start_point{ obj.center.x, obj.center.y, 0.f, 1.f };
		start_point = cam.world_NDC * start_point;
		obj.center.x = start_point.x;
		obj.center.y = start_point.y;
		//-----------------Matrix projection of start point--------------

		//-----------------Matrix projection of end point--------------
		start_point = { obj.end.x, obj.end.y, 0.f, 1.f };
		start_point = cam.world_NDC * start_point;
		obj.end.x = start_point.x;
		obj.end.y = start_point.y;
		//-----------------Matrix projection of end point--------------

		//-----------------Send data to GPU--------------
		debug_vertex server_data[2]{};
		server_data[0].pos = obj.center;
		server_data[0].col = obj.color;
		server_data[1].pos = obj.end;
		server_data[1].col = obj.color;

		glNamedBufferSubData(vbo, index * sizeof(debug_vertex), sizeof(debug_vertex) * 2U, server_data);
		idx.at(index) = (unsigned short)index;
		idx.at(index + 1) = (unsigned short)(index + 1);
		//-----------------Send data to GPU--------------
	}

	//TODO can remove the checking of debug type and just draw lines
	/*!***********************************************************************
	\brief
	 The system's update function which sends compiles the data and sends
	 the information to the GPU to get drawn
	*************************************************************************/
	void Debugger::Update() {
		// If stepped, pause again since it was paused before
		if (m_stepped)
		{
			m_stepped = false;
			TIME->Pause(true);
		}

		//-----------------Debug Rendering------------------
		// Don't do any rendering if debug mode is off
		if (!IsDebugOn()) return;

		size_t index{ 0 };
		while (drawobj.size()) {
			//loop through object to 
			line_update(drawobj.top(), index);
			index += 2;
			drawobj.pop();
		}

		//pass index data inside
		glNamedBufferSubData(ibo, 0, index * sizeof(unsigned short), idx.data());
		glUseProgram(shader);
		glBindVertexArray(vao);
		glDrawElements(GL_LINES, (GLsizei)index, GL_UNSIGNED_SHORT, nullptr);
	}

	/*!***********************************************************************
	\brief
	 Sets the m_drawColor variable of the class
	*************************************************************************/
	void Debugger::SetColor(Vec4<float> color)
	{
		m_drawColor = color;
	}

	/*!***********************************************************************
	\brief
	 Function loads a line for the update to draw. The line is defined by a
	 start and an end point.
	*************************************************************************/
	void Debugger::DrawLine(Vec2<float> start, Vec2<float> end, Vec4<float> color)
	{
		drawobj.push(Debug_Object{ 0.f,0.f,0.f,
			Debug_Types::LINE,
			start,
			end,
			color });
	}

	/*!***********************************************************************
	\brief
	 Function pushes a line for update to draw. This line is defined by a
	 start point, a direction and a magnitude.

	 NOTE: direction is not guarenteed to be normalized. If you want it to be
	 normalized then you need to do it yourself.
	*************************************************************************/
	void Debugger::DrawLine(Vec2<float> start, Vec2<float> direction, float magnitude, Vec4<float> color)
	{
		direction.x = direction.x * magnitude;
		direction.y = direction.y * magnitude;

		direction.x = direction.x + start.x;
		direction.y = direction.y + start.y;

		DrawLine(start, direction, color);
	}
	/*!***********************************************************************
	\brief
	 Deprecated DrawBox function.
	*************************************************************************/
	void Debugger::DrawBox(Vec2<float> center, float length, Vec4<float> color)
	{
		UNREFERENCED_PARAMETER(center);
		UNREFERENCED_PARAMETER(length);
		UNREFERENCED_PARAMETER(color);
	}

	/*!***********************************************************************
	\brief
	 DrawBox function loads 4 lines to be drawn to represent a box with a
	 rotation defined in radians turning counter clockwise.
	*************************************************************************/
	void Debugger::DrawBox(Vec2<float> center, float width, float height, Vec4<float> color, float rot)
	{
		//rotation matrix
		glm::mat4 rotation{
			cosf(rot),sinf(rot),0.f,0.f,
			-sinf(rot),cosf(rot),0.f,0.f,
			0.f,0.f,1.f,0.f,
			0.f,0.f,0.f,1.f
		};
		//matrix to convert the world coord to NDC
		glm::mat4 translate{1.f};
		translate[3][0] = center.x;
		translate[3][1] = center.y;

		glm::mat4 scale{ 1.f };
		scale[0][0] = width;
		scale[1][1] = height;

		debug_vertex data[4]{}; //bot left, bot right, top right, top left
		//-------------Matrix Projection of four points-----------------
		glm::vec4 point[4]{
			{-0.5f, -0.5f, 0.f, 1.f},//bottom left
			{0.5f, -0.5f, 0.f, 1.f}, //bottom right
			{0.5f, 0.5f, 0.f, 1.f}, //top right
			{-0.5f, 0.5f, 0.f, 1.f} //top left
		};

		for (int i{ 0 }; i < 4; ++i) {
			glm::vec4 pos{ translate * rotation * scale * point[i] };
			data[i].pos = { pos.x, pos.y };
			data[i].col = color;
		}
		DrawLine(data[0].pos, data[1].pos, color);
		DrawLine(data[1].pos, data[2].pos, color);
		DrawLine(data[2].pos, data[3].pos, color);
		DrawLine(data[3].pos, data[0].pos, color);
	}

	/*!***********************************************************************
	\brief
	 Deprecated DrawBox function.
	*************************************************************************/
	void Debugger::DrawBox(Vec2<float> center, float length)
	{
		DrawBox(center, length, m_drawColor);
	}

	/*!***********************************************************************
	\brief
	 DrawCircle draws lines in a way that represents a circle at center with
	 a radius of radius.
	*************************************************************************/
	void Debugger::DrawCircle(Vec2<float> center, float radius, Vec4<float> color)
	{
		//one angle for before
		float bangle{ 0.f };
		//one angle for after
		float aangle{ bangle + INCREMENT };
		//set each circle to have 20 lines
		for (size_t i{ 0 }; i < CIRCLE_LINES; ++i) {
			DrawLine({ center.x + radius * cosf(bangle), center.y + radius * sinf(bangle) },
				{ center.x + radius * cosf(aangle), center.y + radius * sinf(aangle) },
				color);
			bangle = i + 1 * (float)INCREMENT;
			aangle = bangle + INCREMENT;
		}
	}

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

		// Print to console (DECAP)
		// consoleLogger->debug(output.str());

		// Print to ImGUI console
		EDITORCONSOLE->AddLogMessage(output.str());

		// Save to debug file and flush it
		debugInfoLogger->debug(output.str());	
		FlushDebugLog();
	}

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

		// Print to console (DECAP)
		//consoleLogger->warn(output.str());

		// Print to ImGUI console
		EDITORCONSOLE->AddWarningMessage(output.str());

		// Save to debug file and flush it
		debugInfoLogger->warn(output.str());
		FlushDebugLog();
	}

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

		// Print to console (DECAP)
		//consoleLogger->error(output.str());

		// Print to ImGUI console
		EDITORCONSOLE->AddErrorMessage(output.str());

		// Save to debug file and flush it
		debugInfoLogger->error(output.str());
		FlushDebugLog();
	}

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
			output << "[" << file << ":" << line << "] " << message;

			// Print to console
			consoleLogger->error(output.str());

			// Save to debug file and flush it
			debugInfoLogger->error(output.str());
			FlushDebugLog();
			
			std::terminate();
		}
	}

	void Debugger::AssertFormat(const char* file, int line, bool expectedCondition, const char* format, ...)
	{
		va_list args;
		va_start(args, format);

		char cStrBuffer[512];
		vsnprintf(cStrBuffer, 512, format, args);

		va_end(args);

		Assert(file, line, expectedCondition, std::string(cStrBuffer));
	}
}
