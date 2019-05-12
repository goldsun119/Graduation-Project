#include "Player.h"

Player::Player()
{
	pos.x = 0.f;
	pos.y = 0.f;
	pos.z = 0.f;
	isDraw = false;
	name = "";
	type = OBJ_TYPE::PLAYER;
	rotation.x = 0.f;
	rotation.y = 0.f;
	rotation.z = 0.f;
	vertical = 0.f;
	horizontal = 0.f;
	animator = 0;
	hp = 0;
	id = 0;
}
void Player::init(int i)
{
	id = i;
	switch (id)
	{
	case 1:
		SetPos(-1, 0, -6);
		break;

	case 2:
		SetPos(1, 0, -6);
		break;

	case 3:
		SetPos(-1, 0, -7);
		break;

	case 4:
		SetPos(1, 0, -7);
		break;
	default:
		break;
	}
	hp = 100;
}


int Player::GetHp()
{
	return hp;
}

void Player::SetHp(int a)
{
	hp = a;
}

vec3 Player::GetRotation()
{
	return rotation;
}

void Player::SetRotation(vec3 a)
{
	rotation = a;
}

float Player::GetVertical()
{
	return vertical;
}
void Player::SetVertical(float a) {
	vertical = a;
}
	   
float Player::GetHorizontal() {
	return horizontal;
}
void Player::SetHorizontal(float a) {
	horizontal = a;
}

int Player::GetAnimator() {
	return animator;
}
void Player::SetAnimator(int a) {
	animator = a;
}

int Player::GetId()
{
	return id;
}

Player::~Player()
{

}