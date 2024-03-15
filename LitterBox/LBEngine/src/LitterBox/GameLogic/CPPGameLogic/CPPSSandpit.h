#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

#pragma once
namespace LB
{
	class CPPSSandpit : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Start function where variables will be initialised
		*************************************************************************/
		void Start() override;
		/*!***********************************************************************
		\brief
		Update function where the mushroom's scale is being changed
		*************************************************************************/
		void Update() override;
		/*!***********************************************************************
		\brief
		Overriden destroy function because of inheritance
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		\brief
		Every time the mushroom collides with anything it's scale changes
		*************************************************************************/
		void OnCollisionEnter(CollisionData colData);
		void OnCollisionStay(CollisionData colData);
		void OnCollisionExit(CollisionData colData);
	private:
		GameObject* mPlayer{ nullptr };
	};

	REGISTER_SCRIPT(CPPSSandpit)
}