#include "CPPVFXManager.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include <functional>
#include <random>
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
		GameObject* explosionClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("VFX Object", *explosionClone);
		explosionClone->GetComponent<CPTransform>()->SetPosition(pos);
		explosionClone->GetComponent<CPTransform>()->SetScale(Vec2<float>(5,5));
		explosionClone->GetComponent<CPAnimator>()->Play("VFX_Explosion");
		GOMANAGER->RemoveGameObject(explosionClone, 2.f);
	}
	void CPPSVFXManager::SpawnHitAnim(Vec2<float> pos)
	{
		GameObject* hitFXClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("VFX Object", *hitFXClone);
		hitFXClone->GetComponent<CPTransform>()->SetPosition(pos);
		hitFXClone->GetComponent<CPAnimator>()->Play("VFX_Hit");
		//Setting the scale UNIFORMLY
		std::mt19937 generator(TIME->GetTime());
		std::uniform_real_distribution<float> uniform_distribution(-0.5f, 0.5f);
		auto rngScaleOffset = std::bind(uniform_distribution, generator);
		Vec2<float> randomScale(1+rngScaleOffset(),1+rngScaleOffset());

		//Setting the rotation randomly
		int rotOffset = (rand() % 360);
		hitFXClone->GetComponent<CPTransform>()->SetScale(randomScale);
		hitFXClone->GetComponent<CPTransform>()->SetRotation(rotOffset);
		GOMANAGER->RemoveGameObject(hitFXClone, 2.f);
	}
	void CPPSVFXManager::SpawnPoofAnim(Vec2<float> pos, float scaleMult)
	{
		GameObject* poofClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("VFX Object", *poofClone);
		poofClone->GetComponent<CPTransform>()->SetPosition(pos);
		poofClone->GetComponent<CPTransform>()->SetScale(Vec2<float>(1, 1)*scaleMult);
		poofClone->GetComponent<CPAnimator>()->Play("VFX_Poof");
		GOMANAGER->RemoveGameObject(poofClone, 2.f);

	}
}