#pragma once
#include "Obj.h"
class Monster : public Obj
{
private:
	vec3 rotation;
	float dirZ;
	float dirX;
	int animator;
	int hp;
	int id;

public:
	Monster();
	~Monster();

	int GetHp();
	void SetHp(int a);

	vec3 GetRotation();
	void SetRotation(vec3 a);

	int GetAnimator();
	void SetAnimator(int a);

	float GetDirX();
	void SetDirX(float a);

	float GetDirZ();
	void SetDirZ(float a);

	int GetId();
};