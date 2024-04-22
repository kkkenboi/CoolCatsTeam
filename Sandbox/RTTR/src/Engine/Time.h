
#pragma once

#include "rttr/registration.h"
#include "Core/ISystem.h"

namespace LB
{
	class Time : public ISystem
	{
	public:
		void Initialize() override;

		void Shutdown() override;
	};

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Time>("Time")
			.method("Initialize", &Time::Initialize)
			.method("Shutdown", &Time::Shutdown);
	}
}