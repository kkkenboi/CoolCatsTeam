/*!************************************************************************
 \file   Time.h
 \author Ang Jiawei Jarrett
 \par    DP email: a.jiaweijarrett@digipen.edu
 \par    Course: CSD2401A
 \date   18-09-2023
 \brief

**************************************************************************/

#pragma once

#include <chrono>
#include "LitterBox/Core/System.h"

namespace LB {
	class Time : public ISystem 
	{
		public:
		float GetDeltaTime();
		float GetFixedDeltaTime();

		float GetUnscaledDeltaTime();
		float GetUnscaledFixedDeltaTime();

		float GetTimeScale();
		void SetTimeScale(float newTimeScale);

		float GetTime();

		private:
		float m_deltaTime, m_fixedDeltaTime;
		float m_unscaledDeltaTime, m_unscaledFixedDeltaTime;
		float m_time, m_timeScale;

		int m_targetFrameRate;
	};
}
