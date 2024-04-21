
#pragma once

namespace LB
{
	class ISystem
	{
	public:
		virtual int InitializePriority() const { return 0; }
		virtual void Initialize() = 0;

		virtual int FixedUpdatePriority() const { return 0; }
		virtual void FixedUpdate() = 0;

		virtual int UpdatePriority() const { return 0; }
		virtual void Update() = 0;

		virtual void Shutdown() = 0;
	};
}