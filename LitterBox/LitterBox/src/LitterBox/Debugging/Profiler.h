/*!************************************************************************
 \file			Profiler.h
 \author		Ang Jiawei Jarrett
 \par DP email: a.jiaweijarrett@digipen.edu
 \par Course:	CSD2401A
 \date			22-09-2023
 \brief

**************************************************************************/

#pragma once

#include <list>
#include "LitterBox/Core/System.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Engine/Input.h"

namespace LB 
{
	enum class ProfileResult {
		IMMEDIATE,	// Prints the result immediately after profiling
		MANAGER		// Adds the result to printing queue in the manager
	};

	struct ProfileInfo
	{
		char const* name;
		double duration;
	};

	class Profiler 
	{
		public:
		Profiler(char const* name, ProfileResult result);
		~Profiler();

		private:
		std::chrono::high_resolution_clock::time_point start;
		char const* name;
		ProfileResult result;
	};

	class ProfilerManager : public ISystem 
	{
		public:
		virtual std::string GetName() { return "Profiling System"; }
		
		ProfilerManager();

		virtual void Destroy() override;

		private:
		KeyCode dumpInfoKey = KeyCode::KEY_P;
		std::list<ProfileInfo> printQueue;
	};

	void DumpInfo();

	extern ProfilerManager* PROFILER;
}
