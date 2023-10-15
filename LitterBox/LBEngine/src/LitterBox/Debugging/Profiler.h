/*!************************************************************************
 \file				Profiler.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				22/09/23
 \brief

 This file profiles the time taken for each frame by each system as well as
 any general profiler.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <map>
#include "LitterBox/Core/System.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Engine/Input.h"

namespace LB 
{
	/*!***********************************************************************
	 \brief
	 When should the profile result print?
	*************************************************************************/
	enum class ProfileResult
	{
		IMMEDIATE,	// Prints the result immediately after profiling
		MANAGER		// Adds the result to printing queue in the manager
	};

	/*!***********************************************************************
	 \brief
	 What kind of profile is this?
	*************************************************************************/
	enum class ProfileMap
	{
		GENERAL,	// Saves result in general snapshot
		SYSTEMS		// Saves result in systems (frame) snapshot
	};

	/*!***********************************************************************
	 \brief
	 Profiler class is an object that can be added to any scope, tracks the
	 time taken from init to end of scope
	*************************************************************************/
	class Profiler
	{
	public:
		/*!***********************************************************************
		 \brief
		 On profiler object creation, save the time, sets where and when and what to print
		*************************************************************************/
		Profiler(char const* name, ProfileResult result, ProfileMap map = ProfileMap::GENERAL, bool overrideInfo = true);
		
		/*!***********************************************************************
		 \brief
		 On file scope end, save time and calculate time information
		*************************************************************************/
		~Profiler();

	private:
		std::chrono::high_resolution_clock::time_point start;	// Time on profiler creation
		char const*   name;										// The name to print out

		ProfileResult result;		// When to print the result?
		ProfileMap    map;			// Which snapshot to print to?
		bool		  overrideInfo;	// Should this append or override the duration in the map?

	};

	/*!***********************************************************************
	 \brief
	 Profiler class is an Engine system that tracks the time spent each frame
	 as well as any general information
	*************************************************************************/
	class ProfilerManager : public ISystem
	{
	public:
		/*!***********************************************************************
		\brief
		Profiler class is an Engine system that tracks the time spent each frame
		as well as any general information
		*************************************************************************/
		ProfilerManager();

		/*!***********************************************************************
		\brief
		Sets up the name for profiling
		*************************************************************************/
		void Initialize() override { SetSystemName("Profiler System"); }

		/*!***********************************************************************
		\brief
		Sets up the name for profiling
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		 \brief
		 Adds the information from a finished profiler to the map to print
		*************************************************************************/
		void AddProfilerInfo(char const* name, double duration, ProfileMap map = ProfileMap::GENERAL, bool overrideInfo = true);
		
		/*!***********************************************************************
		 \brief
		 Prints all the timings stored in the general snapshot
		*************************************************************************/
		void DumpGeneralInfo();

		/*!***********************************************************************
		 \brief
		 Prints all the timings in the systems (frame) snapshot
		*************************************************************************/
		void DumpFrameInfo();

		/*!***********************************************************************
		\brief
		 Every frame, the buffer for times needs to swap to print accurate info
		*************************************************************************/
		void SwapSystemInfoMapBuffer();

	private:
		//-----------------Key bindings to print info--------------------------
		KeyCode dumpGeneralInfoKey = KeyCode::KEY_K;
		KeyCode dumpFrameInfoKey = KeyCode::KEY_L;

		//-----------------Map of profile timings--------------------------
		std::map<char const*, double> generalInfoMap;

		bool systemInfoMapID{};
		std::map<char const*, double>* systemInfoMap { &systemInfoMap0 };
		std::map<char const*, double> systemInfoMap0, systemInfoMap1; // Double buffer
	};

	/*!***********************************************************************
	 \brief
	 For event subscribing, at the end of each frame, swap buffer
	*************************************************************************/
	void SwapSystemInfoMapBuffer(); // For event subscription

	/*!***********************************************************************
	 \brief
	 For event subscription, print general snapshot on key press
	*************************************************************************/
	void DumpGeneralInfo();

	/*!***********************************************************************
	 \brief
	 For event subscription, print systems (frame) snapshot on key press
	*************************************************************************/
	void DumpFrameInfo();

	// A pointer to the system object in the core engine made to be singleton
	extern ProfilerManager* PROFILER;
}
