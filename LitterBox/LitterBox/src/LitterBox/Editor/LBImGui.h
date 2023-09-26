#pragma once

#include "LitterBox/Core/System.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"

namespace LB
{
	class Editor : public ISystem
	{
		public:
		Editor();

		void Initialize() override { SetSystemName("Editor System"); }
		void Update() override;
		void Destroy() override;

	};
}
