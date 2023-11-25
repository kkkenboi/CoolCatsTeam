#include "CPPSPlayerGolfBall.h"

#include "LitterBox/Debugging/Debug.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Physics/PhysicsMath.h"
#include "LitterBox/Audio/AudioManager.h"
#include "LitterBox/Engine/Time.h"
#include "CPPSPlayer.h"

namespace LB
{
	void CPPSPlayerGolfBall::Start()
	{
		mRender = GameObj->GetComponent<CPRender>();
		mRigidBody = GameObj->GetComponent<CPRigidBody>();
		mCollider = GameObj->GetComponent<CPCollider>();

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

		mCurrentLifetime = mLifetime = 1.0f;
	}

	void CPPSPlayerGolfBall::Update()
	{
		if (!mRigidBody)
		{
			mRigidBody = GameObj->GetComponent<CPRigidBody>();
			return;
		}

		if (mRigidBody->mVelocity.LengthSquared() < 50.0f)
		{
			mCurrentLifetime -= TIME->GetDeltaTime();
			if (mCurrentLifetime <= 0.0f)
			{
				CPPSPlayer* player = (CPPSPlayer*)mPlayer->GetComponent<CPScriptCPP>()->GetInstance();
				--player->m_currentBalls;
				GOMANAGER->RemoveGameObject(this->GameObj);
			}
		}
	}

	void CPPSPlayerGolfBall::OnCollisionEnter(CollisionData colData)
	{
		if (colData.colliderOther->m_gameobj->GetName() == "Mage" ||
			colData.colliderOther->m_gameobj->GetName() == "EnemyChaser1")
		{
			int Channel = AUDIOMANAGER->PlaySound("Smoke Poof by sushiman2000 Id - 643876");

			AUDIOMANAGER->SetChannelVolume(Channel, 0.3f);

			CPPSPlayer* player = (CPPSPlayer*)mPlayer->GetComponent<CPScriptCPP>()->GetInstance();
			--player->m_currentBalls;
			GOMANAGER->RemoveGameObject(this->GameObj);
		}

	}

	void CPPSPlayerGolfBall::Destroy()
	{

	}

	//Getter functions
	/*!***********************************************************************
	\brief
	Getter for the render component
	*************************************************************************/
	CPRender* CPPSPlayerGolfBall::GetRender()
	{
		return mRender;
	}
	/*!***********************************************************************
	\brief
	Getter for the rigidbody component
	*************************************************************************/
	CPRigidBody* CPPSPlayerGolfBall::GetRigidBody()
	{
		return mRigidBody;
	}

	/*!***********************************************************************
	\brief
	Getter for the collider component
	*************************************************************************/
	CPCollider* CPPSPlayerGolfBall::GetCollider()
	{
		return mCollider;
	}

	/*!***********************************************************************
	\brief
	Getter for the player object
	*************************************************************************/
	GameObject* CPPSPlayerGolfBall::GetHero()
	{
		return mPlayer;
	}
}

