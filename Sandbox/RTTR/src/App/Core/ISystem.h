
#pragma once

#include "rttr/registration.h"

namespace LB
{
	class ISystem
	{
	public:
		virtual void Initialize() = 0;

		virtual void Shutdown() = 0;
	};

	class IFixedUpdateSystem : public ISystem
	{
		virtual void FixedUpdate() = 0;
	};

	class IUpdateSystem : public ISystem
	{
		virtual void Update() = 0;
	};
}
