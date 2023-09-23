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

namespace LB 
{
	struct ProfileInfo
	{
		std::string 
	};

	class Profiler 
	{
		public:
		Profiler();
		~Profiler();

		private:
		std::chrono::high_resolution_clock::time_point start;
	};

	class ProfilerManager : public ISystem 
	{
		public:
		ProfilerManager();

		virtual std::string GetName() { return "Profiling System"; }

		private:
		std::list<
	};

	extern ProfilerManager* PROFILER;
}
