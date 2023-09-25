/*!************************************************************************
 \file			Profiler.cpp
 \author		Ang Jiawei Jarrett
 \par DP email: a.jiaweijarrett@digipen.edu
 \par Course:	CSD2401A
 \date			22-09-2023
 \brief

**************************************************************************/

#include "Platform/Windows/Windows.h"
#include "Profiler.h"
#include "LitterBox/Renderer/Renderer.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Debugging/Memory.h"

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

		TIME->onFrameEnd.Subscribe(LB::SwapSystemInfoMapBuffer);
	}

	void ProfilerManager::Destroy() 
	{
		INPUT->UnsubscribeToKey(LB::DumpGeneralInfo, dumpGeneralInfoKey, KeyEvent::TRIGGERED);
		INPUT->UnsubscribeToKey(LB::DumpFrameInfo, dumpFrameInfoKey, KeyEvent::TRIGGERED);

		TIME->onFrameEnd.Unsubscribe(LB::SwapSystemInfoMapBuffer);
	}

	void ProfilerManager::AddProfilerInfo(char const* name, double duration, ProfileMap map, bool overrideInfo)
	{
		switch (map) {
		case ProfileMap::GENERAL:
			overrideInfo ? generalInfoMap[name] = duration : generalInfoMap[name] += duration;
			break;

		case ProfileMap::SYSTEMS:
			overrideInfo ? (*systemInfoMap)[name] = duration : (*systemInfoMap)[name] += duration;
			break;
		}
	}

	void ProfilerManager::SwapSystemInfoMapBuffer()
	{
		// Record frame number, budget, FPS before swapping
		(*systemInfoMap)["Frame Number"] = TIME->GetFrameCount();

		double frameBudget = TIME->GetFrameBudget();
		frameBudget = (frameBudget < 0.0) ? 0.0 : frameBudget;

		(*systemInfoMap)["Frame Budget"] = frameBudget;

		// (*systemInfoMap)["Frame Dt"] = TIME->GetUnscaledDeltaTime();

		// Swap the buffer
		systemInfoMapID = !systemInfoMapID;

		if (!systemInfoMapID) {
			systemInfoMap = &systemInfoMap0;
		}
		else 
		{
			systemInfoMap = &systemInfoMap1;
		}
	}

	void SwapSystemInfoMapBuffer()
	{
		PROFILER->SwapSystemInfoMapBuffer();
	}

	void ProfilerManager::DumpGeneralInfo()
	{
		// Print and delete the information

		std::cout << std::setfill('=') << std::setw(59) << "=" << std::setfill(' ') << "\n";
		std::cout << "General Profiling Snapshot\n";

		for (auto const& info : generalInfoMap) {
			std::cout << info.first << ": " << info.second << " milliseconds\n";
		}

		std::cout << std::setfill('=') << std::setw(59) << "=" << std::setfill(' ') << "\n";
		generalInfoMap.clear();
	}

	void ProfilerManager::DumpFrameInfo()
	{
		double totalTime		= (*systemInfoMap)["Total Frame Time"];
		double inputSystem		= (*systemInfoMap)[INPUT->GetName().c_str()];
		double windowsSystem	= (*systemInfoMap)[WINDOWSSYSTEM->GetName().c_str()];
		double factorySystem	= (*systemInfoMap)[FACTORY->GetName().c_str()];
		double renderSystem		= (*systemInfoMap)[Renderer::GRAPHICS->GetName().c_str()];
		double memorySystem		= (*systemInfoMap)[MEMORY->GetName().c_str()];

		// Calculate the overhead for profiling (difference in total time, and time spent for each system)
		double profilingSystem	= totalTime - (*systemInfoMap)[PROFILER->GetName().c_str()] - inputSystem - windowsSystem - factorySystem - renderSystem - memorySystem;

		/**************************************************************************************************/
		// Nicely formatted output :D
		std::cout << std::setfill('=')	<< std::setw(59) << "=" << std::setfill(' ') << "\n";
		std::cout << "Target FPS: "		<< TIME->GetMaxFrameRate() << "\n";
		std::cout << "Average FPS: "	<< std::fixed << std::setprecision(2) << (double)TIME->GetFrameCount() / TIME->GetTime() << "\n";

		std::cout << std::setfill('=')	<< std::setw(59) << "=" << std::setfill(' ') << "\n";
		std::cout << "Frame #" << (int)(*systemInfoMap)["Frame Number"] << " Snapshot\n";
		//std::cout << "Actual FPS: "		<< std::fixed << std::setprecision(2) << 60.0 / ((*systemInfoMap)["Frame Dt"] * 100.0) << "\n\n";

		std::cout << std::left << std::setw(39) << "Total Frame Time" << std::right << std::setw(7) << std::setprecision(4) << totalTime << " milliseconds\n\n";

		// Print in order of execution in the order ( Percentage | System name | Actual time )
		std::cout << std::right << std::setw(5) << std::setprecision(2) << (inputSystem / totalTime) * 100.0		<< "% | " << std::left << std::setw(30) << INPUT->GetName()					<< std::right << std::setw(7) << std::setprecision(4) << inputSystem		<< " milliseconds\n";
		
		std::cout << std::right << std::setw(5) << std::setprecision(2) << (windowsSystem / totalTime) * 100.0		<< "% | " << std::left << std::setw(30) << WINDOWSSYSTEM->GetName()			<< std::right << std::setw(7) << std::setprecision(4) << windowsSystem		<< " milliseconds\n";
		
		std::cout << std::right << std::setw(5) << std::setprecision(2) << (factorySystem / totalTime) * 100.0		<< "% | " << std::left << std::setw(30) << FACTORY->GetName()				<< std::right << std::setw(7) << std::setprecision(4) << factorySystem		<< " milliseconds\n";
		
		std::cout << std::right << std::setw(5) << std::setprecision(2) << (renderSystem / totalTime) * 100.0		<< "% | " << std::left << std::setw(30) << Renderer::GRAPHICS->GetName()	<< std::right << std::setw(7) << std::setprecision(4) << renderSystem		<< " milliseconds\n";
		
		std::cout << std::right << std::setw(5) << std::setprecision(2) << (memorySystem / totalTime) * 100.0		<< "% | " << std::left << std::setw(30) << MEMORY->GetName()				<< std::right << std::setw(7) << std::setprecision(4) << memorySystem		<< " milliseconds\n";
		
		std::cout << std::right << std::setw(5) << std::setprecision(2) << (profilingSystem / totalTime) * 100.0	<< "% | " << std::left << std::setw(30) << PROFILER->GetName()				<< std::right << std::setw(7) << std::setprecision(4) << profilingSystem	<< " milliseconds\n\n";

		std::cout << std::left << std::setw(39) << "Frame Budget" << std::right << std::setw(7) << std::setprecision(4) << (*systemInfoMap)["Frame Budget"] << " milliseconds\n" << std::left;
		std::cout << std::setfill('=') << std::setw(59) << "=" << std::setfill(' ') << "\n\n";
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
	Profiler::Profiler(char const* _name, ProfileResult _result, ProfileMap _map, bool _overrideInfo) : name(_name), result(_result), map(_map), overrideInfo(_overrideInfo)
	{ 
		start = TIME->GetTimeStamp();
	}

	Profiler::~Profiler() 
	{
		std::chrono::high_resolution_clock::time_point end = TIME->GetTimeStamp();
		std::chrono::duration<double, std::milli> duration{end - start};
		PROFILER->AddProfilerInfo(name, duration.count(), map, overrideInfo);
	}
}

