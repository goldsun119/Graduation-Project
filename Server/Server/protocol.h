#pragma once

constexpr int SERVER_PORT =			9000;

constexpr int MAX_USER =					4;
constexpr int MAX_MONSTER =			50;
constexpr int MAX_ITEM =					100;
constexpr int WORLD_WIDTH =			2000;
constexpr int WORLD_HEIGHT =			2000;

constexpr int ISLAND_RADIUS =			900;

constexpr int DUSTAREA_RADIUS =	625;


constexpr int CS_INFO =							1;
constexpr int CS_GET_ITEM =				2;
constexpr int CS_MONSTER_STATUS =	3;
constexpr int CS_LOGIN						 = 4;
constexpr int CS_CHARACTER_SELECT = 5;
constexpr int CS_ATTACK = 6;
constexpr int CS_COMPLETE_MAKING = 7;


constexpr int SC_ID =								1;
constexpr int	SC_PUT_PLAYER =			2;
constexpr int SC_REMOVE_PLAYER =		3;
constexpr int SC_PLAYER_STATUS	 =		4;
constexpr int SC_ALL_PLAYER_DATA =	5;
constexpr int SC_PUT_MONSTER =			6;
constexpr int SC_REMOVE_MONSTER =	7;
constexpr int SC_PUT_ITEM =				8;
constexpr int SC_REMOVE_ITEM =			9;
constexpr int SC_PUT_IN_BAG =				10;
constexpr int SC_INIT_DATA				 =	11;
constexpr int SC_LOGIN_SUCCESS =		12;
constexpr int SC_LOGIN_FAIL =				13;
constexpr int SC_SIGNUP =					14;
constexpr int SC_REVIVE_MONSTER = 15;
constexpr int SC_MONSTER_INFO = 16;
constexpr int SC_MONSTER_TARGET = 17;
constexpr int SC_MONSTER_CALCULATE = 18;
constexpr int SC_COMPLETE_MAKING = 19;
constexpr int SC_END_GAME = 20;
#pragma pack(push, 1)



#pragma pack(pop)