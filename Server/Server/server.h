#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <map>
#include <chrono>
#include <queue>
#include <random>

#include <winsock2.h>
#include <time.h>
#include <sqlext.h>  

using namespace std;

#include "protocol.h"
#include "Player.h"
#include "Monster.h"
#include "Item.h"
#include "info_generated.h"

using namespace Game::Protocol;

#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER        10000
#define MAX_STR_LEN 10

enum DB
{
	DB_LOGIN_SUCCESS,
	DB_ALREADY_LOGIN,
	DB_NO_DATA,
	DB_LOGIN_FAIL,
	DB_SIGNUP
};

struct T_EVENT {
	std::chrono::high_resolution_clock::time_point start_time;
	int			do_object;
	EVENT_TYPE	event_type;

	constexpr bool operator < (const T_EVENT& _Left) const
	{	// apply operator< to operands
		return (start_time > _Left.start_time);
	}
};

class Server
{

public:
	priority_queue <T_EVENT> timer_queue;
	mutex timer_lock;
	Player clients[MAX_USER + 1];
	HANDLE g_iocp;
	std::chrono::high_resolution_clock::time_point point;
	std::map <int, Monster> monsters;
	std::map <int, Item> items;
	int product_complete[5] = { 0,0,0,0,0 };
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt = 0;
	SQLRETURN retcode;
	mutex query_lock;

	Server();
	~Server();
	void add_timer(EVENT_TYPE ev_type, int object, std::chrono::high_resolution_clock::time_point start_time);
	void error_display(const char *msg, int err_no);
	void initialize();
	void make_items();
	int get_new_id();
	float distance(vec3 a, vec3 b);

	void process_event(T_EVENT &ev);
	void do_timer();

	void disconnect(int id);
	void already_login_disconnect(int id);
	void worker_thread();
	void do_accept();
	void do_recv(int id);
	void make_monster();
	int load_item();
	void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
	void insert_item_db(int id, int type, float x, float y, float z, int draw);

	void SendPacket(const int type, const int id, const void *packet, const int packet_size);

	void process_packet(const int id, const int packet_size, const char * buf);

	//------------------------------packet------------------------------
	void send_login_fail_packet(int id);
	void send_signup_packet(int id);
	void send_my_status_to_all_packet(int id);
	void send_remove_player_packet(int to, int obj);
	void send_remove_item_packet(int id, int item);
	void send_init_packet(int id);
	void send_monster_dead_packet(int monster);
	void send_monster_revive_packet(int to, int monster);
	void send_monster_target_packet(int to, int monster, int target);
	void send_monster_calculate_packet(int to, int monster, int cal);
	void send_monster_pos_packet(int to, int monster);
	void send_complete_making_packet(int num);
	void send_end_game_packet();

	//------------------------------packet------------------------------



	//--------------------------------DB-------------------------------
	//float db_x, db_y, db_z;
	//int db_hp, db_maxhp, db_item[4], db_connect;
	void init_DB();
	void set_login_off(int ci);
	int check_login(string a, string b, int id);
	void db_set_character(int id);
	void autosave_info_db();
	void db_set_pos(int id);

	//--------------------------------DB-------------------------------

	wchar_t* ConvertCtoWC(const char *str);
};

