#pragma once

#include "LitterBox.h"
using namespace LB;

class Player {
public:
	GameObject* playerObj;

public:
	void Initialise();
	void Update();

};

void MoveUp();
void MoveLeft();
void MoveRight();
void MoveDown();

extern GameObject* PlayerObj;