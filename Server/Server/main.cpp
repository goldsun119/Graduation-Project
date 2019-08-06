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

enum DB
{
	DB_LOGIN_SUCCESS,
	DB_ALREADY_LOGIN,
	DB_NO_DATA,
	DB_LOGIN_FAIL
};

Player clients[MAX_USER + 1]; // SOCKET이 어떤 SOCKETINFO 인지 알아야한다!!

HANDLE g_iocp;

std::chrono::high_resolution_clock::time_point point;
std::map <int, Monster> monsters;
std::map <int, Item> items;



#define MAX_STR_LEN 10
SQLHENV henv;
SQLHDBC hdbc;
SQLHSTMT hstmt = 0;
SQLRETURN retcode;
SQLWCHAR sz_id[MAX_STR_LEN], sz_password[MAX_STR_LEN], sz_nickname[MAX_STR_LEN];
float db_x, db_y, db_z;
int db_hp, db_maxhp, db_item[4], db_connect;
SQLLEN cb_id = 0, cb_password = 0, cb_nickname = 0, cb_x = 0, cb_y = 0, cb_z = 0, cb_hp = 0, cb_maxhp = 0, cb_item[4]{ 0 }, cb_connect = 0;

void error_display(const char *msg, int err_no);
void initialize();
void make_items();
int get_new_id();

void disconnect(int id);
void worker_thread();
void do_accept();
void do_recv(int id);
void make_obj();
int load_item();
void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
void insert_item_db(int id, int type, float x, float y, float z, int draw);

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



//--------------------------------DB-------------------------------
//float db_x, db_y, db_z;
//int db_hp, db_maxhp, db_item[4], db_connect;
void init_DB();
int get_DB_Info(int ci);
void set_DB_Info(int ci);
void set_DB_Shutdown(int ci);
void new_DB_Id(int ci);
int check_login(string a, string b);
void autosave_info_db();
//--------------------------------DB-------------------------------

int main()
{
	vector<thread> worker_threads;

	initialize();

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	for (int i = 0; i < 4; ++i)
		worker_threads.emplace_back(thread{ worker_thread });

	thread accept_thread{ do_accept };

	point = std::chrono::high_resolution_clock::now();
	thread make_thread{ make_obj };
	thread save_thread{ autosave_info_db };

	accept_thread.join();
	for (auto &th : worker_threads)
		th.join();

	//save_monster, item, player

	

	CloseHandle(g_iocp);
	SQLCancel(hstmt);
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

void error_display(const char *msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	cout << msg;
	wcout << L"에러" << lpMsgBuf << endl;
	LocalFree(lpMsgBuf);
}
void initialize()
{
	for (auto &cl : clients) {
		cl.sock.connected = false;
	}

	init_DB();
	int ret = load_item();
	if (ret == DB_LOGIN_SUCCESS)
	{
	}
	else if (ret == DB_NO_DATA)
	{
		make_items();
	}
	else
	{

	}



}
void make_items()
{
	// 좌표범위
	srand(unsigned(time(NULL)));
	items[0].SetDraw(true);
	items[0].SetId(0);
	items[0].SetType(1);
	items[0].SetPos(351, 150, 351);
	insert_item_db(0, 1, 351, 150, 351, true);
	for (int i = 1; i < MAX_ITEM; ++i)
	{
		float x = float(rand() % 1800 - 900);
		if (-350 <= x && x <= 350)
		{
			--i;
			continue;
		}
		float z = float(rand() % 1800 - 900);
		if (-350 <= z && z <= 350)
		{
			--i;
			continue;
		}
		int type = rand() % 4 + 1;
		items[i].SetDraw( true);
		items[i].SetId(i);
		items[i].SetType(type);
		items[i].SetPos(x, 150, z);
		insert_item_db(i, type, x, 150, z, true);
	}
	cout << "아이템 생성 완료" << endl;
}
int get_new_id()
{
	for (int i = 1; i <= MAX_USER; ++i)
	{
		clients[i].SetLock();
		if (clients[i].sock.connected == false)
		{
			clients[i].sock.connected = true;
			clients[i].SetUnlock();
			return i;
		}
		clients[i].SetUnlock();
	}
	return -1;
}

void disconnect(int id)
{
	clients[id].SetLock();
	closesocket(clients[id].sock.socket);
	clients[id].sock.connected = false;
	clients[id].SetUnlock();
	for (int i = 1; i <= MAX_USER; ++i) {
		if (false == clients[i].sock.connected) continue;
		send_remove_player_packet(i,id);
	}
	cout << "접속종료 ID:" << id << endl;
}
void worker_thread()
{
	while (true)
	{
		DWORD io_byte;
		unsigned long long		key;
		OVER_EX *lpover_ex;

		bool is_error = GetQueuedCompletionStatus(g_iocp, &io_byte, &key, reinterpret_cast<LPWSAOVERLAPPED*>(&lpover_ex), INFINITE);
		if (FALSE == is_error)
		{
			int err_no = WSAGetLastError();
			if (64 != err_no)
				error_display("GQCS ", err_no);
			else {
				disconnect(static_cast<int>(key));
				continue;
			}
		}
		if (0 == io_byte) {
			disconnect(static_cast<int>(key));
			continue;
		}
		if (lpover_ex->is_recv) {
			int rest_size = io_byte;
			char *ptr = lpover_ex->messageBuffer;
			char packet_size = 0;
			if (0 < clients[key].sock.prev_size)
			{
				char packet_data[8] = { 0 };
				int Ipos = 0;
				for (Ipos = 0; Ipos < 8; ++Ipos)
				{
					if (clients[key].sock.packet_buf[Ipos] != 124)
						packet_data[Ipos] = clients[key].sock.packet_buf[Ipos];
					else
					{
						break;
					}
				}

				packet_size = atoi(packet_data) + 8;
			}
			while (rest_size > 0) {
				if (0 == packet_size)
				{
					char packet_data[8] = { 0 };
					int Ipos = 0;
					for (Ipos = 0; Ipos < 8; ++Ipos)
					{
						if (ptr[Ipos] != 124)
							packet_data[Ipos] = ptr[Ipos];
						else
						{
							break;
						}
					}

					packet_size = atoi(packet_data) + 8;
				}
				int required = packet_size - clients[key].sock.prev_size;
				if (rest_size >= required) {
					memcpy(clients[key].sock.packet_buf + clients[key].sock.prev_size, ptr, required);
					process_packet(static_cast<int>(key), packet_size, clients[key].sock.packet_buf);
					rest_size -= required;
					ptr += required;
					packet_size = 0;
				}
				else {
					memcpy(clients[key].sock.packet_buf + clients[key].sock.prev_size,
						ptr, rest_size);
					rest_size = 0;
				}
			}
			do_recv(static_cast<int>(key));
		}
		else {
			delete lpover_ex;
		}
	}
}
void do_accept()
{
	// Winsock Start - windock.dll 로드
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		cout << "Error - Can not load 'winsock.dll' file\n";
		return;
	}

	// 1. 소켓생성  
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET)
	{
		cout << "Error - Invalid socket\n";
		return;
	}

	// 서버정보 객체설정
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 2. 소켓설정
	if (::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		cout << "Error - Fail bind\n";
		// 6. 소켓종료
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return;
	}

	// 3. 수신대기열생성
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		cout << "Error - Fail listen\n";
		// 6. 소켓종료
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return;
	}

	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);
	SOCKET clientSocket;
	DWORD flags;

	while (1)
	{
		clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		BOOL optval = TRUE;
		setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));
		if (clientSocket == INVALID_SOCKET)
		{
			printf("Error - Accept Failure\n");
			return;
		}

		int new_id = get_new_id();
		cout << "접속 아이디: " << new_id << endl;
		clients[new_id].init(new_id);

		memset(&clients[new_id].sock.over, 0x00, sizeof(struct OVER_EX));
		memset(&clients[new_id].sock.packet_buf, 0x00, MAX_BUFFER);

		clients[new_id].sock.socket = clientSocket;
		clients[new_id].sock.over.dataBuffer.len = MAX_BUFFER;
		clients[new_id].sock.over.dataBuffer.buf = clients[new_id].sock.over.messageBuffer;
		clients[new_id].sock.over.is_recv = true;
		flags = 0;

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), g_iocp, new_id, 0);

		//iocp연결 후 트루로 켜야함 그래서 이거 아래에
		clients[new_id].sock.connected = true;

		//send_login_ok_packet(new_id);	// 내 아이디 클라에게 알려주기

		// 처음 연결 시 다른 캐릭터, 몬스터 정보 나에게 보내고 
		//send_all_player_packet(new_id);
		//send_put_item_packet(new_id);

		//new_id 접속을 이미 들어와있는 플레이어들에게 알리기


		do_recv(new_id);
	}

	// 6-2. 리슨 소켓종료
	closesocket(listenSocket);

	// Winsock End
	WSACleanup();

	return;
}
void do_recv(int id)
{
	DWORD flags = 0;

	SOCKET client_s = clients[id].sock.socket;

	OVER_EX *over = &clients[id].sock.over;

	over->dataBuffer.len = MAX_BUFFER;
	over->dataBuffer.buf = over->messageBuffer;
	ZeroMemory(&(over->overlapped), sizeof(WSAOVERLAPPED));
	if (WSARecv(client_s, &over->dataBuffer, 1, NULL, &flags, &(over->overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			cout << "Error - Fail WSARecv(error_code : " << WSAGetLastError << ")" << endl;
		}
	}
}
void make_obj()
{
	int monster_id = 0;
	auto delta = point - std::chrono::high_resolution_clock::now();
	if (std::chrono::duration_cast<std::chrono::seconds>(delta).count()>60)	// 60초 예시
	{
		// monsters map 에 넣기
			float x = float(rand() % 1800 - 900);
			float z = float(rand() % 1800 - 900);

			monsters[monster_id].SetDraw(true);
			monsters[monster_id].SetPos(x, 100, z);
			monsters[monster_id].SetHp(100);

		send_put_monster_packet(monster_id);
		point = std::chrono::high_resolution_clock::now();
		++monster_id;
	}
}


void SendPacket(const int type, const int id, const void *packet, const int packet_size)
{
	if (clients[id].sock.socket != NULL) {
		OVER_EX *over = new OVER_EX;
		ZeroMemory(&over->overlapped, sizeof(over->overlapped));
		over->is_recv = false;
		char p_size[MAX_BUFFER]{ 0 };

		// 클라이언트에게 패킷 전송시 <패킷크기 | 패킷 타입 8바이트 부터 데이터> 으로 전송을 한다.
		itoa(packet_size, p_size, 10);// 10진수로 첫 파라미터를 2번째 파라미터에 문자열로 변환하여 저장
		int buf_len = (int)strlen(p_size);// 저장한 사이즈 읽기 만약 4바이트면 4
		p_size[buf_len] = '|';// 저장한거 뒤에 | 넣기 4|
		p_size[buf_len + 1] = int(type);// 그 뒤에 타입 넣기 4|\x1 

		// 패킷 사이즈를 미리 합쳐서 보내줘야한다.
		memcpy(over->messageBuffer, packet, packet_size);

		for (int i = 8; i < packet_size + 8; ++i) {
			p_size[i] = over->messageBuffer[i - 8];
		}
		ZeroMemory(&over->messageBuffer, sizeof(over->messageBuffer));
		memcpy(over->messageBuffer, p_size, packet_size + 8);

		over->dataBuffer.buf = reinterpret_cast<CHAR *>(p_size);
		over->dataBuffer.len = packet_size + 8;
		int res = WSASend(clients[id].sock.socket, &over->dataBuffer, 1, NULL, 0, &over->overlapped, NULL);
		if (0 != res) {
			int error_no = WSAGetLastError();
			if (WSA_IO_PENDING != error_no) {
				error_display((char *)"SendPacket:WSASend", error_no);
			}
		}
	}
}

void process_packet(const int id, const int packet_size, const char * buf)
{
	if (packet_size == 8)
		return;
	//패킷크기 | 타입 까지 8바이트 그뒤에 데이터
	int Ipos = 0;
	for (Ipos = 0; Ipos < 8; ++Ipos)
	{
		if (buf[Ipos] == 124)
			break;
	}
	char *get_packet = new char[packet_size] {0};
	for (int i = 8; i < packet_size; ++i)
		get_packet[i - 8] = buf[i];

	switch (buf[Ipos + 1])
	{
	case CS_LOGIN:
	{
		auto login_info = Game::Protocol::GetLoginView(get_packet);
		string a = login_info->id()->str();
		string b = login_info->password()->str();
		char game_id[10];
		char game_pw[10];
		strcpy(game_id, a.c_str());
		strcpy(game_pw, b.c_str());

		int ret = check_login(a, b);
		if (ret == DB::DB_LOGIN_SUCCESS)
		{
			clients[id].SetLock();
			clients[id].SetGameId(game_id);
			clients[id].SetGamePassword(game_pw);
			clients[id].SetPos(db_x, db_y, db_z);
			clients[id].SetHp(db_hp);
			clients[id].SetMaxhp(db_maxhp);
			clients[id].SetItem(0, db_item[0]);
			clients[id].SetItem(1, db_item[1]);
			clients[id].SetItem(2, db_item[2]);
			clients[id].SetItem(3, db_item[3]);
			clients[id].SetUnlock();
			send_init_packet(id);
			send_put_player_packet(id);
		}
		else if (ret == DB::DB_ALREADY_LOGIN)
		{

		}
		else if (ret == DB::DB_NO_DATA)
		{
		}
		else
		{

		}
	}
		break;
	case CS_INFO:
	{
		auto client_Check_info = Game::Protocol::GetClientView(get_packet);
		vec3 p = { client_Check_info->position()->x(), client_Check_info->position()->y(), client_Check_info->position()->z() };
		vec3 r = { client_Check_info->rotation()->x(), client_Check_info->rotation()->y(), client_Check_info->rotation()->z() };
		int ani = client_Check_info->animator();
		float x = client_Check_info->dirX();
		float z = client_Check_info->dirZ();
		clients[id].SetLock();
		clients[id].SetPos(p);
		clients[id].SetRotation(r);
		clients[id].SetAnimator(ani);
		clients[id].SetDirX(x);
		clients[id].SetDirZ(z);
		clients[id].SetUnlock();
		//cout << "ID:"<<id<<" pos: " << p.x << "," << p.y << "," << p.z << endl;
	}
	break;
	case CS_GET_ITEM:
	{
		auto item_check_info = Game::Protocol::GetEatView(get_packet);
		int a = item_check_info->itemID();
		int b = item_check_info->playerID();
		//db에 아이템 삽입
		for (int i = 1; i <= MAX_USER; ++i)
		{
			if (i == b) continue;
			if (clients[i].sock.connected == false) continue;
		send_remove_item_packet(i, a);
		}
	}
	break;
	case CS_MONSTER_STATUS:
	{

	}
	break;
	default:
		break;
	}
	send_my_status_to_all_packet(id);

}

//------------------------------packet------------------------------
void send_login_ok_packet(int id)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	clients[id].SetLock();
	int i = clients[id].GetId();
	int hp = clients[id].GetHp();
	int ani = clients[id].GetAnimator();
	float x = clients[id].GetDirX();
	float z = clients[id].GetDirZ();
	auto name = builder.CreateString(clients[id].GetName());
	float h = clients[id].GetHorizontal();
	float v = clients[id].GetVertical();
	auto pos = clients[id].GetPos();
	auto rot = clients[id].GetRotation();
	clients[id].SetUnlock();
	auto data = CreateClient_info(builder, i, hp, ani, x, z, h, v, name, &Vec3(pos.x, pos.y, pos.z), &Vec3(rot.x,rot.y,rot.z));
	builder.Finish(data);
	SendPacket(SC_ID, id, builder.GetBufferPointer(), builder.GetSize());
}
void send_put_player_packet(int id)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	clients[id].SetLock();
	int i = clients[id].GetId();
	int hp = clients[id].GetHp();
	int ani = clients[id].GetAnimator();
	float x = clients[id].GetDirX();
	float z = clients[id].GetDirZ();
	auto name = builder.CreateString(clients[id].GetName());
	float h = clients[id].GetHorizontal();
	float v = clients[id].GetVertical();
	auto pos = clients[id].GetPos();
	auto rot = clients[id].GetRotation();
	clients[id].SetUnlock();
	auto data = CreateClient_info(builder, i, hp, ani, x, z, h, v, name, &Vec3(pos.x, pos.y, pos.z), &Vec3(rot.x, rot.y, rot.z));
	builder.Finish(data);
	for (int to = 1; to <= MAX_USER; ++to)
	{
		clients[to].SetLock();
		if (clients[to].sock.connected == true && to != id)
		{
			clients[to].SetUnlock();
			SendPacket(SC_PUT_PLAYER, to, builder.GetBufferPointer(), builder.GetSize());
			continue;
		}
		clients[to].SetUnlock();
	}
}
void send_all_player_packet(int to)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	std::vector<flatbuffers::Offset<Client_info>> clients_data;

	for (int i = 1; i<=MAX_USER;++i)
	{
		clients[i].SetLock();
		if (clients[i].sock.connected == false || clients[i].GetHp() == 0)
		{
			clients[i].SetUnlock();
			continue;
		}
		if (clients[i].GetId() == to)
		{
			clients[i].SetUnlock();
			continue;
		}
		int id = clients[i].GetId();
		int hp = clients[i].GetHp();
		int ani = clients[i].GetAnimator();
		float x = clients[i].GetDirX();
		float z = clients[i].GetDirZ();
		auto name = builder.CreateString(clients[i].GetName());
		float h = clients[i].GetHorizontal();
		float v = clients[i].GetVertical();
		auto pos = clients[i].GetPos();
		auto rot = clients[i].GetRotation();
		
		clients[i].SetUnlock();

		auto data = CreateClient_info(builder, id, hp, ani, x, z, h, v, name, &Vec3(pos.x, pos.y, pos.z), &Vec3(rot.x, rot.y, rot.z));
		clients_data.emplace_back(data);
	}
	if (clients_data.size() == 0)
		return;
	auto full_data = builder.CreateVector(clients_data);
	auto p = CreateClient_Collection(builder, full_data);
	builder.Finish(p);

	SendPacket(SC_ALL_PLAYER_DATA, to, builder.GetBufferPointer(), builder.GetSize());
}
void send_my_status_to_all_packet(int id)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	clients[id].SetLock();
	int i = clients[id].GetId();
	int hp = clients[id].GetHp();
	int ani = clients[id].GetAnimator();
	float x = clients[i].GetDirX();
	float z = clients[i].GetDirZ();
	auto name = builder.CreateString(clients[id].GetName());
	float h = clients[id].GetHorizontal();
	float v = clients[id].GetVertical();
	auto pos = clients[id].GetPos();
	auto rot = clients[id].GetRotation();
	clients[id].SetUnlock();
	auto data = CreateClient_info(builder,i,hp,ani,x,z,h,v,name,&Vec3(pos.x,pos.y,pos.z),&Vec3(rot.x,rot.y,rot.z));
	builder.Finish(data);
	for (int to = 1; to <= MAX_USER; ++to)
	{
		if (clients[to].sock.connected == true&& to != id)
			SendPacket(SC_PLAYER_STATUS, to, builder.GetBufferPointer(), builder.GetSize());
	}
}

void send_remove_player_packet(int to, int obj)
{
	int i = obj;
	SendPacket(SC_REMOVE_PLAYER, to, &i, sizeof(i));
}

void send_put_monster_packet(int monster_id) 
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	monsters[monster_id].SetLock();
	int i = monsters[monster_id].GetId();
	int hp = monsters[monster_id].GetHp();
	int ani = monsters[monster_id].GetAnimator();
	float x = monsters[monster_id].GetDirX();
	float z = monsters[monster_id].GetDirZ();
	auto pos = monsters[monster_id].GetPos();
	auto rot = monsters[monster_id].GetRotation();
	monsters[monster_id].SetUnlock();
	auto data = CreateMonster_info(builder, i, hp, ani, x, z, &Vec3(pos.x, pos.y, pos.z), &Vec3(rot.x, rot.y, rot.z));
	builder.Finish(data);
	for (int to = 1; to <= MAX_USER; ++to)
	{
		if (clients[to].sock.connected == true)
			SendPacket(SC_PUT_MONSTER, to, builder.GetBufferPointer(), builder.GetSize());
	}
}

void send_put_item_packet(int id)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	std::vector<flatbuffers::Offset<Item_info>> items_data;

	for (int item_id = 0; item_id < MAX_ITEM; ++item_id)
	{
		items[item_id].SetLock();
		int i = items[item_id].GetId();
		int t = items[item_id].GetType();
		auto pos = items[item_id].GetPos();
		items[item_id].SetUnlock();
		auto data = CreateItem_info(builder, i, t, &Vec3(pos.x, pos.y, pos.z));
		items_data.emplace_back(data);
	}
	auto full_data = builder.CreateVector(items_data);
	auto p = CreateItem_Collection(builder, full_data);
	builder.Finish(p);
	for (int to = 1; to <= MAX_USER; ++to)
	{
		if (clients[to].sock.connected == true)
			SendPacket(SC_PUT_ITEM, to, builder.GetBufferPointer(), builder.GetSize());
	}
}

void send_remove_item_packet(int id, int item)
{
	int i = item;
	SendPacket(SC_REMOVE_ITEM, id, &i, sizeof(i));
}

void send_init_packet(int id)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	std::vector<flatbuffers::Offset<Item_info>> items_data;
	std::vector<flatbuffers::Offset<Client_info>> clients_data;
	std::vector<flatbuffers::Offset<Monster_info>> monsters_data;
	for (int item_id = 0; item_id < MAX_ITEM; ++item_id)
	{
		items[item_id].SetLock();
		int i = items[item_id].GetId();
		int t = items[item_id].GetType();
		auto pos = items[item_id].GetPos();
		items[item_id].SetUnlock();
		auto data = CreateItem_info(builder, i, t, &Vec3(pos.x, pos.y, pos.z));
		items_data.emplace_back(data);
	}
	auto full_items_data = builder.CreateVector(items_data);

	for (int i = 1; i <= MAX_USER; ++i)
	{
		clients[i].SetLock();
		if (clients[i].sock.connected == false || clients[i].GetHp() == 0)
		{
			clients[i].SetUnlock();
			continue;
		}
		//if (clients[i].GetId() == id)
		//{
		//	clients[i].SetUnlock();
		//	continue;
		//}
		int id = clients[i].GetId();
		int hp = clients[i].GetHp();
		int ani = clients[i].GetAnimator();
		float x = clients[i].GetDirX();
		float z = clients[i].GetDirZ();
		auto name = builder.CreateString(clients[i].GetName());
		float h = clients[i].GetHorizontal();
		float v = clients[i].GetVertical();
		auto pos = clients[i].GetPos();
		auto rot = clients[i].GetRotation();

		clients[i].SetUnlock();

		auto data = CreateClient_info(builder, id, hp, ani, x, z, h, v, name, &Vec3(pos.x, pos.y, pos.z), &Vec3(rot.x, rot.y, rot.z));
		clients_data.emplace_back(data);
	}
	if (clients_data.size() == 0)
		return;
	auto full_clients_data = builder.CreateVector(clients_data);
	auto full_monsters_data = builder.CreateVector(monsters_data);
	auto p = CreateInit_Collection(builder, id, full_items_data, full_monsters_data, full_clients_data);
	builder.Finish(p);
	SendPacket(SC_INIT_DATA, id, builder.GetBufferPointer(), builder.GetSize());

}


//------------------------------packet------------------------------

void init_DB()
{
	setlocale(LC_ALL, "korean");
	// Allocate environment handle  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	// Set the ODBC version environment attribute  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
		// Allocate connection handle  
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds  
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"FineDust", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					std::cout << std::endl << "DB Connected" << std::endl;
				}
			}
		}
	}

}

void set_login_on(int ci)
{
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	WCHAR Query[MAX_BUFFER];
	wsprintf((LPWSTR)Query, L"EXEC dbo.user_get_info %s, %s", clients[ci].GetGameId(), clients[ci].GetGamePassword());
	//sprintf(buf, "EXEC dbo.user_get_info %s, %s", clients[ci].GetGameId(), clients[ci].GetGamePassword());
	//MultiByteToWideChar(CP_UTF8, 0, buf, strlen(buf), sql_data, sizeof sql_data / sizeof *sql_data);
	//sql_data[strlen(buf)] = '\0';

	retcode = SQLExecDirect(hstmt, (SQLWCHAR*)Query, SQL_NTS);
	//retcode = SQLExecDirect(hstmt, sql_data, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
	{
	}
}
  

int check_login(string a, string b)
{
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
		WCHAR Query[MAX_BUFFER];

		wsprintf((LPWSTR)Query, L"EXEC dbo.user_login %s, %s", a, b);
		retcode = SQLExecDirect(hstmt, (SQLWCHAR*)Query, SQL_NTS);
		//sprintf(buf, "EXEC dbo.user_login %s, %s", a, b);
		//MultiByteToWideChar(CP_UTF8, 0, buf, strlen(buf), sql_data, sizeof sql_data / sizeof *sql_data);
		//sql_data[strlen(buf)] = '\0';

		//retcode = SQLExecDirect(hstmt, sql_data, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

			// Bind columns 1, 2, and 3  
			retcode = SQLBindCol(hstmt, 1, SQL_WCHAR, sz_id, MAX_STR_LEN, &cb_id);
			retcode = SQLBindCol(hstmt, 2, SQL_WCHAR, sz_password, MAX_STR_LEN, &cb_password);
			retcode = SQLBindCol(hstmt, 3, SQL_WCHAR, sz_nickname, MAX_STR_LEN, &cb_nickname);
			retcode = SQLBindCol(hstmt, 4, SQL_REAL, &db_x, MAX_STR_LEN, &cb_x);
			retcode = SQLBindCol(hstmt, 5, SQL_REAL, &db_y, MAX_STR_LEN, &cb_y);
			retcode = SQLBindCol(hstmt, 6, SQL_REAL, &db_z, MAX_STR_LEN, &cb_z);
			retcode = SQLBindCol(hstmt, 7, SQL_INTEGER, &db_hp, MAX_STR_LEN, &cb_hp);
			retcode = SQLBindCol(hstmt, 8, SQL_INTEGER, &db_maxhp, MAX_STR_LEN, &cb_maxhp);
			retcode = SQLBindCol(hstmt, 9, SQL_INTEGER, &db_item[0], MAX_STR_LEN, &cb_item[0]);
			retcode = SQLBindCol(hstmt, 10, SQL_INTEGER, &db_item[1], MAX_STR_LEN, &cb_item[1]);
			retcode = SQLBindCol(hstmt, 11, SQL_INTEGER, &db_item[2], MAX_STR_LEN, &cb_item[2]);
			retcode = SQLBindCol(hstmt, 12, SQL_INTEGER, &db_item[3], MAX_STR_LEN, &cb_item[3]);
			retcode = SQLBindCol(hstmt, 13, SQL_INTEGER, &db_connect, MAX_STR_LEN, &cb_connect);

			// Fetch and print each row of data. On an error, display a message and exit.  

			retcode = SQLFetch(hstmt);

			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				if (db_connect)
				{
					//연결끊기 후
					return DB_ALREADY_LOGIN;
				}
				//connect 1로 바꾸기
				return DB_LOGIN_SUCCESS;
			}
		}

		if (retcode == SQL_NO_DATA) {
			//아이디 비번 추가하기
			return DB_NO_DATA;
		}

		if (retcode == SQL_ERROR) {  
			return DB_LOGIN_FAIL;
		}
}


int get_DB_Info(int ci) {
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
		WCHAR Query[MAX_BUFFER];

		wsprintf((LPWSTR)Query, L"EXEC dbo.user_get_info %s, %s", clients[ci].GetGameId(), clients[ci].GetGamePassword());

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)Query, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

			// Bind columns 1, 2, and 3  
			retcode = SQLBindCol(hstmt, 1, SQL_WCHAR, sz_id, MAX_STR_LEN, &cb_id);
			retcode = SQLBindCol(hstmt, 2, SQL_WCHAR, sz_password, MAX_STR_LEN, &cb_password);
			retcode = SQLBindCol(hstmt, 3, SQL_WCHAR, sz_nickname, MAX_STR_LEN, &cb_nickname);
			retcode = SQLBindCol(hstmt, 4, SQL_FLOAT, &db_x, MAX_STR_LEN, &cb_x);
			retcode = SQLBindCol(hstmt, 5, SQL_FLOAT, &db_y, MAX_STR_LEN, &cb_y);
			retcode = SQLBindCol(hstmt, 6, SQL_FLOAT, &db_z, MAX_STR_LEN, &cb_z);
			retcode = SQLBindCol(hstmt, 7, SQL_INTEGER, &db_hp, MAX_STR_LEN, &cb_hp);
			retcode = SQLBindCol(hstmt, 8, SQL_INTEGER, &db_maxhp, MAX_STR_LEN, &cb_maxhp);
			retcode = SQLBindCol(hstmt, 9, SQL_INTEGER, &db_item[0], MAX_STR_LEN, &cb_item[0]);
			retcode = SQLBindCol(hstmt, 10, SQL_INTEGER, &db_item[1], MAX_STR_LEN, &cb_item[1]);
			retcode = SQLBindCol(hstmt, 11, SQL_INTEGER, &db_item[2], MAX_STR_LEN, &cb_item[2]);
			retcode = SQLBindCol(hstmt, 12, SQL_INTEGER, &db_item[3], MAX_STR_LEN, &cb_item[3]);
			retcode = SQLBindCol(hstmt, 13, SQL_INTEGER, &db_connect, MAX_STR_LEN, &cb_connect);

			// Fetch and print each row of data. On an error, display a message and exit.  

			retcode = SQLFetch(hstmt);

			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
#if (DebugMod == TRUE )
				printf("ID : %s\tX : %d\tY : %d\n", sz_id, db_x, db_y);
#endif
				return DB_LOGIN_SUCCESS;
			}
		}

		if (retcode == SQL_NO_DATA) {
			return DB_NO_DATA;
		}

		if (retcode == SQL_ERROR) {
			return DB_LOGIN_FAIL;
		}
	
}

void set_DB_Info(int ci) {
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

		WCHAR Query[MAX_BUFFER];
		swprintf((LPWSTR)Query, L"EXEC dbo.set_user %s, %s, %s, %f, %f, %f, %d, %d, %d, %d, %d, %d, %d", clients[ci].GetGameId(), clients[ci].GetGamePassword(), clients[ci].GetNickname(), clients[ci].GetXPos(), clients[ci].GetYPos(), clients[ci].GetZPos(), clients[ci].GetHp(), clients[ci].GetMaxhp(), clients[ci].GetItem(0), clients[ci].GetItem(1), clients[ci].GetItem(2), clients[ci].GetItem(3), 1);

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)Query, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {


			// Bind columns 1, 2, and 3  
			retcode = SQLBindCol(hstmt, 1, SQL_WCHAR, sz_id, MAX_STR_LEN, &cb_id);
			retcode = SQLBindCol(hstmt, 2, SQL_WCHAR, sz_password, MAX_STR_LEN, &cb_password);
			retcode = SQLBindCol(hstmt, 3, SQL_WCHAR, sz_nickname, MAX_STR_LEN, &cb_nickname);
			retcode = SQLBindCol(hstmt, 4, SQL_FLOAT, &db_x, MAX_STR_LEN, &cb_x);
			retcode = SQLBindCol(hstmt, 5, SQL_FLOAT, &db_y, MAX_STR_LEN, &cb_y);
			retcode = SQLBindCol(hstmt, 6, SQL_FLOAT, &db_z, MAX_STR_LEN, &cb_z);
			retcode = SQLBindCol(hstmt, 7, SQL_INTEGER, &db_hp, MAX_STR_LEN, &cb_hp);
			retcode = SQLBindCol(hstmt, 8, SQL_INTEGER, &db_maxhp, MAX_STR_LEN, &cb_maxhp);
			retcode = SQLBindCol(hstmt, 9, SQL_INTEGER, &db_item[0], MAX_STR_LEN, &cb_item[0]);
			retcode = SQLBindCol(hstmt, 10, SQL_INTEGER, &db_item[1], MAX_STR_LEN, &cb_item[1]);
			retcode = SQLBindCol(hstmt, 11, SQL_INTEGER, &db_item[2], MAX_STR_LEN, &cb_item[2]);
			retcode = SQLBindCol(hstmt, 12, SQL_INTEGER, &db_item[3], MAX_STR_LEN, &cb_item[3]);
			retcode = SQLBindCol(hstmt, 13, SQL_INTEGER, &db_connect, MAX_STR_LEN, &cb_connect);

			// Fetch and print each row of data. On an error, display a message and exit.  

			retcode = SQLFetch(hstmt);
			if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
			}

			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
#if (DebugMod == TRUE )
				printf("ID : %s\tX : %d\tY : %d\n", sz_id, db_x, db_y);
#endif
			}

		}
}

void new_DB_Id(int ci) {
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
		WCHAR Query[MAX_BUFFER];

		swprintf((LPWSTR)Query, L"EXEC dbo.insert_user %s, %s, %s, %f, %f, %f, %d", clients[ci].GetGameId(), clients[ci].GetGamePassword(), clients[ci].GetNickname(), clients[ci].GetXPos(), clients[ci].GetYPos(), clients[ci].GetZPos(), clients[ci].GetHp());

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)Query, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

			retcode = SQLFetch(hstmt);
			if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
			}

			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
#if (DebugMod == TRUE )
				printf("ID : %s\tX : %d\tY : %d\n", sz_id, db_x, db_y);
#endif
			}

		}
}

void set_DB_Shutdown(int ci) {
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
		WCHAR Query[MAX_BUFFER];

		//sprintf((LPWSTR)buf, "EXEC dbo.shutdown_id %s", g_clients[ci].game_id);

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)Query, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLFetch(hstmt);
			if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
			}

			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
#if (DebugMod == TRUE )
				printf("ID : %s\tX : %d\tY : %d\n", sz_id, db_x, db_y);
#endif
			}

		}
	SQLDisconnect(hdbc);
}

int load_item()
{
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	WCHAR Query[MAX_BUFFER];

	wsprintf((LPWSTR)Query, L"EXEC dbo.get_item_info");

	retcode = SQLExecDirect(hstmt, (SQLWCHAR *)Query, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		
		float db_x, db_y, db_z;
		int db_id, db_type, db_draw;
		SQLLEN cb_x = 0, cb_y = 0, cb_z = 0, cb_id = 0, cb_type = 0, cb_draw = 0;

		// Bind columns 1, 2, and 3  
		retcode = SQLBindCol(hstmt, 1, SQL_INTEGER, &db_id, MAX_STR_LEN, &cb_id);
		retcode = SQLBindCol(hstmt, 2, SQL_INTEGER, &db_type, MAX_STR_LEN, &cb_type);
		retcode = SQLBindCol(hstmt, 3, SQL_REAL, &db_x, MAX_STR_LEN, &cb_x);
		retcode = SQLBindCol(hstmt, 4, SQL_REAL, &db_y, MAX_STR_LEN, &cb_y);
		retcode = SQLBindCol(hstmt, 5, SQL_REAL, &db_z, MAX_STR_LEN, &cb_z);
		retcode = SQLBindCol(hstmt, 6, SQL_INTEGER, &db_draw, MAX_STR_LEN, &cb_draw);

		// Fetch and print each row of data. On an error, display a message and exit.  
		while (true) {
			retcode = SQLFetch(hstmt);
			if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
			{
				//show_error();
			}
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{
				//wprintf(L"%d: %S %S %S\n", i + 1, sCustID, szName, szPhone);
				items[db_id].SetPos(db_x, db_y, db_z);
				items[db_id].SetType(db_type);
				items[db_id].SetDraw(db_draw);
			}
			else
				break;
		}
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
#if (DebugMod == TRUE )
			printf("ID : %s\tX : %d\tY : %d\n", sz_id, db_x, db_y);
#endif
			return DB_LOGIN_SUCCESS;
		}
	}

	if (retcode == SQL_NO_DATA) {
		return DB_NO_DATA;
	}

	if (retcode == SQL_ERROR) {
		return DB_LOGIN_FAIL;
	}		
}

void insert_item_db(int id, int type, float x, float y, float z, int draw)
{
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	WCHAR Query[MAX_BUFFER];

	swprintf((LPWSTR)Query, L"EXEC dbo.insert_item %d, %d, %f, %f, %f, %d", id, type, x, y, z, draw);

	retcode = SQLExecDirect(hstmt, (SQLWCHAR *)Query, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{

	}
	if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
	{
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);

	}
}


void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode)
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER iError;
	WCHAR wszMessage[1000];
	WCHAR wszState[SQL_SQLSTATE_SIZE + 1];
	if (RetCode == SQL_INVALID_HANDLE) {
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}
	while (SQLGetDiagRecW(hType, hHandle, ++iRec, wszState, &iError, wszMessage, (SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)), (SQLSMALLINT *)NULL) == SQL_SUCCESS)
	{
		//Hide data truncated..
		if (wcsncmp(wszState, L"01004", 5))
		{
			fwprintf(stderr, L"[%5.5s] %s (%d) \n", wszState, wszMessage, iError);
		}
	}
}

void autosave_info_db()
{
	std::chrono::high_resolution_clock::time_point save_time = std::chrono::high_resolution_clock::now();
	while (true)
	{
		if (save_time + 10s <= std::chrono::high_resolution_clock::now())
		{
			for (int i = 1; i <= MAX_USER; ++i)
			{
				if (clients[i].sock.connected)
				{
					//DB_SQL_SET_POS(i);
				}
			}
			save_time = std::chrono::high_resolution_clock::now();
		}
	}
}


/*	

inline const Game::Protocol::Client_info *GetClientView(const void *buf) {
	return flatbuffers::GetRoot<Game::Protocol::Client_info>(buf);
}
inline const Game::Protocol::Monster_info *GetMonsterView(const void *buf) {
	return flatbuffers::GetRoot<Game::Protocol::Monster_info>(buf);
}
inline const Game::Protocol::Item_info *GetItemView(const void *buf) {
	return flatbuffers::GetRoot<Game::Protocol::Item_info>(buf);
}

inline const Game::Protocol::Eat_Item *GetEatView(const void *buf) {
	return flatbuffers::GetRoot<Game::Protocol::Eat_ Item>(buf);
}

inline const Game::Protocol::Login *GetLoginView(const void *buf) {
	return flatbuffers::GetRoot<Game::Protocol::Login>(buf);
}
*/