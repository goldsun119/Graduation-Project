#pragma once
#include <string>
struct Vec3
{
	float x, y, z;
};

enum OBJ_TYPE
{
	PLAYER = 1, MONSTER, ITEM
};

class Obj
{
protected:		//파생클래스의 접근은 허용 외부접근은 비허용
	Vec3 pos;
	
	bool isDraw;
	std::string name;
	char type;

public:
	Obj();
	~Obj();
	float GetXPos() const { return pos.x; }
	float GetYPos() const { return pos.y; }
	float GetZPos() const { return pos.z; }
	Vec3 GetPos() const { return pos; }
	bool GetDraw() const { return isDraw; }
	std::string GetName() const { return name; }
	char GetType() { return type; }


	void SetPos(float x, float y, float z) { pos.x = x, pos.y = y, pos.z = z; }
	void SetPos(Vec3 a) { pos = a; }
	void SetXPos(float x) { pos.x = x; }
	void SetYPos(float y) { pos.y = y; }
	void SetZPos(float z) { pos.z = z; }
	void SetDraw(bool d) { isDraw = d; }
	void SetName(std::string n) { name = n; }
	void SetType(char t) { type = t; }
};