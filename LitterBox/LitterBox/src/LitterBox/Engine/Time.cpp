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

		m_frameEnd = m_frameStart = std::chrono::high_resolution_clock::now();
	}

	void Time::LBFrameStart() {
		m_frameStart = std::chrono::high_resolution_clock::now();
	}

	void Time::LBFrameEnd() {
		m_frameEnd = std::chrono::high_resolution_clock::now();

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
	}

	double Time::GetTime()
	{
		return m_time;
	}

	void Time::Sleep(double time) 
	{
		std::this_thread::sleep_for(std::chrono::duration<double>(time));
	}
}