#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class PauseQuitButton : public CPPBehaviour
	{
	public:
		void Start() override;
		void Update() override;
		void Destroy() override;

		CPCollider* GetCollider();
	private:
		CPCollider* mCollider;
	};

}