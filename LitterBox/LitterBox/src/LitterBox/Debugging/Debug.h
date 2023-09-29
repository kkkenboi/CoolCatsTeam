/*!************************************************************************
 \file				Debug.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				18-09-2023
 \brief
 The functions in the Debug class include:

**************************************************************************/

#pragma once

#include "LitterBox/Core/System.h"
#include "LitterBox/Utils/Math.h"
#include "LitterBox/Engine/Events.h"
#include "LitterBox/Renderer/Camera.h"
#include <stack>


namespace LB
{
	#define DebuggerFileName (std::string(__FILE__).substr(std::string(__FILE__).rfind("\\") + 1))

	#define DebuggerAssert(expectedCondition, message) LB::DEBUG->Assert(expectedCondition, message, DebuggerFileName.c_str(), __LINE__)
	#define DebuggerLog(message) LB::DEBUG->Log(message, DebuggerFileName.c_str(), __LINE__)
	#define DebuggerLogWarning(message) LB::DEBUG->LogWarning(message, DebuggerFileName.c_str(), __LINE__)
	#define DebuggerLogError(message) LB::DEBUG->LogError(message, DebuggerFileName.c_str(), __LINE__)

	enum class Debug_Types {
		CIRCLE,
		LINE,
		BOX
	};

	struct Debug_Object {
		float radius, width, height;
		Debug_Types type;
		Vec2<float>	center;
		Vec2<float>	end;
		Vec4<float>	color;
	};

	class Debugger : public ISystem
	{
	public:

		void Initialize() override;
		void Update() override;

		void SetColor(Vec4<float> color);

		void DrawLine(Vec2<float> start, Vec2<float> end, Vec4<float> color);
		void DrawLine(Vec2<float> start, Vec2<float> direction, float magnitude, Vec4<float> color);

		void DrawBox(Vec2<float> center, float length, Vec4<float> color);
		void DrawBox(Vec2<float> center, float width, float height, Vec4<float> color, float rot);
		void DrawBox(Vec2<float> center, float length);
		void DrawCircle(Vec2<float> center, float radius, Vec4<float> color);

		void Assert(bool expectedCondition, std::string const& message, const char* file = "Unnamed", int line = 0);
		void Log(std::string const& message, const char* file = "Unnamed", int line = 0);
		void LogWarning(std::string const& message, const char* file = "Unnamed", int line = 0);
		void LogError(std::string const& message, const char* file = "Unnamed", int line = 0);

		private:
		Vec2<float> m_writePos;
		Vec4<float> m_drawColor;

		unsigned int vao;
		unsigned int vbo;
		unsigned int ibo;
		unsigned int shader;
		Renderer::Camera cam;

		std::stack<Debug_Object> drawobj;
		std::vector<unsigned short> idx;

		void line_update(Debug_Object& obj, const size_t& index);
	};

	extern Debugger* DEBUG;
}
