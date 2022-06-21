#pragma once
#include "Enemy.h"

class Enemy;

class BaseEnemyState
{
public:	
	virtual void Update() = 0;
private:
	Enemy* enemy_;
};

