#include "ParticleSystem.h"

namespace LB
{
	ParticleManager* PARTICLES = nullptr;

	void ParticleManager::Initialize() 
	{
		SetSystemName("Particle System");

		mEmitterPoolIndex = mEmitterPool.size();
		mParticlePoolIndex = mParticlePool.size();
	}

	// Spawns a particle from an emitters configs
	void ParticleManager::Emit(CPParticle emitter) 
	{
		
		// Gets a particle from the particle pool to instantiate
		std::pair<Particle, GameObject*>& particle = mParticlePool[mParticlePoolIndex];
		particle.first.mIsActive = true;
		particle.first.mPosition = emitter.mEmitterPos;
		particle.first.mRotation = 0.f;

		// Velocity
		particle.first.mVelocity = emitter.mEmitterVelocity;
		particle.first.mVelocity.x += RandomRange(emitter.mEmitterVariationMin, emitter.mEmitterVariationMax);
		particle.first.mVelocity.y += RandomRange(emitter.mEmitterVariationMin, emitter.mEmitterVariationMax);

		// Texture
		 
		// Lifetime
		particle.first.mLifetime = emitter.mEmitterLifetime;
		particle.first.mLifetimeRemaining = emitter.mEmitterLifetime;

		// Size
		particle.first.mSizeBegin = emitter.mEmitterSizeBegin;
		particle.first.mSize = particle.first.mSizeBegin;
		particle.first.mSizeEnd = emitter.mEmitterSizeEnd;
	
		mParticlePoolIndex = --mParticlePoolIndex % mParticlePool.size();
		
		// Create a GameObject that follows the Particle's current stats
		particle.second = FACTORY->SpawnGameObject();
		particle.second->AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
		// Get the texture ID from the emitter
		int textureID = emitter.mRender->texture;
		std::string textureName = ASSETMANAGER->GetTextureName(textureID);
		particle.second->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->GetTextureUnit(textureName), static_cast<int>(emitter.mRender->w), static_cast<int>(emitter.mRender->h));
	}


	void ParticleManager::Update()
	{
		for (std::pair<Particle,GameObject*>& particle : mParticlePool) 
		{
			if (!particle.first.mIsActive)
			{
				continue;
			}

			if (particle.first.mLifetimeRemaining <= 0.f) 
			{
				particle.first.mIsActive = false;
				GOMANAGER->RemoveGameObject(particle.second);
				continue;
			}

			// Update the particle stats
			particle.first.mLifetimeRemaining -= TIME->GetDeltaTime();
			particle.first.mPosition += particle.first.mVelocity * TIME->GetDeltaTime();
			particle.first.mRotation += 0.01f * TIME->GetDeltaTime();
			
			// Check if size is enlarging or getting smaller
			if (particle.first.mSizeBegin > particle.first.mSizeEnd) {
				// Getting smaller
				if (particle.first.mSize > particle.first.mSizeEnd) {
					particle.first.mSize *= 0.9f;
				}
			}
			// Getting larger
			else if (particle.first.mSizeBegin < particle.first.mSizeEnd) {
				if (particle.first.mSize < particle.first.mSizeEnd) {
					particle.first.mSize *= 1.1f;
				}
			}

			// Update the GameObject itself
			particle.second->GetComponent<CPTransform>()->SetPosition(particle.first.mPosition);
			particle.second->GetComponent<CPTransform>()->SetScale(Vec2<float>{particle.first.mSize, particle.first.mSize});
		}
	}

	void ParticleManager::Destroy() 
	{

	}

	float RandomRange(float min, float max) {
		return (min + static_cast<float>(rand())) / static_cast<float>(RAND_MAX / (max - min));
	}
}