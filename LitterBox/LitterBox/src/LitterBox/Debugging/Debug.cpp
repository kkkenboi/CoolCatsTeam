/*!************************************************************************
 \file			Debug.cpp
 \author		Ang Jiawei Jarrett
 \par DP email: a.jiaweijarrett@digipen.edu
 \par Course:	CSD2401A
 \date			18-09-2023
 \brief

**************************************************************************/

#include "Debug.h"
#include "LitterBox/Renderer/Renderer.h"
#include <iostream>

namespace LB 
{
	Debugger* DEBUG = nullptr;

	Debugger::Debugger()
	{
		if (!DEBUG)
			DEBUG = this;
		else
			std::cerr << "Debug System already exist" << std::endl;
	}


	void Debugger::SetColor(Vec4<int> color)
	{
		m_drawColor = color;
	}

	void Debugger::DrawLine(Vec2<int> start, Vec2<int> end, Vec4<int> color)
	{
		UNREFERENCED_PARAMETER(start);
		UNREFERENCED_PARAMETER(end);
		UNREFERENCED_PARAMETER(color);
	}

	void Debugger::DrawLine(Vec2<int> start, Vec2<int> end)
	{
		DrawLine(start, end, m_drawColor);
	}

	void Debugger::DrawBox(Vec2<int> center, float length, Vec4<int> color)
	{
		UNREFERENCED_PARAMETER(center);
		UNREFERENCED_PARAMETER(length);
		UNREFERENCED_PARAMETER(color);
	}

	void Debugger::DrawBox(Vec2<int> center, float length)
	{
		DrawBox(center, length, m_drawColor);
	}

	void Debugger::Log(std::string const& message, const char* file, int line)
	{
		fprintf(stdout, "[DEBUGGER LOG] [%s:%d] %s\n", file, line, message.c_str());
	}

	void Debugger::LogWarning(std::string const& message, const char* file, int line)
	{
		fprintf(stdout, "[DEBUGGER WARNING!] [%s:%d] %s\n", file, line, message.c_str());
	}

	void Debugger::LogError(std::string const& message, const char* file, int line)
	{
		fprintf(stderr, "[DEBUGGER ERROR!!] [%s:%d] %s\n", file, line, message.c_str());

	}

	void Debugger::Assert(bool expectedCondition, std::string const& message, const char* file, int line)
	{
		if (!expectedCondition)
		{
			fprintf(stderr, "[DEBUGGER ASSERT!!] [%s:%d] %s\n", file, line, message.c_str());
		}
	}

}
