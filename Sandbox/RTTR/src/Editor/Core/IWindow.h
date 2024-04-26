
#pragma once

#include "rttr/registration.h"

namespace LB
{
	class IWindow
	{
	public:
		virtual void Initialize() = 0;

		virtual void Update() = 0;

		virtual void Shutdown() = 0;
	};
}
