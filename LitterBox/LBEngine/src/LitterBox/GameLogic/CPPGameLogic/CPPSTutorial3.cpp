#include "CPPSTutorial3.h"

namespace LB
{
	void CPPSTutorial3::Start()
	{

	}

	void CPPSTutorial3::Update()
	{

	}

	void CPPSTutorial3::Destroy()
	{

	}

	void CPPSTutorial3::OnCollisionEnter(CollisionData colData)
	{
		if (colData.colliderOther->gameObj->GetName() == "MainChar")
		{
			collided = true;
		}
	}
}