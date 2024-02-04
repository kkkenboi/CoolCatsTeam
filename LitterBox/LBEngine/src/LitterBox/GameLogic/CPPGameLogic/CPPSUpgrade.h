#pragma once
#include "CPPBehaviour.h"
#include "CPPSUpgradeManager.h"
namespace LB
{
	class CPPSUpgrade : public CPPBehaviour
	{
	public:
		void Start() override;
		void Update() override;
		void Destroy() override;

		void OnCollisionEnter(CollisionData);
		void AssignUpgradeID(int);

		int assignedUpgradeType{ 0 };
	private:
		GameObject* BallGameObj{ nullptr };
	};
	REGISTER_SCRIPT(CPPSUpgrade)
}

