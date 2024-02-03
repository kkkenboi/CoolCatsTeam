#include "LitterBox/Utils/Math.h"
#pragma once

namespace LB {
	class Particle 
	{
	public:
	private:
		Vec2<float> mPosition;
		Vec2<float> mVelocity;
		float mRotation;
		
		float mSizeBegin;
		float mSizeEnd;

		float mLifetime = 1.0f;
		float mLifetimeRemain = 0.f;

		bool mIsActive = false;
	};

	class ParticleSystem 
	{
	public:
		void UpdateParticles();
		void RenderParticles();

	private:
		Vec2<float> mEmitterPos;
		float mEmitterRate;

		std::array<Particle*, 1000> mParticlePool;

	};

	extern ParticleSystem PARTICLES;
}