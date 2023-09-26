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
	typedef std::chrono::high_resolution_clock::time_point time_point;

	class Time : public ISystem 
	{
		public:
		Time(int maxFrameRate = 100, int fixedFrameRate = 50);

		void Initialize() override { SetSystemName("Time System"); }

		time_point GetTimeStamp();

		void LBFrameStart();
		void LBFrameEnd();

		void SetMaxFrameRate(int fps);
		int  GetMaxFrameRate();
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
		void AccumulateFixedUpdate();

		void Sleep(double time);
		void Pause(bool shouldPause);
		void ToggleVSync(bool on);

		// Used to broadcast that a frame has ended
		Event<> onFrameEnd;

		private:
		// Time points to calculate the time taken for each frame
		time_point m_frameStart, m_frameEnd;
		std::chrono::duration<double> m_frameDuration; // Length of time from m_frameStart and m_frameEnd

		// How much time passed each frame (m_deltaTime is scaled by timeScale, m_unscaledDeltaTime is not)
		double m_deltaTime{}, m_unscaledDeltaTime{};
		
		// m_fixedDeltaTime is used to maintain a constant stable loop at a fixed interval
		double m_fixedDeltaTime, m_unscaledFixedDeltaTime;

		double m_time{ 0.0 };				// Time tracks the total amount of time passed since application start
		double m_timeScale{ 1.0 };			// Changes deltaTime and fixedDeltaTime, useful to speed up/slow down

		double m_timeScaleBeforePause{};	// Used to return the timescale back to before it was paused

		double m_accumulatedTime{};			// Accumulates time until fixed delta time, used by FixedUpdate for constant loops
		double m_minDeltaTime;				// The minimum delta time should be (prevents loop from running too fast)
		double m_frameBudget{};				// The amount of time to wait before next frame (if it is going too fast)

		long frameCounter{};				// The total number of frames rendered since application start

		// The target fps for Update and FixedUpdate
		int m_maxFrameRate, m_fixedFrameRate;
	};

	// A pointer to the system object in the core engine made to be singleton
	extern Time* TIME;
}
