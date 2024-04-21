
#pragma once

#include <vector>
#include <memory>

#include "ISystem.h"

namespace LB
{
	class LBEngine
	{
	public:
		void Initialize();

	private:
		std::vector<std::shared_ptr<ISystem>> m_systems;
	};
}
