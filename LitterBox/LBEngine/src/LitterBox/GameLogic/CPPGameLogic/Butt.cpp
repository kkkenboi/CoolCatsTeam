#include "Butt.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Engine/Input.h"
#include "Platform/Editor/EditorGameView.h"

namespace LB {
	void Butt::Start()
	{
		coll = GameObj->GetComponent<CPCollider>();
	}
	void Butt::Update()
	{
		if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
		{
			auto test = COLLIDERS->OverlapCircle(EDITORGAMEVIEW->GetMousePos(), 1.f);

			for (const auto& collider : test) {
				if (coll == collider)
					DebuggerLog("UI BUTTON CLICKED");
			}
		}
	}
	void Butt::Destroy()
	{
	}
}