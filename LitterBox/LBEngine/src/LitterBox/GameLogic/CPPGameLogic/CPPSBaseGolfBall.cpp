#include "CPPSBaseGolfBall.h"

#include "LitterBox/Debugging/Debug.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Physics/PhysicsMath.h"
#include "LitterBox/Audio/AudioManager.h"

namespace LB
{
	void CPPSBaseGolfBall::Start()
	{
		if (GameObj->HasComponent<CPRender>())
		{
			mRender = GameObj->GetComponent<CPRender>();
		}
		else
		{
			mRender = nullptr;
			return;
		}
		if (GameObj->HasComponent<CPRigidBody>())
		{
			mRigidBody = GameObj->GetComponent<CPRigidBody>();
		}
		else
		{
			mRigidBody = nullptr;
			return;
		}
		if (GameObj->HasComponent<CPCollider>())
		{
			mCollider = GameObj->GetComponent<CPCollider>();
		}
		else
		{
			mCollider = nullptr;
			return;
		}

		std::vector<GameObject*> const& GOs = GOMANAGER->GetGameObjects();
		for (GameObject* GO : GOs) {
			if (GO->GetName() == "MainChar") 
			{
				mPlayer = GO;
				break;
			}
		}

		mSpeedMagnitude = 1000.0f;
		mVelocity = 1000.0f; //with direction
		mSize = 1.0f;
	}

	void CPPSBaseGolfBall::Update()
	{
		
	}

	void CPPSBaseGolfBall::OnCollisionEnter(CollisionData colData)
	{
		if (colData.colliderOther->m_gameobj->GetName() == "MainChar" ||
			colData.colliderOther->m_gameobj->GetName() == "NorthWall" ||
			colData.colliderOther->m_gameobj->GetName() == "SouthWall" ||
			colData.colliderOther->m_gameobj->GetName() == "WestWall" ||
			colData.colliderOther->m_gameobj->GetName() == "EastWall")
		{
			
			int Channel = AUDIOMANAGER->PlaySound("Smoke Poof by sushiman2000 Id - 643876");
			
			AUDIOMANAGER->SetChannelVolume(Channel, 0.3f);

			GOMANAGER->RemoveGameObject(this->GameObj);
		}
		
	}

	void CPPSBaseGolfBall::Destroy()
	{

	}

	//Getter functions
	/*!***********************************************************************
	\brief
	Getter for the render component
	*************************************************************************/
	CPRender* CPPSBaseGolfBall::GetRender()
	{
		return mRender;
	}
	/*!***********************************************************************
	\brief
	Getter for the rigidbody component
	*************************************************************************/
	CPRigidBody* CPPSBaseGolfBall::GetRigidBody()
	{
		return mRigidBody;
	}

	/*!***********************************************************************
	\brief
	Getter for the collider component
	*************************************************************************/
	CPCollider* CPPSBaseGolfBall::GetCollider()
	{
		return mCollider;
	}

	/*!***********************************************************************
	\brief
	Getter for the player object
	*************************************************************************/
	GameObject* CPPSBaseGolfBall::GetHero()
	{
		return mPlayer;
	}
}

