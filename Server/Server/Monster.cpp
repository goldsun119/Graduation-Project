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

Monster::~Monster()
{

}