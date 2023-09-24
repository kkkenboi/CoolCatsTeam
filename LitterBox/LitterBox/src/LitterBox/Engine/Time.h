/*!************************************************************************
 \file			Time.h
 \author		Ang Jiawei Jarrett
 \par DP email: a.jiaweijarrett@digipen.edu
 \par Course:	CSD2401A
 \date			19-09-2023
 \brief

**************************************************************************/

#pragma once

#include <chrono>
#include "LitterBox/Core/System.h"
#include "LitterBox/Engine/Events.h"

namespace LB {
	class Time : public ISystem 
	{
		public:
		Time(int maxFrameRate = 100, int fixedFrameRate = 50);

		void Initialize() override { SetSystemName("Time System"); }

		std::chrono::high_resolution_clock::time_point GetTimeStamp();

		void LBFrameStart();
		void LBFrameEnd();

		void SetMaxFrameRate(int fps);
		int GetMaxFrameRate();
		void SetFixedFrameRate(int fps);

		double GetDeltaTime();
		double GetFixedDeltaTime();

		double GetUnscaledDeltaTime();
		double GetUnscaledFixedDeltaTime();

		double GetTimeScale();
		void SetTimeScale(double newTimeScale);

		double GetTime();
		double GetFrameBudget();
		int GetFrameCount();

		bool ShouldFixedUpdate();

		Event<> onFrameEnd;

		void Sleep(double time);

		void Pause(bool shouldPause);

		private:
		std::chrono::high_resolution_clock::time_point m_frameStart, m_frameEnd;
		std::chrono::duration<double> m_frameDuration;

		double m_deltaTime{}, m_unscaledDeltaTime{};
		double m_time{ 0.0 }, m_timeScale{ 1.0 };

		double m_elapsedTime{}, m_timeScaleBeforePause{}, m_frameBudget{};

		double m_minDeltaTime, m_fixedDeltaTime, m_unscaledFixedDeltaTime;
		int m_maxFrameRate, m_fixedFrameRate;

		long frameCounter{};
	};

	// A pointer to the system object in the core engine made to be singleton
	extern Time* TIME;
}
