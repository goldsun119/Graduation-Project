#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <map>
#include <chrono>

using namespace std;

#include <winsock2.h>

#include "protocol.h"
#include "Player.h"
#include "Monster.h"
#include "info_generated.h"

using namespace Game::Protocol;

#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER        1024

Player clients[MAX_USER+1]; // SOCKET이 어떤 SOCKETINFO 인지 알아야한다!!

HANDLE g_iocp;

mutex buf_lock;

std::chrono::high_resolution_clock::time_point point;
std::map <int, Monster> monsters;

void error_display(const char *msg, int err_no);
void initialize();
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
//------------------------------packet------------------------------

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

	accept_thread.join();
	for (auto &th : worker_threads)
		th.join();

	CloseHandle(g_iocp);
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
}
int get_new_id()
{
	for (int i = 1; i < MAX_USER; ++i)
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
		ULONG		key;
		OVER_EX *lpover_ex;

		bool is_error = GetQueuedCompletionStatus(g_iocp, &io_byte, &key, reinterpret_cast<LPWSAOVERLAPPED*>(&lpover_ex), INFINITE);
		if (FALSE == is_error)
		{
			int err_no = WSAGetLastError();
			if (64 != err_no)
				error_display("GQCS ", err_no);
			else {
				disconnect(key);
				continue;
			}
		}
		if (0 == io_byte) {
			disconnect(key);
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
					process_packet(key, packet_size, clients[key].sock.packet_buf);
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
			do_recv(key);
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

		send_login_ok_packet(new_id);	// 내 아이디 클라에게 알려주기

		// 처음 연결 시 다른 캐릭터, 몬스터 정보 나에게 보내고 
		send_all_player_packet(new_id);

		//new_id 접속을 이미 들어와있는 플레이어들에게 알리기
		send_put_player_packet(new_id);


		do_recv(new_id);
	}

	// 6-2. 리슨 소켓종료
	closesocket(listenSocket);

	// Winsock End
	WSACleanup();

	return;
}
void do_recv(char id)
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
		// monster map 에 넣기
		

		send_put_monster_packet(monster_id);
		point = std::chrono::high_resolution_clock::now();
		++monster_id;
	}
}


void SendPacket(const int type, const int id, const void *packet, const int packet_size)
{
	// 창현오빠꺼에서 가져옴 이해하기
	if (clients[id].sock.socket != NULL) {
		OVER_EX *over = new OVER_EX;
		ZeroMemory(&over->overlapped, sizeof(over->overlapped));
		over->is_recv = false;
		buf_lock.lock();
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
		memcpy(over->messageBuffer, p_size, MAX_BUFFER);

		over->dataBuffer.buf = reinterpret_cast<CHAR *>(p_size);
		over->dataBuffer.len = packet_size + 8;
		int res = WSASend(clients[id].sock.socket, &over->dataBuffer, 1, NULL, 0, &over->overlapped, NULL);
		buf_lock.unlock();
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
	SendPacket(SC_LOGIN_OK, id, builder.GetBufferPointer(), builder.GetSize());
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
			SendPacket(SC_PUT_PLAER, to, builder.GetBufferPointer(), builder.GetSize());
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

	for (int i = 1; i<MAX_USER+1;++i)
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
	for(int to = 1; to<=MAX_USER;++to)
		SendPacket(SC_PUT_MONSTER, to, builder.GetBufferPointer(), builder.GetSize());
}
//------------------------------packet------------------------------