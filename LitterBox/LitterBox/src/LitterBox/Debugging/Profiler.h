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
		Profiler(char const* name, ProfileResult result, ProfileMap map = ProfileMap::GENERAL);
		~Profiler();

		private:
		std::chrono::high_resolution_clock::time_point start;
		char const* name;
		ProfileResult result;
		ProfileMap map;
	};

	class ProfilerManager : public ISystem 
	{
		public:
		ProfilerManager();
		void Initialize() override { SetSystemName("Profiler System"); }

		virtual void Destroy() override;

		void AddProfilerInfo(char const* name, double duration, ProfileMap map = ProfileMap::GENERAL);
		void DumpGeneralInfo();
		void DumpFrameInfo();

		private:
		KeyCode dumpGeneralInfoKey = KeyCode::KEY_O;
		KeyCode dumpFrameInfoKey = KeyCode::KEY_P;

		std::map<char const*, double> generalInfoMap;
		std::map<char const*, double> systemInfoMap;
	};

	void DumpGeneralInfo();
	void DumpFrameInfo();

	extern ProfilerManager* PROFILER;
}
