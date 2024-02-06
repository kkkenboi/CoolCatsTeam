#include "ParticleSystem.h"

namespace LB
{

	void ParticleManager::Initialize() 
	{
		SetSystemName("Particle System");
		std::cout << "Particle System Initialized\n";
		mEmitterPoolIndex = mEmitterPool.size() - 1;
		mParticlePoolIndex = mParticlePool.size() - 1;

		const EmitterType EmitterTypeVector[] = { TRAIL, RADIAL };
		for (EmitterType emit : EmitterTypeVector)
		{
			mEmitterTypes.emplace_back(std::make_pair(ParticleManager::Instance()->GetEmitterType(emit), emit));
		}
	}

	// Spawns a particle from an emitters configs
	void ParticleManager::Emit(CPParticle emitter) 
	{
		//std::cout << "Emitting\n";
		// Gets a particle from the particle pool to instantiate
		Particle& particle = mParticlePool[mParticlePoolIndex];
		particle.mIsActive = true;
		particle.mPosition = emitter.mEmitterPos;
		particle.mRotation = 0.f;

		// Velocity
		particle.mVelocity = emitter.mEmitterVelocity;
		particle.mVelocity.x += RandomRange(emitter.mEmitterVariationMinX, emitter.mEmitterVariationMaxX);
		particle.mVelocity.y += RandomRange(emitter.mEmitterVariationMinY, emitter.mEmitterVariationMaxY);

		// Texture
		 
		// Lifetime
		particle.mLifetime = emitter.mEmitterLifetime;
		particle.mLifetimeRemaining = emitter.mEmitterLifetime;

		// Size
		particle.mSizeBegin = emitter.mEmitterSizeBegin;
		particle.mSize = particle.mSizeBegin;
		particle.mSizeEnd = emitter.mEmitterSizeEnd;
	
		mParticlePoolIndex = (mParticlePoolIndex - 1 + mParticlePool.size()) % mParticlePool.size();
		
		// Create a GameObject that follows the Particle's current stats
		particle.mGameObj = FACTORY->SpawnGameObject();
		particle.mGameObj->GetComponent<CPTransform>()->SetPosition(particle.mPosition);
		particle.mGameObj->AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
		particle.mGameObj->GetComponent<CPRender>()->Initialise();
		// Get the texture ID from the emitter
		int textureID = emitter.mRender->texture;
		std::string textureName = ASSETMANAGER->GetTextureName(textureID);
		//std::cout << emitter.mRender->w << '\n';
		//std::cout << emitter.mRender->h << '\n';
		//std::cout << ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second << std::endl;
		//std::cout << ASSETMANAGER->GetTextureName(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second) << std::endl;
		particle.mGameObj->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second, static_cast<int>(emitter.mRender->w), static_cast<int>(emitter.mRender->h));

	}


	void ParticleManager::Update()
	{
		if (!TIME->IsPaused() && CORE->IsPlaying())
		{
			UpdateParticles();
			UpdateEmitters();
		}
	}

	void ParticleManager::Destroy() 
	{
		// Loop through the EmitterPool and make them all nullptr
		for (int i = 0; i < mEmitterPool.size(); ++i) 
		{
			mEmitterPool[i] = nullptr;
		}
	}

	std::string ParticleManager::GetEmitterType(EmitterType EmitType)
	{
		switch (EmitType)
		{
		case TRAIL:
			return "Trail";
		case RADIAL:
			return "Radial";
		default:
			return "ERROR!";
		}
	}

	std::vector<std::pair<std::string, EmitterType>>& ParticleManager::GetVectorEmits() {
		return mEmitterTypes;
	}

	void ParticleManager::AddEmitter(CPParticle* newEmitter) {
		// Loop through the mEmitterPool looking for the first nullptr and place the brand new
		// created emitter here
		for (int i = 0; i < mEmitterPool.size(); ++i) {
			if (mEmitterPool[i] == nullptr) {
				mEmitterPool[i] = newEmitter;
				return;
			}
		}
	}

	void ParticleManager::RemoveEmitter(CPParticle* findEmitter) {
		for (int i = 0; i < mEmitterPool.size(); ++i) {
			if (mEmitterPool[i] == findEmitter) {
				mEmitterPool[i] = nullptr;
			}
		}
	}

	void ParticleManager::UpdateParticles() {
		for (Particle& particle : mParticlePool)
		{
			if (!particle.mIsActive)
			{
				continue;
			}

			if (particle.mLifetimeRemaining <= 0.f)
			{
				particle.mIsActive = false;
				if (particle.mGameObj != nullptr)
				{
					GOMANAGER->RemoveGameObject(particle.mGameObj);
				}
				particle.mGameObj = nullptr;
				continue;
			}

			// Update the particle stats
			particle.mLifetimeRemaining -= TIME->GetDeltaTime();
			particle.mPosition += particle.mVelocity * TIME->GetDeltaTime();
			particle.mRotation += 0.01f * TIME->GetDeltaTime();

			// Check if size is enlarging or getting smaller
			if (particle.mSizeBegin > particle.mSizeEnd) {
				// Getting smaller
				if (particle.mSize > particle.mSizeEnd) {
					particle.mSize *= 0.9f;
				}
			}
			// Getting larger
			else if (particle.mSizeBegin < particle.mSizeEnd) {
				if (particle.mSize < particle.mSizeEnd) {
					particle.mSize *= 1.1f;
				}
			}

			// Update the GameObject itself
			if (particle.mGameObj != nullptr && particle.mGameObj->IsActive() == true) {
				particle.mGameObj->GetComponent<CPTransform>()->SetPosition(particle.mPosition);
				particle.mGameObj->GetComponent<CPTransform>()->SetRotation(particle.mRotation);
				particle.mGameObj->GetComponent<CPTransform>()->SetScale(Vec2<float>{particle.mSize, particle.mSize});
			}
		}
	}

	void ParticleManager::UpdateEmitters() 
	{
		for (CPParticle* emitter : mEmitterPool) 
		{
			if (emitter != nullptr)
			{
				//std::cout << "Here!\n";
				emitter->Update();
			}
		}
	}

	float RandomRange(float min, float max) {
		return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max - min);
	}

}