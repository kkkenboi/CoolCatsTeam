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

#pragma once

#include "LitterBox/Core/System.h"
#include "LitterBox/Utils/Math.h"
#include "LitterBox/Engine/Events.h"
#include "LitterBox/Renderer/Camera.h"
#include <stack>

namespace LB
{
	// Keycode forward declaration
	enum class KeyCode;

	// Macro to remove full file path, leaving only the name of the file
	#define DebuggerFileName (std::string(__FILE__).substr(std::string(__FILE__).rfind("\\") + 1))

	// Macros for Debugging, calls the respective functions with __FILE__ and __LINE__
	#define DebuggerLog(message) LB::DEBUG->Log(DebuggerFileName.c_str(), __LINE__, message)
	#define DebuggerLogFormat(format, ...) LB::DEBUG->LogFormat(DebuggerFileName.c_str(), __LINE__, format, __VA_ARGS__)

	#define DebuggerLogWarning(message) LB::DEBUG->LogWarning(DebuggerFileName.c_str(), __LINE__, message)
	#define DebuggerLogWarningFormat(format, ...) LB::DEBUG->LogWarningFormat(DebuggerFileName.c_str(), __LINE__, format, __VA_ARGS__)

	#define DebuggerLogError(message) LB::DEBUG->LogError(DebuggerFileName.c_str(), __LINE__, message)
	#define DebuggerLogErrorFormat(format, ...) LB::DEBUG->LogErrorFormat(DebuggerFileName.c_str(), __LINE__, format, __VA_ARGS__)

	#define DebuggerAssert(expectedCondition, message) LB::DEBUG->Assert(DebuggerFileName.c_str(), __LINE__, expectedCondition, message)
	#define DebuggerAssertFormat(expectedCondition, format, ...) LB::DEBUG->AssertFormat(DebuggerFileName.c_str(), __LINE__, expectedCondition, format, __VA_ARGS__)

	/*!***********************************************************************
	\brief
	 Determine which type of debug object to draw.
	*************************************************************************/
	enum class Debug_Types {
		CIRCLE,
		LINE,
		BOX
	};

	/*!***********************************************************************
	\brief
	 This struct holds all the relevant information required for rendering
	 and shape of object on the screen. As long as the shape is either a line,
	 a box or a circle. So maybe not any shape but most of them.
	*************************************************************************/
	struct Debug_Object {
		float radius, width, height;
		Debug_Types type;
		Vec2<float>	center;
		Vec2<float>	end;
		Vec4<float>	color;
	};

	/*!***********************************************************************
	\brief
	 Debugger class is an Engine system used when you want to print something to
	 console or start drawing debugging info for collision. Yes it's really only 
	 used for collision what else could it be for.
	*************************************************************************/
	class Debugger : public ISystem
	{
	public:
		/*!***********************************************************************
		\brief
		 Debugger class constructor
		*************************************************************************/
		Debugger();

		/*!***********************************************************************
		\brief
		 Debugger class Initializer that loads all the VAO, VBO and IBO necessary
		 to start drawing things on the screen.
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		\brief
		 The system's update function which sends compiles the data and sends
		 the information to the GPU to get drawn
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		\brief
		 Sets the m_drawColor variable of the class
		*************************************************************************/
		void SetColor(Vec4<float> color);

		/*!***********************************************************************
		\brief
		 Function loads a line for the update to draw. The line is defined by a
		 start and an end point.
		*************************************************************************/
		void DrawLine(Vec2<float> start, Vec2<float> end, Vec4<float> color);

		/*!***********************************************************************
		\brief
		 Function pushes a line for update to draw. This line is defined by a
		 start point, a direction and a magnitude.

		 NOTE: direction is not guarenteed to be normalized. If you want it to be
		 normalized then you need to do it yourself.
		*************************************************************************/
		void DrawLine(Vec2<float> start, Vec2<float> direction, float magnitude, Vec4<float> color);

		/*!***********************************************************************
		\brief
		 Deprecated DrawBox function.
		*************************************************************************/
		void DrawBox(Vec2<float> center, float length, Vec4<float> color);

		/*!***********************************************************************
		\brief
		 DrawBox function loads 4 lines to be drawn to represent a box with a
		 rotation defined in radians turning counter clockwise.
		*************************************************************************/
		void DrawBox(Vec2<float> center, float width, float height, Vec4<float> color, float rot);

		/*!***********************************************************************
		\brief
		 Deprecated DrawBox function.
		*************************************************************************/
		void DrawBox(Vec2<float> center, float length);

		/*!***********************************************************************
		\brief
		 DrawCircle draws lines in a way that represents a circle at center with
		 a radius of radius.
		*************************************************************************/
		void DrawCircle(Vec2<float> center, float radius, Vec4<float> color);

		/*!***********************************************************************
		\brief
		 Assert prints out a debug line if a specific condition is not met.
		*************************************************************************/
		void Assert(const char* file, int line, bool expectedCondition, std::string const& message);

		void AssertFormat(const char* file, int line, bool expectedCondition, const char* format, ...);

		/*!***********************************************************************
		\brief
		 Log prints a given message and the file that called it and from which line.
		*************************************************************************/
		void Log(const char* file, int line, std::string const& message);

		void LogFormat(const char* file, int line, const char* format, ...);

		/*!***********************************************************************
		\brief
		 LogWarning does the same thing as log but this time prints an additional
		 word WARNING! so you know it's serious.
		*************************************************************************/
		void LogWarning(const char* file, int line, std::string const& message);

		void LogWarningFormat(const char* fil, int line, const char* format, ...);

		/*!***********************************************************************
		\brief
		 LogError does the same thing as LogWarning but this time prints an additional
		 word ERROR!! so you know it's even more serious.
		*************************************************************************/
		void LogError(const char* file, int line, std::string const& message);

		void LogErrorFormat(const char* file, int line, const char* format, ...);

		/*!***********************************************************************
		\brief
		 Returns true if debug mode is on
		*************************************************************************/
		bool IsDebugOn();

		/*!***********************************************************************
		\brief
		 Flips the state of debug mode on/off
		*************************************************************************/
		void ToggleDebugMode();

		/*!***********************************************************************
		\brief
		 If game is currently paused, runs the simulation for 1 frame then pauses
		*************************************************************************/
		void StepPhysics();

	private:
		//-----------------Member variables-----------------
		bool m_debugModeOn { false };
		KeyCode m_debugToggleKey, m_stepPhysicsKey;
		double m_stepped { false };

		Vec4<float> m_drawColor;

		//----------Box & Lines rendering information------------
		unsigned int vao;
		unsigned int vbo;
		unsigned int ibo;
		unsigned int shader;
		Renderer::Camera cam;

		std::stack<Debug_Object> drawobj;
		std::vector<unsigned short> idx;

		/*!***********************************************************************
		\brief
		 Sends the data of a debug object to the GPU to be drawn.
		*************************************************************************/
		void line_update(Debug_Object& obj, const size_t& index);
	};

	/*!***********************************************************************
	\brief
	 Sets up the file loggers (Debug & Crash)
	*************************************************************************/
	void InitializeLoggers();

	/*!***********************************************************************
	\brief
	 Prints the information stored in the debug log logger
	*************************************************************************/
	void FlushDebugLog();

	/*!***********************************************************************
	\brief
	 Prints the information stored in the crash log & all loggers
	*************************************************************************/
	void FlushCrashLog(int signal);

	/*!***********************************************************************
	\brief
	 Sets the debug mode true (Used for event subscription)
	*************************************************************************/
	void ToggleDebugOn();

	/*!***********************************************************************
	\brief
	 Steps the physics by 1 frame (Used for event subscription)
	*************************************************************************/
	void StepPhysics();

	// A pointer to the system object in the core engine made to be singleton
	extern Debugger* DEBUG;
}
