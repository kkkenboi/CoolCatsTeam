#include "pch.h"
#include "CPPSBaseEnemy.h"

namespace LB
{
	void CPPSBaseEnemy::Start()
	{
		mRender = GameObj->GetComponent<CPRender>();
		mRigidBody = GameObj->GetComponent<CPRigidBody>();
		mCollider = GameObj->GetComponent<CPCollider>();
		//Grabbing the player GO reference
		std::vector<GameObject*> const& GOs = GOMANAGER->GetGameObjects();
		for (GameObject* GO : GOs) {
			if (GO->GetName() == "MainChar") {
				mPlayer = GO;
				break;
			}
		}

		rightFace = GameObj->GetComponent<CPTransform>()->GetScale();
		leftFace = GameObj->GetComponent<CPTransform>()->GetScale();
		leftFace.x = -leftFace.x;
	}
	void CPPSBaseEnemy::Update()
	{
		DirToPlayer = mPlayer->GetComponent<CPTransform>()->GetPosition() - GameObj->GetComponent<CPTransform>()->GetPosition();
		if (DotProduct(DirToPlayer.Normalise(), TransformRight) < 0.0f)
		{
			GameObj->GetComponent<CPTransform>()->SetScale(leftFace);
		}
		else GameObj->GetComponent<CPTransform>()->SetScale(rightFace);
	}
	void CPPSBaseEnemy::Destroy()
	{
		//Should be empty?
	}
	void CPPSBaseEnemy::OnCollisionEnter(CollisionData col)
	{
		//Should be empty??????
	}
	CPRender* CPPSBaseEnemy::GetRender()
	{
		return mRender;
	}
	CPRigidBody* CPPSBaseEnemy::GetRigidBody()
	{
		return mRigidBody;
	}
	CPCollider* CPPSBaseEnemy::GetCollider()
	{
		return mCollider;
	}
	GameObject* CPPSBaseEnemy::GetHero()
	{
		return mPlayer;
	}
	int& CPPSBaseEnemy::GetHealth()
	{
		return mHealth;
	}
	float& CPPSBaseEnemy::GetSpeedMag()
	{
		return mSpeedMagnitude;
	}
}
