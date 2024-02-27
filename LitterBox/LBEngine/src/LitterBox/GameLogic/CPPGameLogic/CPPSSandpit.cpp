#include "CPPSSandpit.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	Start function where variables will be initialised
	*************************************************************************/
	void CPPSSandpit::Start()
	{

	}

	/*!***********************************************************************
	\brief
	Update function where the mushroom's scale is being changed
	*************************************************************************/
	void CPPSSandpit::Update()
	{

	}

	/*!***********************************************************************
	\brief
	Overriden destroy function because of inheritance
	*************************************************************************/
	void CPPSSandpit::Destroy()
	{

	}

	/*!***********************************************************************
	\brief
	Every time the mushroom collides with anything it's scale changes
	*************************************************************************/

	void CPPSSandpit::OnCollisionEnter(CollisionData colData)
	{
		colData.colliderOther->gameObj->GetComponent<CPTransform>()->SetScale(Vec2<float>{2.f, 2.f});
	}

	void CPPSSandpit::OnCollisionStay(CollisionData colData)
	{
		if (colData.colliderOther->rigidbody != nullptr)
		{
			if (colData.colliderOther->gameObj->GetName() != "Projectile") 
			{
				colData.colliderOther->rigidbody->mVelocity.x *= 0.90f;
				colData.colliderOther->rigidbody->mVelocity.y *= 0.90f;
			}
		}
	}

	void CPPSSandpit::OnCollisionExit(CollisionData colData)
	{
		colData.colliderOther->gameObj->GetComponent<CPTransform>()->SetScale(Vec2<float>{1.f, 1.f});
	}
}