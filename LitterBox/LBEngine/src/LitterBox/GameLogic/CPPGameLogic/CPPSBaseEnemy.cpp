#include "pch.h"
#include "CPPSBaseEnemy.h"
#include "CPPGameManager.h"
#include "LitterBox/Engine/Input.h"

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
		//Grabbing GameManager reference
		for (GameObject* GO : GOs)
		{
			if (GO->GetName() == "GameManager")
			{
				mGameManager = GO;
				break;
			}
		}

		rightFace = GameObj->GetComponent<CPTransform>()->GetScale();
		leftFace = GameObj->GetComponent<CPTransform>()->GetScale();
		leftFace.x = -leftFace.x;
	}
	void CPPSBaseEnemy::Update()
	{
		//All enemies must always face player (?) Might not work for charger who knows
		DirToPlayer = mPlayer->GetComponent<CPTransform>()->GetPosition() - GameObj->GetComponent<CPTransform>()->GetPosition();
		if (DotProduct(DirToPlayer.Normalise(), TransformRight) < 0.0f)
		{
			GameObj->GetComponent<CPTransform>()->SetScale(leftFace);
		}
		else GameObj->GetComponent<CPTransform>()->SetScale(rightFace);

		//All enemies will sepuku if you press K
		if (INPUT->IsKeyTriggered(KeyCode::KEY_K))
		{
			Die();
		}
		
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
	void CPPSBaseEnemy::Die()
	{
		//If the enemy dies, regardless we MUST reduce the enemy count
		mGameManager->GetComponent<CPPSGameManager>()->ReduceEnemyCount();
		GameObj->GetComponent<CPTransform>()->SetPosition(Vec2<float>{0.0f, 10000.0f});
		GameObj->RemoveComponent(C_CPCollider);
		mShouldDestroy = true;
	}
}
