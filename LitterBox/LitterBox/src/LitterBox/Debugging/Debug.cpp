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
	Debug* DEBUG = nullptr;

	Debug::Debug()
	{
		if (!DEBUG)
			DEBUG = this;
		else
			std::cerr << "Debug System already exist" << std::endl;
	}


	void Debug::SetColor(Vec4<int> color) 
	{
		m_drawColor = color;
	}

	void Debug::DrawLine(Vec2<int> start, Vec2<int> end, Vec4<int> color)
	{

	}

	void Debug::DrawLine(Vec2<int> start, Vec2<int> end)
	{
		DrawLine(start, end, m_drawColor);
	}

	void Debug::DrawBox(Vec2<int> center, float length, Vec4<int> color)
	{

	}

	void Debug::DrawBox(Vec2<int> center, float length)
	{
		DrawBox(center, length, m_drawColor);
	}

	void Debug::LogFull(const char* file, int line, std::string const& message)
	{
		fprintf(stdout, "[LOG] [%s:%d] %s\n", file, line, message.c_str());
	}

}
