/*!************************************************************************
 \file			Profiler.cpp
 \author		Ang Jiawei Jarrett
 \par DP email: a.jiaweijarrett@digipen.edu
 \par Course:	CSD2401A
 \date			22-09-2023
 \brief

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "Platform/Windows/Windows.h"
#include "Profiler.h"
//#include "EditorProfiler.h" // Send the info to visualize

namespace LB 
{
	ProfilerManager* PROFILER = nullptr;

	/**************************************************************************************************/
	// Profiler manager

	/*!***********************************************************************
	\brief
	Default constructor, ensures this system is a singleton
	*************************************************************************/
	ProfilerManager::ProfilerManager() 
	{
		if (!PROFILER)
			PROFILER = this;
		else
			DebuggerLogError("Profiling System already exist");
	}

	/*!***********************************************************************
	 \brief
	 Adds the information from a finished profiler to the map to print
	*************************************************************************/
	void ProfilerManager::AddProfilerInfo(char const* name, double duration, ProfileMap map, bool overrideInfo)
	{
		switch (map) {
		case ProfileMap::GENERAL:
			overrideInfo ? m_generalInfoMap[name] = duration : m_generalInfoMap[name] += duration;
			break;

		case ProfileMap::SYSTEMS:
			overrideInfo ? m_systemInfoMap[name] = duration : m_systemInfoMap[name] += duration;
			break;
		}
	}

	/*!***********************************************************************
	 \brief
	 Sends the timings for all systems to the editor profiler
	*************************************************************************/
	std::map<std::string, double> const& ProfilerManager::GetSystemFrameTimings()
	{
		return m_systemInfoMap;
	}
	
	//---------------------------Profiler object-----------------------------

	/*!***********************************************************************
	 \brief
	 On profiler object creation, save the time
	*************************************************************************/
	Profiler::Profiler(char const* _name, ProfileResult _result, ProfileMap _map, bool _overrideInfo) : name(_name), result(_result), map(_map), overrideInfo(_overrideInfo)
	{ 
		start = TIME->GetTimeStamp();
	}

	/*!***********************************************************************
	 \brief
	 On file scope end, save time and calculate time information
	*************************************************************************/
	Profiler::~Profiler()
	{
		std::chrono::high_resolution_clock::time_point end = TIME->GetTimeStamp();
		std::chrono::duration<double, std::milli> duration{end - start};
		PROFILER->AddProfilerInfo(name, duration.count(), map, overrideInfo);
	}
}

