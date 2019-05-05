#include "Monster.h"
/*
table Monster {
  pos:Vec3;

  hp:short = 100;
}
*/

using namespace MyGame::Monster;

void monster_create(float x, float y, float z, short hp)
{
	flatbuffers::FlatBufferBuilder builder;
	auto position = Vec3(x, y, z);
	auto inputHp = hp;
	auto pig = CreateMonster(builder,&position,inputHp);

	builder.Finish(pig);
}