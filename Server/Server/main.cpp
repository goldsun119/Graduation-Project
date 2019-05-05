#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

#include <winsock2.h>

#include "protocol.h"
#include "Player.h"
#include "Client_info_generated.h"

using namespace Game::Protocol;

#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER        1024

Player clients[MAX_USER]; // SOCKET이 어떤 SOCKETINFO 인지 알아야한다!!

HANDLE g_iocp;

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
		if (clients[i].sock.connected == false)
		{
			clients[i].sock.connected = true;
			return i;
		}
	}
	return -1;
}
void SendPacket(const int type, const int id, const void *packet, const int packet_size)
{
	Player c = clients[id];
	if (c.sock.socket != NULL) {
		//int ptype = reinterpret_cast<unsigned char *>(packet)[1];
		OVER_EX *over = new OVER_EX;
		ZeroMemory(&over->overlapped, sizeof(over->overlapped));
		char p_size[MAX_BUFFER]{ 0 };

		// 클라이언트에게 패킷 전송시 <패킷크기 | 패킷 타입> 으로 전송을 한다.
		itoa(packet_size + 8, p_size, 10);
		int buf_len = (int)strlen(p_size);
		p_size[buf_len] = '|';
		p_size[buf_len + 1] = int(type);

		// 패킷 사이즈를 미리 합쳐서 보내줘야한다.
		memcpy(over->messageBuffer, packet, packet_size);

		for (int i = 8; i < packet_size + 8; ++i) {
			p_size[i] = over->messageBuffer[i - 8];
		}

		over->dataBuffer.buf = reinterpret_cast<CHAR *>(p_size);
		over->dataBuffer.len = packet_size + 8;
		int res = WSASend(c.sock.socket, &over->dataBuffer, 1, NULL, 0, &over->overlapped, NULL);
		if (0 != res) {
			int error_no = WSAGetLastError();
			if (WSA_IO_PENDING != error_no) {
				//err_display((char *)"SendPacket:WSASend", error_no);
			}
		}
	}
}

void send_all_player_packet(int id)
{
	flatbuffers::FlatBufferBuilder builder;
	std::vector<flatbuffers::Offset<Client_info>> clients_data;

	/*
	table Client_info
{
id:int;
hp:int;
animator:int;
horizontal:float;
vertical:float;
name:string;
position:Vec3;
rotation:Vec3;
}
*/
	for (auto c : clients)
	{
		if (c.sock.connected == false||c.GetHp == 0)
			continue;
		int id = c.GetId();
		int hp = c.GetHp();
		int ani = c.GetAnimator();
		auto name = builder.CreateString(c.GetName());
		float h = c.GetHorizontal();
		float v = c.GetVertical();
		auto pos = c.GetPos();
		auto rot = c.GetRotation();

		auto data = CreateClient_info(builder, id, hp, ani, h, v, name, pos, rot);
		clients_data.emplace_back(data);
	}
	auto full_data = builder.CreateVector(clients_data);
	auto p = CreateClient_Collection(builder, full_data);
	builder.Finish(p);

	SendPacket(SC_ALL_PLAYER_DATA, id, builder.GetBufferPointer(), builder.GetSize());

	/*SOCKET client_s = clients[key].sock.socket;

	OVER_EX *over = new OVER_EX;

	over->dataBuffer.len = packet[0];
	over->dataBuffer.buf = over->messageBuffer;
	memcpy(over->messageBuffer, packet, packet[0]);
	ZeroMemory(&(over->overlapped), sizeof(WSAOVERLAPPED));
	if (WSASend(client_s, &over->dataBuffer, 1, NULL, 0, &(over->overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			cout << "Error - Fail WSASend(error_code : " << WSAGetLastError << ")" << endl;
		}
	}*/
}

void send_login_ok_packet(char id)
{
	//sc_packet_login_ok packet;
	//packet.id = id;
	//packet.size = sizeof(packet);
	//packet.type = SC_LOGIN_OK;

	//send_packet(id, reinterpret_cast<char *>(&packet));
}

void send_pos_packet(char to, char obj)
{
	//sc_packet_pos packet;
	//packet.id = obj;
	//packet.size = sizeof(packet);
	//packet.type = SC_POS;

	//send_packet(to, reinterpret_cast<char *>(&packet));
}

void process_packet(const char id,const int packet_size, const char * buf)
{
	char *get_packet = new char[packet_size] {0};	
	for (int i = 0; i < packet_size; ++i)
		get_packet[i] = buf[i];

	switch (get_packet[1])
	{
	case CS_MOVE:
		auto client_Check_info = Game::Protocol::GetClientView(get_packet);
		clients[id].SetPos(client_Check_info->position);
		break;
	case CS_ATTACK:
		break;
	case CS_GET_ITEM:
		break;
	default:
		break;
	}

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

void worker_thread()
{
	while (true)
	{
		DWORD io_byte;
		ULONG		key;
		OVER_EX *lpover_ex;

		bool is_error = GetQueuedCompletionStatus(g_iocp, &io_byte, &key, reinterpret_cast<LPWSAOVERLAPPED*>(&lpover_ex), INFINITE);

		if (lpover_ex->is_recv) {
			int rest_size = io_byte;
			char *ptr = lpover_ex->messageBuffer;
			char packet_size = 0;
			if (0 < clients[key].sock.prev_size) packet_size = clients[key].sock.packet_buf[0];
			while (rest_size > 0)
			{
				if (0 == packet_size) packet_size = ptr[0];
				int required = packet_size - clients[key].sock.prev_size;
				if (rest_size >= required) {
					memcpy(clients[key].sock.packet_buf + clients[key].sock.prev_size, ptr, required);
					process_packet(key,clients[key].sock.packet_buf.len, clients[key].sock.packet_buf);
					rest_size -= required;
					ptr += required;
					packet_size = 0;
				}
				else
				{
					memcpy(clients[key].sock.packet_buf + clients[key].sock.prev_size, ptr, rest_size);
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
		if (clientSocket == INVALID_SOCKET)
		{
			printf("Error - Accept Failure\n");
			return;
		}

		int new_id = get_new_id();

		clients[new_id].init(new_id);

		clients[new_id].sock.connected = false;
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

		send_login_ok_packet(new_id);

		// 처음 연결 시 다른 캐릭터, 몬스터 정보 나에게 보내고 내 정보 다른 캐릭터에게 보내기

		do_recv(new_id);
	}

	// 6-2. 리슨 소켓종료
	closesocket(listenSocket);

	// Winsock End
	WSACleanup();

	return;
}

int main()
{
	vector<thread> worker_threads;

	initialize();

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	for (int i = 0; i < 4; ++i)
		worker_threads.emplace_back(thread{ worker_thread });

	thread accept_thread{ do_accept };

	accept_thread.join();
	for (auto &th : worker_threads)
		th.join();

	CloseHandle(g_iocp);
}