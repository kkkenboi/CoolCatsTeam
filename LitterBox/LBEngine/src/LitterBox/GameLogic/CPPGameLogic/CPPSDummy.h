#pragma once
#include "CPPSBaseEnemy.h"
#include "LitterBox/GameLogic/StateMachine.h"

namespace LB
{
	class CPPSDummy : public CPPSBaseEnemy
	{
	public:
		/*!***********************************************************************
		\brief
		Inherited Functions from CPPBehaviour that the user must implement
		*************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;
		void Hurt() override;

		void OnCollisionEnter(CollisionData colData) override;
		void Die() override;
	};
	REGISTER_SCRIPT(CPPSDummy)
}