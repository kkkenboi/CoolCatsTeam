/*!************************************************************************
 \file			Profiler.cpp
 \author		Ang Jiawei Jarrett
 \par DP email: a.jiaweijarrett@digipen.edu
 \par Course:	CSD2401A
 \date			22-09-2023
 \brief

**************************************************************************/

#include "Profiler.h"

namespace LB 
{
	ProfilerManager* PROFILER = nullptr;

	/**************************************************************************************************/
	// Profiler manager
	ProfilerManager::ProfilerManager() 
	{
		if (!PROFILER)
			PROFILER = this;
		else
			std::cerr << "Profiling System already exist" << std::endl;

		INPUT->SubscribeToKey(LB::DumpGeneralInfo, dumpGeneralInfoKey, KeyEvent::TRIGGERED);
		INPUT->SubscribeToKey(LB::DumpFrameInfo, dumpFrameInfoKey, KeyEvent::TRIGGERED);
	}

	void ProfilerManager::Destroy() 
	{
		INPUT->UnsubscribeToKey(LB::DumpGeneralInfo, dumpGeneralInfoKey, KeyEvent::TRIGGERED);
		INPUT->UnsubscribeToKey(LB::DumpFrameInfo, dumpFrameInfoKey, KeyEvent::TRIGGERED);
	}

	void ProfilerManager::AddProfilerInfo(char const* name, double duration, ProfileMap map)
	{
		switch (map) {
		case ProfileMap::GENERAL:
			generalInfoMap[name] = duration;
			break;

		case ProfileMap::SYSTEMS:
			systemInfoMap[name] = duration;
			break;
		}
	}

	void ProfilerManager::DumpGeneralInfo()
	{
		for (auto const& info : generalInfoMap) {
			std::cout << info.first << ": " << info.second << " milliseconds\n";
		}
	}

	void ProfilerManager::DumpFrameInfo()
	{
		for (auto const& info : systemInfoMap) {
			std::cout << info.first << ": " << info.second << " milliseconds\n";
		}
	}

	void DumpGeneralInfo()
	{
		PROFILER->DumpGeneralInfo();
	}

	void DumpFrameInfo()
	{
		PROFILER->DumpFrameInfo();
	}

	/**************************************************************************************************/
	// Profiler object
	Profiler::Profiler(char const* _name, ProfileResult _result, ProfileMap _map) : name(_name), result(_result), map(_map) 
	{ 
		start = TIME->GetTimeStamp();
	}

	Profiler::~Profiler() 
	{
		std::chrono::high_resolution_clock::time_point end = TIME->GetTimeStamp();
		std::chrono::duration<double, std::milli> duration{end - start};
		PROFILER->AddProfilerInfo(name, duration.count(), map);
	}
}

