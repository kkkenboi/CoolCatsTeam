#include "CPPSSandpit.h"
#include "LitterBox/Animation/ParticleSystem.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	Start function where variables will be initialised
	*************************************************************************/
	void CPPSSandpit::Start()
	{
		mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
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
			if (colData.colliderOther->gameObj->GetName() == "MainChar") 
			{
				colData.colliderOther->rigidbody->mVelocity.x *= 0.5f;
				colData.colliderOther->rigidbody->mVelocity.y *= 0.5f;
			}
		}
	}

	void CPPSSandpit::OnCollisionExit(CollisionData colData)
	{
	}
}