/*!************************************************************************
 \file			Time.cpp
 \author		Ang Jiawei Jarrett
 \par DP email: a.jiaweijarrett@digipen.edu
 \par Course:	CSD2401A
 \date			19-09-2023
 \brief

**************************************************************************/

#include "Time.h"

namespace LB {
	Time* TIME = nullptr;

	/*!***********************************************************************
	\brief
	Default constructor, ensures this system is a singleton, sets the target
	frame rate for Update and FixedUpdate
	*************************************************************************/
	Time::Time(int maxFrameRate, int fixedFrameRate) 
	{ 
		if (!TIME)
			TIME = this;
		else
			std::cerr << "Render System already exist" << std::endl;

		SetMaxFrameRate(maxFrameRate);
		SetFixedFrameRate(fixedFrameRate);

		m_frameEnd = m_frameStart = GetTimeStamp();
	}

	/*!***********************************************************************
	\brief
	Returns the time point for at this current moment
	*************************************************************************/
	time_point Time::GetTimeStamp() 
	{
		return std::chrono::high_resolution_clock::now();
	}

	/*!***********************************************************************
	\brief
	Time stamps the start of the frame
	*************************************************************************/
	void Time::LBFrameStart() 
	{
		m_frameStart = GetTimeStamp();
	}

	/*!***********************************************************************
	\brief
	Time stamps the end of the frame and updates dt, fixeddt, etc.
	*************************************************************************/
	void Time::LBFrameEnd() 
	{
		m_frameEnd = GetTimeStamp();

		m_frameDuration = m_frameEnd - m_frameStart;

		m_unscaledDeltaTime = m_frameDuration.count();

		m_frameBudget = m_minDeltaTime - m_unscaledDeltaTime;
		// Check if we need to wait before next frame
		if (m_frameBudget > 0.0) {
			TIME->Sleep(m_frameBudget);
			m_unscaledDeltaTime = m_minDeltaTime;
		}

		m_deltaTime = m_unscaledDeltaTime * m_timeScale;
		m_time += m_unscaledDeltaTime;

		onFrameEnd.Invoke();

		++frameCounter;
	}

	/*!***********************************************************************
	\brief
	How many times should Update runs per second
	*************************************************************************/
	void Time::SetMaxFrameRate(int fps)
	{
		m_maxFrameRate = fps;
		m_minDeltaTime = 1.0 / (double)m_maxFrameRate;
	}

	/*!***********************************************************************
	\brief
	Returns the number of times Update runs per second
	*************************************************************************/
	int Time::GetMaxFrameRate()
	{
		return m_maxFrameRate;
	}

	/*!***********************************************************************
	\brief
	How many times should FixedUpdate runs per second
	*************************************************************************/
	void Time::SetFixedFrameRate(int fps)
	{
		m_fixedFrameRate = fps;
		m_unscaledFixedDeltaTime = 1.0 / (double)m_fixedFrameRate;
		m_fixedDeltaTime = m_unscaledFixedDeltaTime * m_timeScale;
	}

	/*!***********************************************************************
	\brief
	Returns whether FixedUpdate should be called
	*************************************************************************/
	bool Time::ShouldFixedUpdate() 
	{
		if (m_accumulatedTime > m_unscaledFixedDeltaTime) {
			m_accumulatedTime = 0.0;
			return true;
		}
		return false;
	}

	/*!***********************************************************************
	\brief
	Adds Dt to a counter until it is time to FixedUpdate
	*************************************************************************/
	void Time::AccumulateFixedUpdate()
	{
		if (!IsPaused())
		{
			m_accumulatedTime += m_unscaledDeltaTime;
		}
	}

	/*!***********************************************************************
	\brief
	Returns time taken last frame
	*************************************************************************/
	double Time::GetDeltaTime()
	{
		return m_deltaTime;
	}

	/*!***********************************************************************
	\brief
	Returns time taken for each FixedUpdate loop
	*************************************************************************/
	double Time::GetFixedDeltaTime()
	{
		return m_fixedDeltaTime;
	}

	/*!***********************************************************************
	\brief
	Returns time taken last frame (not affected by timeScale)
	*************************************************************************/
	double Time::GetUnscaledDeltaTime()
	{
		return m_unscaledDeltaTime;
	}

	/*!***********************************************************************
	\brief
	Returns time taken for each FixedUpdate loop (not affected by timeScale)
	*************************************************************************/
	double Time::GetUnscaledFixedDeltaTime()
	{
		return m_unscaledFixedDeltaTime;
	}

	/*!***********************************************************************
	\brief
	How fast is the current simulation running? 1 is default
	*************************************************************************/
	double Time::GetTimeScale()
	{
		return m_timeScale;
	}

	/*!***********************************************************************
	\brief
	Sets the speed for the simulation (1 is default, 0.5 is half speed, etc)
	*************************************************************************/
	void Time::SetTimeScale(double newTimeScale)
	{
		if (newTimeScale < 0.0) 
		{
			DebuggerLogWarning("Time Scale cannot be less than 0!");
			return;
		}

		m_timeScale = newTimeScale;
		m_fixedDeltaTime = m_unscaledFixedDeltaTime * m_timeScale;
	}

	/*!***********************************************************************
	\brief
	Returns total time since application launch
	*************************************************************************/
	double Time::GetTime()
	{
		return m_time;
	}

	/*!***********************************************************************
	\brief
	How long to wait after end of each frame to match the FPS set
	*************************************************************************/
	double Time::GetFrameBudget()
	{
		return m_frameBudget;
	}

	/*!***********************************************************************
	\brief
	How many frames completed since application launch
	*************************************************************************/
	int Time::GetFrameCount()
	{
		return frameCounter;
	}

	/*!***********************************************************************
	\brief
	Pauses the simulation for this set amount of time
	*************************************************************************/
	void Time::Sleep(double time) 
	{
		std::this_thread::sleep_for(std::chrono::duration<double>(time));
	}

	/*!***********************************************************************
	\brief
	Pauses the simulation
	*************************************************************************/
	void Time::Pause(bool shouldPause) 
	{
		if (!IsPaused() && shouldPause)
		{
			m_timeScaleBeforePause = m_timeScale;
			m_timeScale = 0.0;
		}
		else if (IsPaused() && !shouldPause)
		{
			m_timeScale = m_timeScaleBeforePause;
		}
	}

	/*!***********************************************************************
	 \brief
	 Returns whether the simulation is currently paused
	*************************************************************************/
	bool Time::IsPaused()
	{
		return m_timeScale == 0.0;
	}

	/*!***********************************************************************
	 \brief
	 Runs one loop of FixedUpdate
	*************************************************************************/
	void Time::StepFixedDeltaTime()
	{
		// Accumulate time to ensure one Fixed Update
		m_accumulatedTime += m_unscaledFixedDeltaTime + 0.001;
	}
}