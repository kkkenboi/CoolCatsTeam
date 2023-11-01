#include "Chaser.h"

void Chaser::Initialise() 
{
	this->chaserObj = LB::FACTORY->SpawnGameObject({ LB::C_CPRender, LB::C_CPRigidBody, LB::C_CPCollider });
	this->chaserObj->GetComponent<LB::CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureIndex("cat"));
	this->chaserObj->GetComponent<LB::CPTransform>()->SetPosition(LB::Vec2<float>{1200.f, 800.f});


	ChaserIdleState* CHASERIDLE = DBG_NEW ChaserIdleState(mChaserFSM, "IDLE");
	ChaserChaseState* CHASERCHASE = DBG_NEW ChaserChaseState(mChaserFSM, "CHASE");

	mChaserFSM.AddState(CHASERIDLE);
	mChaserFSM.AddState(CHASERCHASE);

	
}

void Chaser::Update() 
{

}

void Chaser::Destroy()
{
	delete mChaserFSM.GetState("IDLE");
	delete mChaserFSM.GetState("CHASE");
}