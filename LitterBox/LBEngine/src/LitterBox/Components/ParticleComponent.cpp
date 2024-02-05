#include "ParticleComponent.h"

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

		mIsActive = false;
	}

	void CPParticle::Update()
	{
		if (gameObj->HasComponent<CPRender>())
		{
			mRender = gameObj->GetComponent<CPRender>();
		}

		mEmitterPos = mTransform->GetPosition();
		
	}

	void CPParticle::Destroy()
	{

	}

	ComponentTypeID CPParticle::GetType() 
	{
		return C_CPParticle;
	}
}