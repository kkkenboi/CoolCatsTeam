
#pragma once

#include "App/Core/ISystem.h"

namespace LB
{
	class Serializer : public ISystem
	{
	public:
		void Initialize() override;

		void Shutdown() override;
	};

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Serializer>("Serializer")
			.method("Initialize", &Serializer::Initialize)
			.method("Shutdown", &Serializer::Shutdown);
	}
}
