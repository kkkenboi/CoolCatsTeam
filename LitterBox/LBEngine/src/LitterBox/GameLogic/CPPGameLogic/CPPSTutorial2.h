#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class CPPSTutorial2 : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Inherited Functions from CPPBehaviour that the user must implement
		*************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;

		void OnCollisionEnter(CollisionData colData) override;

		bool collided{ false };
	};
	REGISTER_SCRIPT(CPPSTutorial2)
}