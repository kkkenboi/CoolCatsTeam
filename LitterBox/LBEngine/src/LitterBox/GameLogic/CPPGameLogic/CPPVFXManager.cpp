#include "CPPVFXManager.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Factory/GameObjectFactory.h"
namespace LB
{
	void CPPSVFXManager::Start()
	{
		
	}
	void CPPSVFXManager::Update()
	{
		
	}
	void CPPSVFXManager::Destroy()
	{
	}
	//Spawn functions that spawn the GO then removes them after 2 seconds
	void CPPSVFXManager::SpawnExplosion(Vec2<float> pos)
	{
		GameObject* explosionClone = FACTORY->SpawnGameObject(pos);
		JSONSerializer::DeserializeFromFile("VFX Object", *explosionClone);
		explosionClone->GetComponent<CPTransform>()->SetPosition(pos);
		explosionClone->GetComponent<CPAnimator>()->Play("Explosion");
		GOMANAGER->RemoveGameObject(explosionClone, 2.f);
	}
	void CPPSVFXManager::SpawnHitAnim(Vec2<float> pos)
	{
		GameObject* hitFXClone = FACTORY->SpawnGameObject(pos);
		JSONSerializer::DeserializeFromFile("VFX Object", *hitFXClone);
		hitFXClone->GetComponent<CPTransform>()->SetPosition(pos);

		hitFXClone->GetComponent<CPAnimator>()->Play("Hit");
		GOMANAGER->RemoveGameObject(hitFXClone, 2.f);


	}
	void CPPSVFXManager::SpawnPoofAnim(Vec2<float> pos)
	{
		GameObject* poofClone = FACTORY->SpawnGameObject(pos);
		JSONSerializer::DeserializeFromFile("VFX Object", *poofClone);
		poofClone->GetComponent<CPTransform>()->SetPosition(pos);

		poofClone->GetComponent<CPAnimator>()->Play("Poof");
		GOMANAGER->RemoveGameObject(poofClone, 2.f);

	}
}