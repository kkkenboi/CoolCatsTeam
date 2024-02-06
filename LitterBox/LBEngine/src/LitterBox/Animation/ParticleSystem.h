#include "LitterBox/Utils/Math.h"
#include "LitterBox/Components/ParticleComponent.h"
#include "LitterBox/Components/RenderComponent.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include <cstdlib>
#include <ctime>
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Core/Core.h"

#pragma once

namespace LB {

	class Particle {
	public:
		Vec2<float> mPosition;
		Vec2<float> mVelocity;
		float mRotation;

		float mSize;
		float mSizeBegin;
		float mSizeEnd;

		float mLifetime;
		float mLifetimeRemaining;


		GameObject* mGameObj;

		bool mIsActive;
	};

	class ParticleManager : public ISystem, public Singleton<ParticleManager>
	{
	public:

		// ISystem stuff
		void Initialize() override;
		void Update() override;
		void Destroy() override;

		void Emit(CPParticle emitter);
		std::vector<std::pair<std::string, EmitterType>>& GetVectorEmits();
		std::string GetEmitterType(EmitterType EmitType);
		void AddEmitter(CPParticle* emitter);
		void RemoveEmitter(CPParticle* emitter);
		void UpdateParticles();
		void UpdateEmitters();
	private:

		// Postion, Velocity, Variation, Texture, SizeBegin

		std::array<CPParticle*, 100> mEmitterPool;
		std::array<Particle, 1000> mParticlePool;
		int mEmitterPoolIndex;
		int mParticlePoolIndex;

		std::vector<std::pair<std::string, EmitterType>> mEmitterTypes;
	};

	
	float RandomRange(float min, float max);
}
