#include "CPPSTutorial1.h"

namespace LB
{
	void CPPSTutorial1::Start()
	{
	
	}

	void CPPSTutorial1::Update()
	{

	}

	void CPPSTutorial1::Destroy()
	{

	}

	void CPPSTutorial1::OnCollisionEnter(CollisionData colData)
	{
		if (colData.colliderOther->gameObj->GetName() == "MainChar")
		{
			collided = true;
		}
	}
}