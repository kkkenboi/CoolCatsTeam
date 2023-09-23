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

	ProfilerManager::ProfilerManager() 
	{
		if (!PROFILER)
			PROFILER = this;
		else
			std::cerr << "Profiling System already exist" << std::endl;
	}

	// Profiler object
	Profiler::Profiler() 
	{
		start = TIME->GetTimeStamp();
	}

	Profiler::~Profiler() 
	{
		std::chrono::high_resolution_clock::time_point end = TIME->GetTimeStamp();

	}

}

