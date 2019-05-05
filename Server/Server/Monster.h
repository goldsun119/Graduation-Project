#pragma once
#include "Obj.h"
class Monster : public Obj
{
private:
	Vec3 rotation;
	float vertical;
	float horizontal;
	int animator;
	int hp;
	int id;

public:
	Monster();
	~Monster();
};