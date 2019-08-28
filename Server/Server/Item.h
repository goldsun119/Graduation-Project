#pragma once
#include "Obj.h"

enum ITEM_TYPE
{

};

class Item : public Obj
{

private:
	int id;
	int player_id = 0;

public:
	Item();
	~Item();
	int GetId() { return id; }
	void SetId(int i) { id = i; }

	int GetPlayerId() { return player_id; }
	void SetPlayerId(int i) { player_id = i; }

};

