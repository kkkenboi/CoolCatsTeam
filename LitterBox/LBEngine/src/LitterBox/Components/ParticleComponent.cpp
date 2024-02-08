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
		//mEmitterRate = 0.05f;

		//mEmitterVelocity = Vec2<float>{ 0.f, 0.f };
		//mEmitterVariationMinX = 0.f;
		//mEmitterVariationMaxX = 0.f;
		//mEmitterVariationMinY = 0.f;
		//mEmitterVariationMaxY = 0.f;

		//mEmitterRadialSpeed = 100.f;

		//mEmitterSizeBegin = 1.f;
		//mEmitterSizeEnd = 1.f;

		//mEmitterLifetime = 1.f;
		mEmitterLifetimeRemaining = mEmitterLifetime;

		//mParticleLifetime = 1.f;

		//mRadialParticles = 10;

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
			if (mIsLooping == false)
			{
				mEmitterLifetimeRemaining -= TIME->GetDeltaTime();
			}
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
					ParticleManager::Instance()->Emit(this);
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

	bool CPParticle::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		data.AddMember("EmitterType", static_cast<int>(mEmitterType), alloc);
		data.AddMember("EmitterRate", mEmitterRate, alloc);
		data.AddMember("RadialNum", mRadialParticles, alloc);
		Value VelocityValue;
		if (mEmitterVelocity.Serialize(VelocityValue, alloc))
		{
			data.AddMember("EmitterVelocity", VelocityValue, alloc);
		}
		data.AddMember("VelocityMinX", mEmitterVariationMinX, alloc);
		data.AddMember("VelocityMaxX", mEmitterVariationMaxX, alloc);
		data.AddMember("VelocityMinY", mEmitterVariationMinY, alloc);
		data.AddMember("VelocityMaxY", mEmitterVariationMaxY, alloc);
		data.AddMember("RadialSpeed", mEmitterRadialSpeed, alloc);
		data.AddMember("SizeBegin", mEmitterSizeBegin, alloc);
		data.AddMember("SizeEnd", mEmitterSizeEnd, alloc);
		data.AddMember("EmitterLifetime", mEmitterLifetime, alloc);
		data.AddMember("ParticleLifetime", mParticleLifetime, alloc);
		data.AddMember("EmitterActive", mIsActive, alloc);
		data.AddMember("EmitterLooping", mIsLooping, alloc);

		return true;
	}

	bool CPParticle::Deserialize(const Value& data)
	{
		mEmitterType = static_cast<EmitterType>(data["EmitterType"].GetInt());
		mEmitterRate = data["EmitterRate"].GetFloat();
		mRadialParticles = data["RadialNum"].GetInt();
		const Value& velocityValue = data["EmitterVelocity"];
		mEmitterVelocity.Deserialize(velocityValue);
		mEmitterVariationMinX = data["VelocityMinX"].GetFloat();
		mEmitterVariationMaxX = data["VelocityMaxX"].GetFloat();
		mEmitterVariationMinY = data["VelocityMinY"].GetFloat();
		mEmitterVariationMaxY = data["VelocityMaxY"].GetFloat();
		mEmitterRadialSpeed = data["RadialSpeed"].GetFloat();
		mEmitterSizeBegin = data["SizeBegin"].GetFloat();
		mEmitterSizeEnd = data["SizeEnd"].GetFloat();
		mEmitterLifetime = data["EmitterLifetime"].GetFloat();
		mParticleLifetime = data["ParticleLifetime"].GetFloat();
		mIsActive = data["EmitterActive"].GetBool();
		mIsLooping = data["EmitterLooping"].GetBool();

		return true;
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