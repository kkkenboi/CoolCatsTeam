/*!************************************************************************
 \file			Profiler.h
 \author		Ang Jiawei Jarrett
 \par DP email: a.jiaweijarrett@digipen.edu
 \par Course:	CSD2401A
 \date			22-09-2023
 \brief

**************************************************************************/

#pragma once

#include <map>
#include "LitterBox/Core/System.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Engine/Input.h"

namespace LB 
{
	enum class ProfileResult 
	{
		IMMEDIATE,	// Prints the result immediately after profiling
		MANAGER		// Adds the result to printing queue in the manager
	};

	enum class ProfileMap 
	{
		GENERAL,
		SYSTEMS
	};

	class Profiler 
	{
		public:
		Profiler(char const* name, ProfileResult result, ProfileMap map = ProfileMap::GENERAL, bool overrideInfo = true);
		~Profiler();

		private:
		std::chrono::high_resolution_clock::time_point start;
		char const*   name;
		ProfileResult result;
		ProfileMap    map;
		bool		  overrideInfo; // Should this append or override the duration in the map?
	};

	class ProfilerManager : public ISystem 
	{
		public:
		ProfilerManager();
		void Initialize() override { SetSystemName("Profiler System"); }
		void Destroy() override;

		void AddProfilerInfo(char const* name, double duration, ProfileMap map = ProfileMap::GENERAL, bool overrideInfo = true);
		void DumpGeneralInfo();
		void DumpFrameInfo();
		void SwapSystemInfoMapBuffer();

		private:
		KeyCode dumpGeneralInfoKey = KeyCode::KEY_O;
		KeyCode dumpFrameInfoKey = KeyCode::KEY_P;

		std::map<char const*, double> generalInfoMap;

		bool systemInfoMapID{};
		std::map<char const*, double>* systemInfoMap { &systemInfoMap0 };
		std::map<char const*, double> systemInfoMap0, systemInfoMap1; // Double buffer
	};

	void SwapSystemInfoMapBuffer(); // For event subscription

	void DumpGeneralInfo();
	void DumpFrameInfo();

	extern ProfilerManager* PROFILER;
}
