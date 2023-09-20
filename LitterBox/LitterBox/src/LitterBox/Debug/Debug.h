/*!************************************************************************
 \file   Debug.h
 \author Ang Jiawei Jarrett
 \par    DP email: a.jiaweijarrett@digipen.edu
 \par    Course: CSD2401A
 \date   18-09-2023
 \brief


 The functions in the Debug class include:

**************************************************************************/

#pragma once

#include "LitterBox/Utils/Math.h"

namespace LB
{
	class Debugger
	{
		public:
		void SetColor(Vec4<int> color);

		void DrawLine(Vec2<int> start, Vec2<int> end);
		void DrawLine(Vec2<int> start, Vec2<int> end, Vec4<int> color);

		//void DrawCircle(Vec2<int> center, float radius);
		//void DrawCircle(Vec2<int> center, float radius, Vec4<int> color);

		void DrawBox(Vec2<int> center, float length);
		void DrawBox(Vec2<int> center, float length, Vec4<int> color);

		private:
		Vec2<int> writePos;
		Vec4<int> drawColor;
	};
}
