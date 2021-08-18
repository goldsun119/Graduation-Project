#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <map>
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

class iocp_server
{
private:
	Player clients[MAX_USER + 1];
	HANDLE g_iocp;

	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt = 0;
	SQLRETURN retcode;

	void error_display(const char *msg, int err_no);
	void initialize();
	void disconnect(int id);
	void already_login_disconnect(int id);
	void worker_thread();
	void do_accept();
	void do_recv(int id);

	void make_items();
	int load_item();
	void make_monster();

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

	mutex query_lock;
	//--------------------------------DB-------------------------------

public:
	iocp_server();
	~iocp_server();
};

