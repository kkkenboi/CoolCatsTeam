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

	std::chrono::high_resolution_clock::time_point Time::GetTimeStamp() 
	{
		return std::chrono::high_resolution_clock::now();
	}

	void Time::LBFrameStart() {
		m_frameStart = GetTimeStamp();
	}

	void Time::LBFrameEnd() {
		m_frameEnd = GetTimeStamp();

		m_frameDuration = m_frameEnd - m_frameStart;

		m_unscaledDeltaTime = m_frameDuration.count();
		// Check if we need to wait before next frame
		if (m_unscaledDeltaTime < m_minDeltaTime) {
			TIME->Sleep(m_minDeltaTime - m_unscaledDeltaTime);
			m_unscaledDeltaTime = m_minDeltaTime;
		}

		m_deltaTime = m_unscaledDeltaTime * m_timeScale;
		m_time += m_unscaledDeltaTime;
	}

	void Time::SetMaxFrameRate(double fps)
	{
		m_maxFrameRate = fps;
		m_minDeltaTime = 1.0 / m_maxFrameRate;
	}

	void Time::SetFixedFrameRate(double fps)
	{
		m_fixedFrameRate = fps;
		m_unscaledFixedDeltaTime = 1.0 / m_fixedFrameRate;
		m_fixedDeltaTime = m_unscaledFixedDeltaTime * m_timeScale;
	}

	bool Time::ShouldFixedUpdate() 
	{
		m_elapsedTime += m_unscaledDeltaTime;
		if (m_elapsedTime > m_unscaledFixedDeltaTime) {
			m_elapsedTime = 0.0;
			return true;
		}
		return false;
	}

	double Time::GetDeltaTime()
	{
		return m_deltaTime;
	}

	double Time::GetFixedDeltaTime()
	{
		return m_fixedDeltaTime;
	}

	double Time::GetUnscaledDeltaTime()
	{
		return m_unscaledDeltaTime;
	}

	double Time::GetUnscaledFixedDeltaTime()
	{
		return m_unscaledFixedDeltaTime;
	}

	double Time::GetTimeScale()
	{
		return m_timeScale;
	}

	void Time::SetTimeScale(double newTimeScale)
	{
		m_timeScale = newTimeScale;
		m_fixedDeltaTime = m_unscaledFixedDeltaTime * m_timeScale;
	}

	double Time::GetTime()
	{
		return m_time;
	}

	void Time::Sleep(double time) 
	{
		std::this_thread::sleep_for(std::chrono::duration<double>(time));
	}

	void Time::Pause(bool shouldPause) 
	{

	}

}