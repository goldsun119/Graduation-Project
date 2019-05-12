#pragma once
/*
	방향키, 점프 공격 키 입력
	아이템 생성, 지우기
*/
constexpr int SERVER_PORT =			9000;

constexpr int MAX_USER =					4;
constexpr int WORLD_WIDTH =			2000;
constexpr int WORLD_HEIGHT =			2000;

constexpr int ISLAND_RADIUS =			900;

constexpr int DUSTAREA_RADIUS =	625;


constexpr int CS_INFO =					1;
constexpr int CS_GET_ITEM =		2;
constexpr int CS_MONSTER_HP =	3;

constexpr int SC_LOGIN_OK =					1;
constexpr int	SC_PUT_PLAER =				2;
constexpr int SC_REMOVE_PLAYER =		3;
constexpr int SC_PLAYER_STATUS	 =		4;
constexpr int SC_ALL_PLAYER_DATA =	5;
constexpr int SC_PUT_MONSTER =			6;
constexpr int SC_REMOVE_MONSTER =	7;

#pragma pack(push, 1)

// 프로토콜 (서버-<클라)
; struct sc_packet_pos {
	char size;
	char type;
	char id;
	char x, y;
};

struct sc_packet_remove_player {
	char size;
	char type;
	char id;
};
struct sc_packet_login_ok {
	char size;
	char type;
	char id;
};

struct sc_packet_put_player {
	char size;
	char type;
	char id;
	char x, y;
};

struct sc_packet_remove_item {
	char size;
	char type;
	char id;
};

; struct sc_packet_item_pos {
	char size;
	char type;
	char id;
	char x, y;
};

// 프로토콜 ( 클라 -> 서버)
struct cs_packet_key_input {
	char	size;
	char	type;
};

struct cs_packet_put_item {
	char size;
	char type;
	char id;
	char x, y;
};

#pragma pack(pop)