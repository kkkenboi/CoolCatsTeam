#include "ParticleComponent.h"
#include "Litterbox/Animation/ParticleSystem.h"

namespace LB {

	void CPParticle::Initialise() 
	{
		mTransform = gameObj->GetComponent<CPTransform>();

		if (gameObj->HasComponent<CPRender>()) 
		{
			mRender = gameObj->GetComponent<CPRender>();
		}

		mEmitterType = TRAIL;
		
		mEmitterPos = mTransform->GetPosition();
		mEmitterRate = 0.5f;

		mEmitterVelocity = Vec2<float>{ 0.f, 0.f };
		float mEmitterVariationMin = 0.f;
		float mEmitterVariationMax = 0.f;

		mEmitterSizeBegin = 1.f;
		mEmitterSizeEnd = 1.f;

		mEmitterLifetime = 1.f;
		mEmitterLifetimeRemaining = 1.f;

		mIsActive = false;

		ParticleManager::Instance()->AddEmitter(this);
	}

	void CPParticle::Update()
	{
		if (gameObj->HasComponent<CPRender>())
		{
			mRender = gameObj->GetComponent<CPRender>();
		}

		//std::cout << "Testing\n";

		mEmitterPos = mTransform->GetPosition();
		
		if (mEmitterLifetimeRemaining > 0.f)
		{
			mEmitterLifetimeRemaining -= TIME->GetDeltaTime();
		}

		if (mEmitterLifetimeRemaining <= 0.f) 
		{
			mIsActive = false;
			mEmitterLifetimeRemaining = 0.f;
		}

		if (mIsActive)
		{
			ParticleManager::Instance()->Emit(*this);
		}
	}

	void CPParticle::Destroy()
	{

	}

	ComponentTypeID CPParticle::GetType() 
	{
		return C_CPParticle;
	}

	std::string CPParticle::GetEmitterType()
	{
		switch (mEmitterType)
		{
		case TRAIL:
			return "Trail";
		case RADIAL:
			return "Radial";
		default:
			return "ERROR!";
		}
	}
}