/*!************************************************************************
 \file			Debug.h
 \author		Ang Jiawei Jarrett
 \par DP email: a.jiaweijarrett@digipen.edu
 \par Course:	CSD2401A
 \date			18-09-2023
 \brief

 The functions in the Debug class include:

**************************************************************************/

#pragma once

#include "LitterBox/Core/System.h"
#include "LitterBox/Utils/Math.h"
#include "LitterBox/Engine/Events.h"

namespace LB
{
	#define DebuggerFileName (std::string(__FILE__).substr(std::string(__FILE__).rfind("\\") + 1))

	#define DebuggerAssert(expectedCondition, message) LB::DEBUG->Assert(expectedCondition, message, DebuggerFileName.c_str(), __LINE__)
	#define DebuggerLog(message) LB::DEBUG->Log(message, DebuggerFileName.c_str(), __LINE__)
	#define DebuggerLogWarning(message) LB::DEBUG->LogWarning(message, DebuggerFileName.c_str(), __LINE__)
	#define DebuggerLogError(message) LB::DEBUG->LogError(message, DebuggerFileName.c_str(), __LINE__)

	class Debugger : public ISystem
	{
	public:
		Debugger();

		void Initialize() override { SetSystemName("Debug System"); }

		void SetColor(Vec4<int> color);

		void DrawLine(Vec2<int> start, Vec2<int> end, Vec4<int> color);
		void DrawLine(Vec2<int> start, Vec2<int> end);

		void DrawBox(Vec2<int> center, float length, Vec4<int> color);
		void DrawBox(Vec2<int> center, float length);

		void Assert(bool expectedCondition, std::string const& message, const char* file = "Unnamed", int line = 0);
		void Log(std::string const& message, const char* file = "Unnamed", int line = 0);
		void LogWarning(std::string const& message, const char* file = "Unnamed", int line = 0);
		void LogError(std::string const& message, const char* file = "Unnamed", int line = 0);

		private:
		Vec2<int> m_writePos;
		Vec4<int> m_drawColor;
	};

	extern Debugger* DEBUG;
}
