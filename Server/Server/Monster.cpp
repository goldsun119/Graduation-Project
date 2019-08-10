#include "Monster.h"

Monster::Monster()
{

}

int Monster::GetHp()
{
	return hp;
}

void Monster::SetHp(int a)
{
	hp = a;
}

vec3 Monster::GetRotation()
{
	return rotation;
}

void Monster::SetRotation(vec3 a)
{
	rotation = a;
}

vec3 Monster::GetInitPos()
{
	return initPos;
}

void Monster::SetInitPos(vec3 a)
{
	initPos = a;
}

int Monster::GetAnimator() {
	return animator;
}
void Monster::SetAnimator(int a) {
	animator = a;
}

float Monster::GetDirX() {
	return dirX;
}
void Monster::SetDirX(float a) {
	dirX = a;
}

float Monster::GetDirZ() {
	return dirZ;
}
void Monster::SetDirZ(float a) {
	dirZ = a;
}

int Monster::GetId()
{
	return id;
}

void Monster::SetId(int a)
{
	id = a;
}

int Monster::GetTarget()
{
	return target;
}

void Monster::SetTarget(int a)
{
	target = a;
}

int Monster::GetCalculate()
{
	return calculate;
}

void Monster::SetCalculate(int a)
{
	calculate = a;
}


Monster::~Monster()
{

}