
#pragma once

#include <string>

namespace LB
{
	class ISystem
	{
	public:
		virtual void Initialize() = 0;

		virtual void Shutdown() = 0;

		std::string m_Name{ "Unnamed System" };
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