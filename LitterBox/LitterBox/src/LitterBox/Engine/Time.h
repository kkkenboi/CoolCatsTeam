/*!************************************************************************
 \file				Time.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				19/09/23
 \brief

 This file tracks allocations and deallocations used by the game engine.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <chrono>
#include "LitterBox/Core/System.h"
#include "LitterBox/Engine/Events.h"

namespace LB {
	typedef std::chrono::high_resolution_clock::time_point time_point;

	/*!***********************************************************************
	 \brief

	*************************************************************************/
	class Time : public ISystem 
	{
		public:
		/*!***********************************************************************
		 \brief

		*************************************************************************/
		Time(int maxFrameRate = 100, int fixedFrameRate = 50);

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		void Initialize() override { SetSystemName("Time System"); }

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		time_point GetTimeStamp();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		void LBFrameStart();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		void LBFrameEnd();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		void SetMaxFrameRate(int fps);

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		int  GetMaxFrameRate();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		void SetFixedFrameRate(int fps);

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		double GetDeltaTime();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		double GetFixedDeltaTime();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		double GetUnscaledDeltaTime();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		double GetUnscaledFixedDeltaTime();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		double GetTimeScale();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		void SetTimeScale(double newTimeScale);

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		double GetTime();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		double GetFrameBudget();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		int GetFrameCount();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		bool ShouldFixedUpdate();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		void AccumulateFixedUpdate();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		void Sleep(double time);

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		void Pause(bool shouldPause);

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		bool IsPaused();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		void StepFixedDeltaTime();

		Event<> onFrameEnd;									// Used to broadcast that a frame has ended
		
		private:
		time_point m_frameStart, m_frameEnd;				// Time points to calculate the time taken for each frame
		std::chrono::duration<double> m_frameDuration;		// Length of time from m_frameStart and m_frameEnd

		double m_deltaTime{}, m_unscaledDeltaTime{};		// How much time passed each frame (m_deltaTime is scaled by timeScale, m_unscaledDeltaTime is not)
		double m_fixedDeltaTime, m_unscaledFixedDeltaTime;	// m_fixedDeltaTime is used to maintain a constant stable loop at a fixed interval

		double m_time{ 0.0 };								// Time tracks the total amount of time passed since application start
		double m_timeScale{ 1.0 };							// Changes deltaTime and fixedDeltaTime, useful to speed up/slow down

		double m_timeScaleBeforePause{};					// Used to return the timescale back to before it was paused
		double m_accumulatedTime{};							// Accumulates time until fixed delta time, used by FixedUpdate for constant loops	
		double m_minDeltaTime;								// The minimum delta time should be (prevents loop from running too fast)		
		double m_frameBudget{};								// The amount of time to wait before next frame (if it is going too fast)	 

		long frameCounter{};								// The total number of frames rendered since application start		
		int m_maxFrameRate, m_fixedFrameRate;				// The target fps for Update and FixedUpdate
	};

	// A pointer to the system object in the core engine made to be singleton
	extern Time* TIME;
}
