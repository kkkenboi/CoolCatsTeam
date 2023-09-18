#include "Time.h"

namespace LB {
	float Time::GetDeltaTime() 
	{
		return m_deltaTime;
	}

	float Time::GetFixedDeltaTime() 
	{
		return m_fixedDeltaTime;
	}

	float Time::GetUnscaledDeltaTime() 
	{
		return m_unscaledDeltaTime;
	}

	float Time::GetUnscaledFixedDeltaTime() 
	{
		return m_unscaledFixedDeltaTime;
	}

	float Time::GetTimeScale() 
	{
		return m_timeScale;
	}

	void Time::SetTimeScale(float newTimeScale)
	{
		m_timeScale = newTimeScale;
	}

	float Time::GetTime() 
	{
		return m_time;
	}
}