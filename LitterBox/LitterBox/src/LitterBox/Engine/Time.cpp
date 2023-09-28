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


	 \return

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


	 \return

	*************************************************************************/
	time_point Time::GetTimeStamp() 
	{
		return std::chrono::high_resolution_clock::now();
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void Time::LBFrameStart() 
	{
		m_frameStart = GetTimeStamp();
	}

	/*!***********************************************************************
	 \brief


	 \return

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


	 \return

	*************************************************************************/
	void Time::SetMaxFrameRate(int fps)
	{
		m_maxFrameRate = fps;
		m_minDeltaTime = 1.0 / (double)m_maxFrameRate;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	int Time::GetMaxFrameRate()
	{
		return m_maxFrameRate;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void Time::SetFixedFrameRate(int fps)
	{
		m_fixedFrameRate = fps;
		m_unscaledFixedDeltaTime = 1.0 / (double)m_fixedFrameRate;
		m_fixedDeltaTime = m_unscaledFixedDeltaTime * m_timeScale;
	}

	/*!***********************************************************************
	 \brief


	 \return

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


	 \return

	*************************************************************************/
	void Time::AccumulateFixedUpdate()
	{
		m_accumulatedTime += m_unscaledDeltaTime;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	double Time::GetDeltaTime()
	{
		return m_deltaTime;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	double Time::GetFixedDeltaTime()
	{
		return m_fixedDeltaTime;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	double Time::GetUnscaledDeltaTime()
	{
		return m_unscaledDeltaTime;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	double Time::GetUnscaledFixedDeltaTime()
	{
		return m_unscaledFixedDeltaTime;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	double Time::GetTimeScale()
	{
		return m_timeScale;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void Time::SetTimeScale(double newTimeScale)
	{
		m_timeScale = newTimeScale;
		m_fixedDeltaTime = m_unscaledFixedDeltaTime * m_timeScale;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	double Time::GetTime()
	{
		return m_time;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	double Time::GetFrameBudget()
	{
		return m_frameBudget;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	int Time::GetFrameCount()
	{
		return frameCounter;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void Time::Sleep(double time) 
	{
		std::this_thread::sleep_for(std::chrono::duration<double>(time));
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void Time::Pause(bool shouldPause) 
	{
		UNREFERENCED_PARAMETER(shouldPause);
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void Time::ToggleVSync(bool on)
	{
		UNREFERENCED_PARAMETER(on);

	}
}