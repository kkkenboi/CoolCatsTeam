/*!************************************************************************
 \file				Time.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				19/09/23
 \brief

 This file manages timekeeping for the engine, it has two loops Update (every frame)
 and FixedUpdate (for physics)

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
	 Time class is an Engine system used to limit the frame rate and control 
	 the updating of systems
	*************************************************************************/
	class Time : public ISystem 
	{
		public:
		/*!***********************************************************************
		 \brief
		 Default constructor, ensures this system is a singleton, sets the target
		 frame rate for Update and FixedUpdate
		*************************************************************************/
		Time(int maxFrameRate = 100, int fixedFrameRate = 50);

		/*!***********************************************************************
		 \brief
		 Sets up the name for profiling
		*************************************************************************/
		void Initialize() override { SetSystemName("Time System"); }

		/*!***********************************************************************
		 \brief
		 Returns the time point for at this current moment
		*************************************************************************/
		time_point GetTimeStamp();

		/*!***********************************************************************
		 \brief
		 Time stamps the start of the frame
		*************************************************************************/
		void LBFrameStart();

		/*!***********************************************************************
		 \brief
		 Time stamps the end of the frame and updates dt, fixeddt, etc.
		*************************************************************************/
		void LBFrameEnd();

		/*!***********************************************************************
		 \brief
		 How many times should Update runs per second
		*************************************************************************/
		void SetMaxFrameRate(int fps);

		/*!***********************************************************************
		 \brief
		 Returns the number of times Update runs per second
		*************************************************************************/
		int  GetMaxFrameRate();

		/*!***********************************************************************
		 \brief
		 How many times should FixedUpdate runs per second
		*************************************************************************/
		void SetFixedFrameRate(int fps);

		/*!***********************************************************************
		 \brief
		 Returns time taken last frame
		*************************************************************************/
		double GetDeltaTime();

		/*!***********************************************************************
		 \brief
		 Returns time taken for each FixedUpdate loop
		*************************************************************************/
		double GetFixedDeltaTime();

		/*!***********************************************************************
		 \brief
		 Returns time taken last frame (not affected by timeScale)
		*************************************************************************/
		double GetUnscaledDeltaTime();

		/*!***********************************************************************
		 \brief
		 Returns time taken for each FixedUpdate loop (not affected by timeScale)
		*************************************************************************/
		double GetUnscaledFixedDeltaTime();

		/*!***********************************************************************
		 \brief
		 How fast is the current simulation running? 1 is default
		*************************************************************************/
		double GetTimeScale();

		/*!***********************************************************************
		 \brief
		 Sets the speed for the simulation (1 is default, 0.5 is half speed, etc)
		*************************************************************************/
		void SetTimeScale(double newTimeScale);

		/*!***********************************************************************
		 \brief
		 Returns total time since application launch
		*************************************************************************/
		double GetTime();

		/*!***********************************************************************
		 \brief
		 How long to wait after end of each frame to match the FPS set
		*************************************************************************/
		double GetFrameBudget();

		/*!***********************************************************************
		 \brief
		 How many frames completed since application launch
		*************************************************************************/
		int GetFrameCount();

		/*!***********************************************************************
		 \brief
		 Returns whether FixedUpdate should be called
		*************************************************************************/
		bool ShouldFixedUpdate();

		/*!***********************************************************************
		 \brief
		 Adds Dt to a counter until it is time to FixedUpdate
		*************************************************************************/
		void AccumulateFixedUpdate();

		/*!***********************************************************************
		 \brief
		 Pauses the simulation for this set amount of time
		*************************************************************************/
		void Sleep(double time);

		/*!***********************************************************************
		 \brief
		 Pauses the simulation
		*************************************************************************/
		void Pause(bool shouldPause);

		/*!***********************************************************************
		 \brief
		 Returns whether the simulation is currently paused
		*************************************************************************/
		bool IsPaused();

		/*!***********************************************************************
		 \brief
		 Runs one loop of FixedUpdate
		*************************************************************************/
		void StepFixedDeltaTime();

		Event<> onFrameEnd;									// Used to broadcast that a frame has ended
		
		private:
		bool m_frameStarted { false };						// Whether the frame has started or not

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
