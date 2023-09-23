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
	class Debug : public ISystem
	{
	public:
		Event<bool> onDebugToggle;

		void SetColor(Vec4<int> color);

		void DrawLine(Vec2<int> start, Vec2<int> end, Vec4<int> color);
		void DrawLine(Vec2<int> start, Vec2<int> end);

		//void DrawCircle(Vec2<int> center, float radius);
		//void DrawCircle(Vec2<int> center, float radius, Vec4<int> color);

		void DrawBox(Vec2<int> center, float length, Vec4<int> color);
		void DrawBox(Vec2<int> center, float length);

		private:
		Vec2<int> m_writePos;
		Vec4<int> m_drawColor;
	};
}
