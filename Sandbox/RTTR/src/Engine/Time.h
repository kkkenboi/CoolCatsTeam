
#pragma once

#include "rttr/registration.h"
#include "Core/ISystem.h"

namespace LB
{
	class Time : public ISystem
	{
	public:
		int InitializePriority() const override { return 10; }
		void Initialize() override;

		void FixedUpdate() override { };
		void Update() override { };

		void Shutdown() override;
	};

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Time>("Time")
			.method("Initialize", &Time::Initialize)
			.method("Shutdown", &Time::Shutdown);
	}
}