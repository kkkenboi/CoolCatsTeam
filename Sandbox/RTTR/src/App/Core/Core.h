
#pragma once

#include <vector>
#include <memory>

#include "ISystem.h"

namespace LB
{
	class LBEngine
	{
	public:
		void Create();

		void Initialize();

		void Update();

		void Destroy();

	private:
		std::vector<std::shared_ptr<ISystem>> m_systems, m_updateSystems, m_fixedUpdateSystems;
	};
}
