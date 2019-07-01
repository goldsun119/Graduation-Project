#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <map>
#include <chrono>
#include <sqlext.h>  

using namespace std;

#include <winsock2.h>
#include <time.h>

#include "protocol.h"
#include "Player.h"
#include "Monster.h"
#include "Item.h"
#include "info_generated.h"

using namespace Game::Protocol;

#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER        10000

Player clients[MAX_USER + 1]; // SOCKET이 어떤 SOCKETINFO 인지 알아야한다!!

extern HANDLE g_iocp;

extern std::chrono::high_resolution_clock::time_point point;
extern std::map <int, Monster> monsters;
extern std::map <int, Item> items;


void error_display(const char *msg, int err_no);
void initialize();
void make_items();
int get_new_id();

void disconnect(int id);
void worker_thread();
void do_accept();
void do_recv(char id);
void make_obj();

void SendPacket(const int type, const int id, const void *packet, const int packet_size);

void process_packet(const int id, const int packet_size, const char * buf);

//------------------------------packet------------------------------
void send_login_ok_packet(int id);
void send_put_player_packet(int id);
void send_my_status_to_all_packet(int id);
void send_all_player_packet(int id);
void send_remove_player_packet(int to, int obj);
void send_put_monster_packet(int monster_id);
void send_put_item_packet(int id);
void send_remove_item_packet(int id, int item);
void send_init_packet(int id);
//------------------------------packet------------------------------



//------------------------------DB------------------------------
extern int db_x, db_y, db_level, db_hp, db_maxhp, db_exp, db_skill[3], db_connect;
void init_DB();
int get_DB_Info(int ci);
void set_DB_Info(int ci);
void set_DB_Shutdown(int ci);
void new_DB_Id(int ci);
//------------------------------DB------------------------------
