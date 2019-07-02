#include "Player.h"

Player::Player()
{
	pos.x = 0.f;
	pos.y = 0.f;
	pos.z = 0.f;
	isDraw = false;
	name = "";
	rotation.x = 0.f;
	rotation.y = 0.f;
	rotation.z = 0.f;
	vertical = 0.f;
	horizontal = 0.f;
	animator = 0;
	dirX = 0.f;
	dirZ = 0.f;
	hp = 0;
	maxhp = 0;
	id = 0;
	
}
void Player::init(int i)
{
	id = i;
	switch (id)
	{
	case 1:
		SetPos(50, 50, 95);
		break;

	case 2:
		SetPos(55, 50, 100);
		break;

	case 3:
		SetPos(50, 50, 100);
		break;

	case 4:
		SetPos(55, 50, 95);
		break;
	default:
		break;
	}
	SetRotation(vec3{0, 0, 0});
	hp = 100;
	isDraw = true;
}


int Player::GetHp()
{
	return hp;
}

void Player::SetHp(int a)
{
	hp = a;
}

int Player::GetItem(int index)
{
	return item[index];
}

void Player::SetItem(int index, int a)
{
	item[index] = a;
}

int Player::GetMaxhp()
{
	return maxhp;
}

void Player::SetMaxhp(int a)
{
	maxhp = a;
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

float Player::GetDirX() {
	return dirX;
}
void Player::SetDirX(float a) {
	dirX = a;
}

float Player::GetDirZ() {
	return dirZ;
}
void Player::SetDirZ(float a) {
	dirZ = a;
}


char* Player::GetNickname() {
	return nickname;
}
void Player::SetNickname(char* a) {
	strcpy_s(nickname, a);
}

char* Player::GetGameId() {
	return game_id;
}
void Player::SetGameId(char* a) {
	strcpy_s(game_id, a);
}

char* Player::GetGamePassword() {
	return game_password;
}
void Player::SetGamePassword(char* a) {
	strcpy_s(game_password, a);
}


int Player::GetId()
{
	return id;
}

Player::~Player()
{

}