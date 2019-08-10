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
	int target;
	int calculate;
	vec3 initPos;

public:
	Monster();
	~Monster();

	int GetHp();
	void SetHp(int a);

	vec3 GetRotation();
	void SetRotation(vec3 a);

	vec3 GetInitPos();
	void SetInitPos(vec3 a);

	int GetAnimator();
	void SetAnimator(int a);

	float GetDirX();
	void SetDirX(float a);

	float GetDirZ();
	void SetDirZ(float a);

	int GetId();
	void SetId(int a);


	int GetTarget();
	void SetTarget(int a);

	int GetCalculate();
	void SetCalculate(int a);

	float GetInitXPos() const { return initPos.x; }
	float GetInitYPos() const { return initPos.y; }
	float GetInitZPos() const { return initPos.z; }

	void SetInitPos(float x, float y, float z) { initPos.x = x, initPos.y = y, initPos.z = z; }
	void SetInitXPos(float x) { initPos.x = x; }
	void SetInitYPos(float y) { initPos.y = y; }
	void SetInitZPos(float z) { initPos.z = z; }
};