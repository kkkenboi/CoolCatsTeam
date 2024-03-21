#include "AudioListenerComponent.h"
#include "AudioManager.h"
namespace LB
{
	void LB::CPAudioListener::Initialise()
	{
		AUDIOMANAGER->AudioListener = this;
		transform = GetComponent<CPTransform>();
	}
	void CPAudioListener::Update()
	{	//We need to convert our vector to an FMOD_VECTOR for it to be used 
		FMOD_VECTOR pos = { transform->GetPosition().x, transform->GetPosition().y};
		//Then we just constantly update the position of the listener
		AUDIOMANAGER->audioSystem->set3DListenerAttributes(0, &pos, NULL, NULL, NULL);
	}
	void CPAudioListener::Destroy()
	{
	}
	bool CPAudioListener::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		data.AddMember("isActive", m_active, alloc);
		return true;
	}
	bool CPAudioListener::Deserialize(const Value& data)
	{
		bool HasActive = data.HasMember("isActive");
		if (HasActive)
		{
			m_active = data["isActive"].GetBool();
			return true;
		}
		return false;
	}
}