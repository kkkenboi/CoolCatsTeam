
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

		void InitializeEngine() { m_engine->Initialize(); }

		void UpdateEngine() { m_engine->Update(); }

		bool IsRunning() const { return m_running; }

		void Quit() { m_running = false; }

	private:
		bool m_running;

		std::shared_ptr<LBEngine> m_engine;
	};
}
