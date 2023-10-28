#include "Chaser.h"

void Chaser::Initialise() 
{
	this->chaserObj = LB::FACTORY->SpawnGameObject({ LB::C_CPRender, LB::C_CPRigidBody, LB::C_CPCollider });
	this->chaserObj->GetComponent<LB::CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureIndex("cat"));
	this->chaserObj->GetComponent<LB::CPTransform>()->SetPosition(LB::Vec2<float>{1200.f, 800.f});


	// Add States to the FSM of Chaser
	//mChaserFSM.AddState(ChaserStateType::IDLE);
	
}

void Chaser::Update() 
{

}