#pragma once
#include <LitterBox/Utils/Math.h>
#include <Litterbox/Engine/Message.h>
#include <Litterbox/Factory/GameObjectComposition.h>

namespace LB
{
	class GameLogic : public ISystem
	{
	public:
		GameLogic();
		~GameLogic();
		void Initialize();
		void Update(float timeslice);
		virtual std::string GetName() override { return "GameLogic"; }
		virtual void SendMessage(Message*) override;
		//GameObjectComposition* CreateObjectAt(Vec2& position, float rotation, const std::string& file);
		void LoadLevelFile(const std::string& file);
		unsigned GrabbedObjectID;
		//Vec2 WorldMousePosition;
	public:
		//ObjectLinkList<Controller> Controllers;
	};

	//A global pointer to our game so that it can be accessed from anywhere.
	extern GameLogic* LOGIC;

}
