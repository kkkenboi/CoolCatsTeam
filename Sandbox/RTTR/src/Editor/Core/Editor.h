
#pragma once

#include <vector>
#include <memory>

#include "IWindow.h"

namespace LB
{
	class Editor
	{
	public:
		void CreateContext();

		void DestroyContext();

		void InitializeEditor();

		void UpdateEditor();

	private:
		std::vector<std::shared_ptr<IWindow>> m_windows;
	};
}
