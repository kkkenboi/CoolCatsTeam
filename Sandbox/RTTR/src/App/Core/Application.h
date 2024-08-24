/*!************************************************************************
 \file				Application.h
 \author(s)
 \par DP email(s):
 \par Course:
 \date
 \brief

 This file contains

 Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Core.h"

namespace LB
{
	class Application
	{
	public:
		void CreateContext();

		void DestroyContext();

		void NewFrame();

		void EndFrame();

		inline void InitializeEngine() { m_engine->Initialize(); }

		inline void UpdateEngine() { m_engine->Update(); }

		inline bool IsRunning() const { return m_running; }

		inline void Quit() { m_running = false; }

	private:
		bool m_running;

		std::shared_ptr<LBEngine> m_engine;
	};
}
