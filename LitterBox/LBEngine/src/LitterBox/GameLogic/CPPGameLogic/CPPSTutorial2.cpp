#include "CPPSTutorial2.h"

namespace LB
{

	void CPPSTutorial2::Start()
	{

	}

	void CPPSTutorial2::Update()
	{

	}

	void CPPSTutorial2::Destroy()
	{

	}

	void CPPSTutorial2::OnCollisionEnter(CollisionData colData)
	{
		if (colData.colliderOther->gameObj->GetName() == "MainChar")
		{
			collided = true;
		}
	}
}