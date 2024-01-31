#include "CPPSUpgrade.h"

namespace LB
{
	void CPPSUpgrade::Start()
	{
	}
	void CPPSUpgrade::Update()
	{
	}
	void CPPSUpgrade::Destroy()
	{
	}

	void CPPSUpgrade::OnCollisionEnter(CollisionData colData)
	{
		//make collision layer that only contains ball and upgrade interaction
		//If the upgrade collides with a ball
		if (colData.colliderOther->m_gameobj->GetName() == "ball")
		{
			std::cout << "Hit upgrade\n";
			this->GameObj->SetActive(false);
		}
	}
}