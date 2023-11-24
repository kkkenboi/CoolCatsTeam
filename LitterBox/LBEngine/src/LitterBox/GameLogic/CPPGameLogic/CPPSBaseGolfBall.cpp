#include "CPPSBaseGolfBall.h"

#include "LitterBox/Debugging/Debug.h"
#include "LitterBox/Factory/GameObjectManager.h"

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

