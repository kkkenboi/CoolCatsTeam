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

		INPUT->SubscribeToKey(DumpInfo, dumpInfoKey, KeyEvent::TRIGGERED);
	}

	void ProfilerManager::Destroy() 
	{
		INPUT->UnsubscribeToKey(DumpInfo, dumpInfoKey, KeyEvent::TRIGGERED);
	}

	void DumpInfo()
	{

	}

	/**************************************************************************************************/
	// Profiler object
	Profiler::Profiler(char const* _name, ProfileResult _result) : name(_name), result(_result), start(TIME->GetTimeStamp()) { }

	Profiler::~Profiler() 
	{
		std::chrono::high_resolution_clock::time_point end = TIME->GetTimeStamp();
		std::chrono::duration<double> duration{end - start};
	}

}

