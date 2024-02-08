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
		mEmitterRate = 0.05f;

		mEmitterVelocity = Vec2<float>{ 0.f, 0.f };
		float mEmitterVariationMinX = 0.f;
		float mEmitterVariationMaxX = 0.f;
		float mEmitterVariationMinY = 0.f;
		float mEmitterVariationMaxY = 0.f;

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
		
		if (mEmitterLifetimeRemaining > 0.f && mIsActive)
		{
			mEmitterLifetimeRemaining -= TIME->GetDeltaTime();
			mTimeSinceLastEmit += TIME->GetDeltaTime();
		}

		if (mEmitterLifetimeRemaining <= 0.f) 
		{
			mIsActive = false;
			mEmitterLifetimeRemaining = mEmitterLifetime;
		}

		if (mIsActive)
		{
			if (mTimeSinceLastEmit >= mEmitterRate) {
				if (mRender != nullptr) {
					ParticleManager::Instance()->Emit(*this);
				}
				mTimeSinceLastEmit = 0.f;
			}
		}
	}

	void CPParticle::Destroy()
	{
		// Remove it from the EmitterPool
		mTransform = nullptr;
		mRender = nullptr;
		ParticleManager::Instance()->RemoveEmitter(this);
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

	void CPParticle::RestartLifetime() 
	{
		mEmitterLifetimeRemaining = mEmitterLifetime;
	}
}