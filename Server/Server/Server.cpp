#include "server.h"



Server::Server()
{
	vector<thread> worker_threads;

	for (auto &cl : clients) {
		cl.sock.connected = false;
	}

	init_DB();
	int ret = load_item();
	if (ret == DB_LOGIN_SUCCESS)
	{
		cout << "������ �ҷ����� �Ϸ�" << endl;
	}
	else if (ret == DB_NO_DATA)
	{
		make_items();
	}

	make_monster();

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	for (int i = 0; i < 4; ++i)
		worker_threads.emplace_back(thread{ &Server::worker_thread, this });

	thread accept_thread{ &Server::do_accept, this };
	thread timer_thread{ &Server::do_timer, this };
	point = std::chrono::high_resolution_clock::now();
	thread save_thread{ &autosave_info_db, this };

	accept_thread.join();
	timer_thread.join();
	for (auto &th : worker_threads)
		th.join();

	//save_monster, item, player
	for (int i = 1; i <= MAX_USER; ++i)
	{
		if (clients[i].sock.connected == true)
			set_login_off(i);
	}

	CloseHandle(g_iocp);
	SQLCancel(hstmt);
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}


Server::~Server()
{
}

void Server::initialize()
{

}


void Server::error_display(const char *msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	cout << msg;
	wcout << L"����" << lpMsgBuf << endl;
	LocalFree(lpMsgBuf);
}

void Server::make_items()
{
	// ��ǥ����
	srand(unsigned(time(NULL)));
	//�ÿ��� ������ �� ������ 30���� ��ġ ���� ������ ����

	int item_id = 0;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(1);
	items[item_id].SetPos(-41, 30, 370);

	item_id = 1;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(2);
	items[item_id].SetPos(-49, 30, 360);

	item_id = 2;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(3);
	items[item_id].SetPos(-1, 30, 395);

	item_id = 3;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(2);
	items[item_id].SetPos(44, 30, 394);

	item_id = 4;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(1);
	items[item_id].SetPos(57, 30, 359);

	item_id = 5;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(1);
	items[item_id].SetPos(35, 30, 410);

	item_id = 6;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(2);
	items[item_id].SetPos(40, 30, 415);

	item_id = 7;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(3);
	items[item_id].SetPos(45, 30, 423);

	item_id = 8;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(3);
	items[item_id].SetPos(5.9, 30, 427);

	item_id = 9;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(2);
	items[item_id].SetPos(30, 30, 445);

	item_id = 10;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(1);
	items[item_id].SetPos(61, 30, 452);

	item_id = 11;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(1);
	items[item_id].SetPos(77, 30, 476);

	item_id = 12;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(2);
	items[item_id].SetPos(97, 30, 476);

	item_id = 13;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(3);
	items[item_id].SetPos(107, 30, 465);

	item_id = 14;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(3);
	items[item_id].SetPos(124, 30, 456);

	item_id = 15;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(2);
	items[item_id].SetPos(129, 30, 444);

	item_id = 16;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(1);
	items[item_id].SetPos(116, 30, 426);

	item_id = 17;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(1);
	items[item_id].SetPos(133, 30, 411);

	item_id = 18;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(2);
	items[item_id].SetPos(123, 30, 389);

	item_id = 19;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(3);
	items[item_id].SetPos(149, 30, 380);

	item_id = 20;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(3);
	items[item_id].SetPos(141, 30, 365);

	item_id = 4;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(2);
	items[item_id].SetPos(126, 30, 349);

	item_id = 21;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(1);
	items[item_id].SetPos(105, 30, 347);

	item_id = 22;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(1);
	items[item_id].SetPos(-35, 30, 416);

	item_id = 23;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(2);
	items[item_id].SetPos(-22, 30, 437);

	item_id = 24;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(3);
	items[item_id].SetPos(-50, 30, 429);

	item_id = 25;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(3);
	items[item_id].SetPos(-31, 30, 455);

	item_id = 26;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(2);
	items[item_id].SetPos(-15, 30, 463);

	item_id = 27;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(1);
	items[item_id].SetPos(-36, 30, 385);

	item_id = 28;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(1);
	items[item_id].SetPos(13, 30, 459);

	item_id = 29;
	items[item_id].SetDraw(true);
	items[item_id].SetId(item_id);
	items[item_id].SetType(3);
	items[item_id].SetPos(-10, 30, 466);

	for (int i = 0; i < 30; ++i)
	{
		insert_item_db(i, items[i].GetType(), items[i].GetXPos(), items[i].GetYPos(), items[i].GetZPos(), true);
	}

	for (item_id = 30; item_id < MAX_ITEM; ++item_id)
	{
		items[item_id].SetDraw(true);
		items[item_id].SetId(item_id);
		int r = rand() % 3 + 1;
		items[item_id].SetType(r);

		float x = float(rand() % 1000 - 500);
		float z = float(rand() % 1000 - 500);
		if (-350 <= z && z <= 350 && -350 <= x && x <= 350)
		{
			--item_id;
			continue;
		}

		items[item_id].SetPos(x, 30, z);
		insert_item_db(item_id, items[item_id].GetType(), items[item_id].GetXPos(), items[item_id].GetYPos(), items[item_id].GetZPos(), true);
	}

	cout << "������ ���� ���� �� db���� �Ϸ�" << endl;
}

void Server::make_monster()
{
	int monster_id = 1;
	//�ÿ��� ���� �� ������ 4���� ��ġ ���� ������ ����

	srand(unsigned(time(NULL)));

	monsters[0].SetDraw(true);
	monsters[0].SetId(0);
	monsters[0].SetHp(100);
	monsters[0].SetPos(121, 30, 373);
	monsters[0].SetDirX(0);
	monsters[0].SetDirZ(0);
	monsters[0].SetAnimator(100);
	monsters[0].SetCalculate(1);
	monsters[0].SetInitPos(monsters[0].GetPos());
	monsters[0].SetTarget(0);
	monsters[0].SetRotation(vec3(0, 0, 0));

	monsters[1].SetDraw(true);
	monsters[1].SetId(1);
	monsters[1].SetHp(100);
	monsters[1].SetPos(-5, 30, 458);
	monsters[1].SetDirX(0);
	monsters[1].SetDirZ(0);
	monsters[1].SetAnimator(100);
	monsters[1].SetCalculate(1);
	monsters[1].SetInitPos(monsters[1].GetPos());
	monsters[1].SetTarget(0);
	monsters[1].SetRotation(vec3(0, 0, 0));

	monsters[2].SetDraw(true);
	monsters[2].SetId(2);
	monsters[2].SetHp(100);
	monsters[2].SetPos(137, 30, 400);
	monsters[2].SetDirX(0);
	monsters[2].SetDirZ(0);
	monsters[2].SetAnimator(100);
	monsters[2].SetCalculate(1);
	monsters[2].SetInitPos(monsters[2].GetPos());
	monsters[2].SetTarget(0);
	monsters[2].SetRotation(vec3(0, 0, 0));


	monsters[3].SetDraw(true);
	monsters[3].SetId(3);
	monsters[3].SetHp(100);
	monsters[3].SetPos(42, 30, 490);
	monsters[3].SetDirX(0);
	monsters[3].SetDirZ(0);
	monsters[3].SetAnimator(100);
	monsters[3].SetCalculate(1);
	monsters[3].SetInitPos(monsters[3].GetPos());
	monsters[3].SetTarget(0);
	monsters[3].SetRotation(vec3(0, 0, 0));


	monsters[4].SetDraw(true);
	monsters[4].SetId(4);
	monsters[4].SetHp(100);
	monsters[4].SetPos(218, 30, 341);
	monsters[4].SetDirX(0);
	monsters[4].SetDirZ(0);
	monsters[4].SetAnimator(100);
	monsters[4].SetCalculate(1);
	monsters[4].SetInitPos(monsters[4].GetPos());
	monsters[4].SetTarget(0);
	monsters[4].SetRotation(vec3(0, 0, 0));
	for (monster_id = 5; monster_id < MAX_MONSTER; ++monster_id)
	{
		float x = float(rand() % 1000 - 500);
		float z = float(rand() % 1000 - 500);
		if (-350 <= z && z <= 350 && -350 <= x && x <= 350)
		{
			--monster_id;
			continue;
		}
		monsters[monster_id].SetDraw(true);
		monsters[monster_id].SetId(monster_id);
		monsters[monster_id].SetHp(100);
		monsters[monster_id].SetPos(x, 30, z);
		monsters[monster_id].SetInitPos(x, 30, z);
		monsters[monster_id].SetDirX(0);
		monsters[monster_id].SetDirZ(0);
		monsters[monster_id].SetAnimator(100);
		monsters[monster_id].SetCalculate(1);
		monsters[monster_id].SetTarget(0);
		monsters[monster_id].SetRotation(vec3(0, 0, 0));
		//send_put_monster_packet(monster_id);
	}
	cout << "���� ���� �Ϸ�" << endl;
}

int Server::get_new_id()
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

float Server::distance(vec3 a, vec3 b)
{
	float x = (a.x - b.x) * (a.x - b.x);
	float y = (a.y - b.y) * (a.y - b.y);
	float z = (a.z - b.z) * (a.z - b.z);

	return x + y + z;
}

void Server::disconnect(int id)
{
	clients[id].SetLock();
	closesocket(clients[id].sock.socket);
	clients[id].sock.connected = false;
	clients[id].SetUnlock();
	set_login_off(id);
	for (int i = 1; i <= MAX_USER; ++i) {
		if (false == clients[i].sock.connected) continue;
		send_remove_player_packet(i, id);
	}
	for (int i = 0; i < monsters.size(); ++i)
	{
		if (monsters[i].GetCalculate() == id)
		{
			for (int c = 1; c <= MAX_USER; ++c)
			{
				if (clients[c].sock.connected == false) continue;
				monsters[i].SetCalculate(c);
				for (int to = 1; to <= MAX_USER; ++to)
					send_monster_calculate_packet(to, i, c);
				break;
			}
		}
	}
	cout << "�������� ID:" << id << endl;
}

void Server::already_login_disconnect(int id)
{
	clients[id].SetLock();
	closesocket(clients[id].sock.socket);
	clients[id].sock.connected = false;
	clients[id].SetUnlock();
	cout << "�������� ID:" << id << endl;
}

void Server::worker_thread()
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
		if (EV_RECV == lpover_ex->event) {
			int rest_size = io_byte;
			char *ptr = lpover_ex->messageBuffer;
			int packet_size = 0;
			if (0 < clients[key].sock.prev_size)
			{
				char packet_data[8] = { 0 };
				int Ipos = 0;
				for (Ipos = 0; Ipos < 8; ++Ipos)
				{
					if (124 != clients[key].sock.packet_buf[Ipos])
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
		else if (EV_SEND == lpover_ex->event)
		{
			delete lpover_ex;
		}
		else if (EV_MONSTER_POS == lpover_ex->event)
		{
			T_EVENT ev;
			ev.do_object = static_cast<int>(key);
			ev.event_type = EV_MONSTER_POS;
			process_event(ev);
		}
		else if (EV_MONSTER_DEAD == lpover_ex->event )
		{
			T_EVENT ev;
			ev.do_object = static_cast<int>(key);
			ev.event_type = EV_MONSTER_DEAD;
			process_event(ev);
		}
		else if (EV_MONSTER_REVIVE == lpover_ex->event)
		{
			T_EVENT ev;
			ev.do_object = static_cast<int>(key);
			ev.event_type = EV_MONSTER_REVIVE;
			process_event(ev);
		}
	}
}

void Server::process_event(T_EVENT &ev)
{
	switch (ev.event_type)
	{
	case EV_MONSTER_POS:
	{
		//���� ��ǥ ���Ϳ� �ְ� ������ �°�
		//���� ������� ��� 20.f

		float d = 400.f;
		int target = 0;
		for (int player = 1; player < MAX_USER; ++player)
		{
			if (clients[player].sock.connected == false)
				continue;
			float temp = distance(monsters[ev.do_object].GetPos(), clients[player].GetPos());
			if (temp < d)
			{
				d = temp;
				target = player;
			}
			else
				continue;
		}

		for (int to = 1; to <= MAX_USER; ++to)
		{
			if (clients[to].sock.connected == true)
			{
				send_monster_pos_packet(to, ev.do_object);
				if (target != 0)
				{
					//Ÿ���� ����ɶ� ����
					monsters[ev.do_object].SetLock();
					monsters[ev.do_object].SetTarget(target);
					//monsters[ev.do_object].SetCalculate(target);
					monsters[ev.do_object].SetUnlock();

					send_monster_target_packet(to, ev.do_object, target);
					//send_monster_calculate_packet(to, ev.do_object, target);
				}
				else
				{
					monsters[ev.do_object].SetLock();
					monsters[ev.do_object].SetTarget(target);
					//monsters[ev.do_object].SetCalculate(1);
					monsters[ev.do_object].SetUnlock();
					send_monster_target_packet(to, ev.do_object, target);
				}
			}
		}

	}
	break;

	case EV_MONSTER_REVIVE:
	{
		//��Ƴ�ó���ϱ�
		//monsters[ev.do_object].SetDraw(true);
		for (int to = 1; to <= MAX_USER; ++to)
		{
			if (clients[to].sock.connected == true)
				send_monster_revive_packet(to, ev.do_object);
		}
	}
	break;

	}
}

void Server::add_timer(EVENT_TYPE ev_type, int object, std::chrono::high_resolution_clock::time_point start_time)
{
	timer_lock.lock();
	timer_queue.push(T_EVENT{ start_time, object, ev_type });
	timer_lock.unlock();
}

void Server::do_timer()
{
	auto timer_start = std::chrono::high_resolution_clock::now();
	while (true)
	{
		this_thread::sleep_for(10ms);
		while (true)
		{
			timer_lock.lock();
			if (timer_queue.empty() == true)
			{
				timer_lock.unlock();
				break;
			}
			T_EVENT ev = timer_queue.top();
			timer_lock.unlock();
			if (ev.start_time > std::chrono::high_resolution_clock::now())
				break;

			timer_lock.lock();
			timer_queue.pop();
			timer_lock.unlock();

			OVER_EX *over_ex = new OVER_EX;
			over_ex->event = ev.event_type;
			PostQueuedCompletionStatus(g_iocp, 1, ev.do_object, &over_ex->overlapped);
		}
	}
}

void Server::do_accept()
{
	// Winsock Start - windock.dll �ε�
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		cout << "Error - Can not load 'winsock.dll' file\n";
		return;
	}

	// 1. ���ϻ���  
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET)
	{
		cout << "Error - Invalid socket\n";
		return;
	}

	// �������� ��ü����
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 2. ���ϼ���
	if (::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		cout << "Error - Fail bind\n";
		// 6. ��������
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return;
	}

	// 3. ���Ŵ�⿭����
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		cout << "Error - Fail listen\n";
		// 6. ��������
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
		cout << "���� ���̵�: " << new_id << endl;
		clients[new_id].init(new_id);

		memset(&clients[new_id].sock.over, 0x00, sizeof(struct OVER_EX));
		memset(&clients[new_id].sock.packet_buf, 0x00, MAX_BUFFER);

		clients[new_id].sock.socket = clientSocket;
		clients[new_id].sock.over.dataBuffer.len = MAX_BUFFER;
		clients[new_id].sock.over.dataBuffer.buf = clients[new_id].sock.over.messageBuffer;
		clients[new_id].sock.over.event = EV_RECV;
		flags = 0;

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), g_iocp, new_id, 0);

		//iocp���� �� Ʈ��� �Ѿ��� �׷��� �̰� �Ʒ���
		clients[new_id].sock.connected = true;

		do_recv(new_id);
	}

	// 6-2. ���� ��������
	closesocket(listenSocket);

	// Winsock End
	WSACleanup();

	return;
}
void Server::do_recv(int id)
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


void Server::SendPacket(const int type, const int id, const void *packet, const int packet_size)
{
	if (clients[id].sock.socket != NULL) {
		OVER_EX *over = new OVER_EX;
		ZeroMemory(&over->overlapped, sizeof(over->overlapped));
		over->event = EV_SEND;
		char p_size[MAX_BUFFER]{ 0 };

		// Ŭ���̾�Ʈ���� ��Ŷ ���۽� <��Ŷũ�� | ��Ŷ Ÿ�� 8����Ʈ ���� ������> ���� ������ �Ѵ�.
		itoa(packet_size, p_size, 10);// 10������ ù �Ķ���͸� 2��° �Ķ���Ϳ� ���ڿ��� ��ȯ�Ͽ� ����
		int buf_len = (int)strlen(p_size);// ������ ������ �б� ���� 4����Ʈ�� 4
		p_size[buf_len] = '|';// �����Ѱ� �ڿ� | �ֱ� 4|
		p_size[buf_len + 1] = int(type);// �� �ڿ� Ÿ�� �ֱ� 4|\x1 

		// ��Ŷ ����� �̸� ���ļ� ��������Ѵ�.
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

void Server::process_packet(const int id, const int packet_size, const char * buf)
{
	if (packet_size == 8)
		return;
	//��Ŷũ�� | Ÿ�� ���� 8����Ʈ �׵ڿ� ������
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
		clients[id].SetGameId(game_id);
		clients[id].SetGamePassword(game_pw);

		int ret = check_login(a, b, id);
		if (ret == DB::DB_LOGIN_SUCCESS)
		{
			send_init_packet(id);
			send_my_status_to_all_packet(id);
		}
		else if (ret == DB::DB_ALREADY_LOGIN)
		{
			already_login_disconnect(id);
		}
		else if (ret == DB::DB_SIGNUP)
		{
			send_signup_packet(id);
		}
		else if (ret == DB::DB_LOGIN_FAIL)
		{
			disconnect(id);
		}
	}
	break;
	case CS_CHARACTER_SELECT:
	{
		if (buf[11] == 1)
		{
			clients[id].SetType(1);
		}
		else if (buf[11] == 2)
		{
			clients[id].SetType(2);
		}
		db_set_character(id);
		send_init_packet(id);//����, ������, ��Ÿ �������� ������
		send_my_status_to_all_packet(id);//�������� �ٸ� ��ο���
	}
	break;
	case CS_INFO:
	{

		auto client_Check_info = Game::Protocol::GetClientView(get_packet);
		if (sizeof(client_Check_info) == 0) break;

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
		send_my_status_to_all_packet(id);
	}
	break;
	case CS_GET_ITEM:
	{
		if (get_packet == "") break;
		auto item_check_info = Game::Protocol::GetEatView(get_packet);
		int a = item_check_info->itemID();
		int b = item_check_info->playerID();
		for (int i = 0; i < 4; ++i)
		{
			if (clients[b].GetItem(i) == a)
			{
				clients[b].item_count[i]++;
			}
		}
		for (int i = 1; i <= MAX_USER; ++i)
		{
			if (i == b) continue;
			if (clients[i].sock.connected == false) continue;
			send_remove_item_packet(i, a);
		}
		send_my_status_to_all_packet(id);
	}
	break;
	case CS_MONSTER_STATUS:
	{
		if (get_packet == "") break;
		auto monster_pos = Game::Protocol::GetMonsterView(get_packet);
		int monster_id = monster_pos->id();
		int hp = monster_pos->animator();
		float x = monster_pos->dirX();
		float z = monster_pos->dirZ();
		vec3 p = { 0,0,0 };
		vec3 r = { 0,0,0 };
		if (monster_pos->position()->x() != NULL || monster_pos->position()->y() != NULL || monster_pos->position()->z() != NULL)
		{
			p = { monster_pos->position()->x(), monster_pos->position()->y(), monster_pos->position()->z() };
		}
		if (monster_pos->rotation()->x() != NULL || monster_pos->rotation()->y() != NULL || monster_pos->rotation()->z() != NULL)
			r = { monster_pos->rotation()->x(), monster_pos->rotation()->y(), monster_pos->rotation()->z() };


		monsters[monster_id].SetLock();

		//monsters[monster_id].SetAnimator(hp);
		//monsters[monster_id].SetHp(hp);
		monsters[monster_id].SetDirX(x);
		monsters[monster_id].SetDirZ(z);
		monsters[monster_id].SetPos(p);
		monsters[monster_id].SetRotation(r);

		monsters[monster_id].SetUnlock();
		//��ǥ �޾Ƽ� �ֱ�

		send_my_status_to_all_packet(id);
		OVER_EX *over_ex = new OVER_EX;
		over_ex->event = EV_MONSTER_POS;
		PostQueuedCompletionStatus(g_iocp, 1, monster_id, &over_ex->overlapped);
	}
	break;
	case CS_ATTACK:
	{
		int hp = monsters[buf[11]].GetHp();
		monsters[buf[11]].SetHp(hp - 20);
		//�׾��ٰ� �ٸ� Ŭ��鿡�� �˸���
		if (monsters[buf[11]].GetHp() <= 0)
		{
			monsters[buf[11]].SetDraw(false);
			send_monster_dead_packet(buf[11]);

			// Ÿ�̸ӿ� 1�� �ڷ� �־ �ٽ� ��Ƴ���
			//add_timer(EV_MONSTER_REVIVE, ev.do_object, std::chrono::high_resolution_clock::now() + 60s);
			monsters[buf[11]].SetPos(monsters[buf[11]].GetInitPos());
			monsters[buf[11]].SetHp(100);
			monsters[buf[11]].SetAnimator(0);
			monsters[buf[11]].SetCalculate(1);
			monsters[buf[11]].SetDirX(0);
			monsters[buf[11]].SetDirZ(0);
			monsters[buf[11]].SetTarget(0);
		}

	}
	break;
	case CS_COMPLETE_MAKING:
	{
		if (buf[11] == 1)
		{
			product_complete[0] = 1;
		}
		else if (buf[11] == 2)
		{
			product_complete[1] = 1;
		}
		else if (buf[11] == 3)
		{
			product_complete[2] = 1;
		}
		else if (buf[11] == 4)
		{
			product_complete[3] = 1;
		}
		else if (buf[11] == 5)
		{
			product_complete[4] = 1;
		}
		int t = 0;
		for (int i = 0; i < 5; ++i)
		{
			t = product_complete[i];
			if (t == 1)
				continue;
			else break;
		}
		if (t == 0)
			send_complete_making_packet(buf[11]);
		else
			send_end_game_packet();
	}
	break;
	default:
		break;
	}

}

//------------------------------packet------------------------------

void Server::send_login_fail_packet(int id)
{
	SendPacket(SC_LOGIN_FAIL, id, 0, 0);
}

void Server::send_signup_packet(int id)
{
	SendPacket(SC_SIGNUP, id, 0, 0);
}

void Server::send_my_status_to_all_packet(int id)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	clients[id].SetLock();
	int i = clients[id].GetId();
	int type = clients[id].GetType();
	int hp = clients[id].GetHp();
	int ani = clients[id].GetAnimator();
	float x = clients[i].GetDirX();
	float z = clients[i].GetDirZ();
	auto name = builder.CreateString(clients[id].GetName());
	float h = clients[id].GetHorizontal();
	float v = clients[id].GetVertical();
	Vec3 pos = Vec3(clients[id].GetPos().x, clients[id].GetPos().y, clients[id].GetPos().z);
	Vec3 rot = Vec3(clients[id].GetRotation().x, clients[id].GetRotation().y, clients[id].GetRotation().z);
	clients[id].SetUnlock();
	auto data = CreateClient_info(builder, i, type, hp, ani, x, z, h, v, name, &pos,  &rot);
	builder.Finish(data);
	for (int to = 1; to <= MAX_USER; ++to)
	{
		if (clients[to].sock.connected == true)
			SendPacket(SC_PLAYER_STATUS, to, builder.GetBufferPointer(), builder.GetSize());
	}
}

void Server::send_remove_player_packet(int to, int obj)
{
	int i = obj;
	SendPacket(SC_REMOVE_PLAYER, to, &i, sizeof(i));
}

void Server::send_remove_item_packet(int id, int item)
{
	int i = item;
	SendPacket(SC_REMOVE_ITEM, id, &i, sizeof(i));
}

void Server::send_init_packet(int id)	// ������, ����, ���� �����͸� �ٸ� ������ ������ SC_ID��, ������ SC_INIT_DATA�� ����
{
	int kind = -1;
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
		Vec3 pos = Vec3(items[item_id].GetPos().x, items[item_id].GetPos().y, items[item_id].GetPos().z);
		int d = items[item_id].GetDraw();
		items[item_id].SetUnlock();
		auto data = CreateItem_info(builder, i, t, &pos, d);
		items_data.emplace_back(data);
	}
	auto full_items_data = builder.CreateVector(items_data);

	for (int monster_id = 0; monster_id < MAX_MONSTER; ++monster_id)
	{
		monsters[monster_id].SetLock();
		if (monsters[monster_id].GetDraw() == false)
		{
			monsters[monster_id].SetUnlock();
			continue;
		}
		int ani = monsters[monster_id].GetAnimator();
		int cal = monsters[monster_id].GetCalculate();
		float x = monsters[monster_id].GetDirX();
		float z = monsters[monster_id].GetDirZ();
		int hp = monsters[monster_id].GetHp();
		Vec3 pos =Vec3( monsters[monster_id].GetPos().x, monsters[monster_id].GetPos().y, monsters[monster_id].GetPos().z);
		Vec3 rot = Vec3(monsters[monster_id].GetRotation().x, monsters[monster_id].GetRotation().y, monsters[monster_id].GetRotation().z);
		int target = monsters[monster_id].GetTarget();
		monsters[monster_id].SetUnlock();
		auto data = CreateMonster_info(builder, monster_id, hp, ani, x, z, &pos, &rot);
		monsters_data.emplace_back(data);
	}
	auto full_monsters_data = builder.CreateVector(monsters_data);
	for (int i = 1; i <= MAX_USER; ++i)
	{
		clients[i].SetLock();
		if (clients[i].sock.connected == false || clients[i].GetHp() == 0)
		{
			clients[i].SetUnlock();
			continue;
		}
		if (i == id)
		{
			clients[i].SetUnlock();
			continue;
		}
		int id = clients[i].GetId();
		int type = clients[i].GetType();
		int hp = clients[i].GetHp();
		int ani = clients[i].GetAnimator();
		float x = clients[i].GetDirX();
		float z = clients[i].GetDirZ();
		auto name = builder.CreateString(clients[i].GetName());
		float h = clients[i].GetHorizontal();
		float v = clients[i].GetVertical();
		auto pos = Vec3(clients[i].GetPos().x, clients[i].GetPos().y, clients[i].GetPos().z);
		auto rot = Vec3(clients[i].GetRotation().x, clients[i].GetRotation().y, clients[i].GetRotation().z);

		clients[i].SetUnlock();

		auto data = CreateClient_info(builder, id, type, hp, ani, x, z, h, v, name, &pos, &rot);
		clients_data.emplace_back(data);
	}
	if (clients_data.size() == 0)
	{
		kind = 1;
		clients[id].SetLock();
		int i = clients[id].GetId();
		auto name = builder.CreateString(clients[id].GetName());
		auto pos = Vec3(clients[id].GetPos().x, clients[id].GetPos().y, clients[id].GetPos().z);
		int hp = clients[id].GetHp();
		int mhp = clients[id].GetMaxhp();
		int i1 = clients[id].GetItem(0);
		int i2 = clients[id].GetItem(1);
		int i3 = clients[id].GetItem(2);
		int i4 = clients[id].GetItem(3);
		int t = clients[id].GetType();
		int ic1 = clients[id].item_count[0];
		int ic2 = clients[id].item_count[1];
		int ic3 = clients[id].item_count[2];
		int ic4 = clients[id].item_count[3];
		clients[id].SetUnlock();

		auto mydata = CreateLogin_my_DB(builder, i, name, &pos, hp, mhp, i1, i2, i3, i4, t, ic1, ic2, ic3, ic4);

		auto p = CreateInit_Collection_IM(builder, kind, full_items_data, full_monsters_data, mydata, product_complete[0], product_complete[1], product_complete[2], product_complete[3], product_complete[4]);
		builder.Finish(p);
		SendPacket(SC_ID, id, builder.GetBufferPointer(), builder.GetSize());
	}
	else
	{
		kind = 0;
		auto full_clients_data = builder.CreateVector(clients_data);
		clients[id].SetLock();
		int i = clients[id].GetId();
		auto name = builder.CreateString(clients[id].GetName());
		auto pos = Vec3(clients[id].GetPos().x, clients[id].GetPos().y, clients[id].GetPos().z);
		int hp = clients[id].GetHp();
		int mhp = clients[id].GetMaxhp();
		int i1 = clients[id].GetItem(0);
		int i2 = clients[id].GetItem(1);
		int i3 = clients[id].GetItem(2);
		int i4 = clients[id].GetItem(3);
		int t = clients[id].GetType();
		int ic1 = clients[id].item_count[0];
		int ic2 = clients[id].item_count[1];
		int ic3 = clients[id].item_count[2];
		int ic4 = clients[id].item_count[3];
		clients[id].SetUnlock();
		auto mydata = CreateLogin_my_DB(builder, i, name, &pos, hp, mhp, i1, i2, i3, i4, t, ic1, ic2, ic3, ic4);

		auto p = CreateInit_Collection(builder, kind, full_items_data, full_monsters_data, full_clients_data, mydata, product_complete[0], product_complete[1], product_complete[2], product_complete[3], product_complete[4]);
		builder.Finish(p);
		SendPacket(SC_INIT_DATA, id, builder.GetBufferPointer(), builder.GetSize());
	}
}

void Server::send_monster_dead_packet(int monster)
{
	int i = monster;
	for (int to = 1; to <= MAX_USER; ++to)
	{
		if (clients[to].sock.connected == true)
			SendPacket(SC_REMOVE_MONSTER, to, &i, sizeof(i));
	}
}

void Server::send_monster_revive_packet(int to, int monster)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	monsters[monster].SetLock();
	int hp = monsters[monster].GetHp();
	int ani = monsters[monster].GetAnimator();
	float x = monsters[monster].GetDirX();
	float z = monsters[monster].GetDirZ();
	auto pos = Vec3(monsters[monster].GetPos().x, monsters[monster].GetPos().y, monsters[monster].GetPos().z);
	auto rot = Vec3(monsters[monster].GetRotation().x, monsters[monster].GetRotation().y, monsters[monster].GetRotation().z);
	int target = monsters[monster].GetTarget();
	int calculate = monsters[monster].GetCalculate();
	monsters[monster].SetUnlock();
	auto data = CreateMonster_info(builder, monster, hp, ani, x, z, &pos, &rot, target, calculate);
	builder.Finish(data);
	SendPacket(SC_REVIVE_MONSTER, to, builder.GetBufferPointer(), builder.GetSize());

}

void Server::send_monster_target_packet(int to, int monster, int target)
{
	int a = monster;
	int b = target;

	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	auto data = CreateEat_Item(builder, a, b);
	builder.Finish(data);
	SendPacket(SC_MONSTER_TARGET, to, builder.GetBufferPointer(), builder.GetSize());
}

void Server::send_monster_calculate_packet(int to, int monster, int cal)
{
	int a = monster;
	int b = cal;

	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	auto data = CreateEat_Item(builder, a, b);
	builder.Finish(data);
	SendPacket(SC_MONSTER_CALCULATE, to, builder.GetBufferPointer(), builder.GetSize());
}

void Server::send_monster_pos_packet(int to, int monster)
{
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	monsters[monster].SetLock();
	int ani = monsters[monster].GetAnimator();
	float x = monsters[monster].GetDirX();
	float z = monsters[monster].GetDirZ();
	int hp = monsters[monster].GetHp();
	Vec3 pos = Vec3(monsters[monster].GetPos().x, monsters[monster].GetPos().y, monsters[monster].GetPos().z);
	Vec3 rot = Vec3(monsters[monster].GetRotation().x, monsters[monster].GetRotation().y, monsters[monster].GetRotation().z);
	int target = monsters[monster].GetTarget();
	int calculate = monsters[monster].GetCalculate();
	monsters[monster].SetUnlock();

	auto data = CreateMonster_info(builder, monster, hp, ani, x, z, &pos, &rot, target, calculate);
	builder.Finish(data);

	SendPacket(SC_MONSTER_INFO, to, builder.GetBufferPointer(), builder.GetSize());
}


void Server::send_complete_making_packet(int num)
{
	int i = num;
	for (int to = 1; to <= MAX_USER; ++to)
	{
		if (clients[to].sock.connected == false) continue;
		SendPacket(SC_COMPLETE_MAKING, to, &i, sizeof(i));
	}
}
void Server::send_end_game_packet()
{
	int i = 3;
	for (int to = 1; to <= MAX_USER; ++to)
	{
		if (clients[to].sock.connected == false) continue;
		SendPacket(SC_END_GAME, to, &i, sizeof(i));
	}
}

//------------------------------packet------------------------------

void Server::init_DB()
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
				if (retcode == SQL_ERROR)
					HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
			}
		}
	}
}

void Server::set_login_off(int ci)
{
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	WCHAR Query[MAX_BUFFER];

	wchar_t * game_id = ConvertCtoWC(clients[ci].GetGameId());

	query_lock.lock();
	swprintf((LPWSTR)Query, L"EXEC dbo.user_logout %s, %f, %f, %f, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", game_id, clients[ci].GetXPos(), clients[ci].GetYPos(), clients[ci].GetZPos(), clients[ci].GetHp(), clients[ci].GetItem(0), clients[ci].GetItem(1), clients[ci].GetItem(2), clients[ci].GetItem(3), clients[ci].GetType(), clients[ci].item_count[0], clients[ci].item_count[1], clients[ci].item_count[2], clients[ci].item_count[3]);

	retcode = SQLExecDirect(hstmt, (SQLWCHAR*)Query, SQL_NTS);
	query_lock.unlock();
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
	}
}

int Server::check_login(string a, string b, int id)
{
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	WCHAR Query[MAX_BUFFER];

	wchar_t * game_id = ConvertCtoWC(a.c_str());
	wchar_t * game_pw = ConvertCtoWC(b.c_str());

	query_lock.lock();
	swprintf((LPWSTR)Query, L"EXEC dbo.user_login %s, %s", game_id, game_pw);
	retcode = SQLExecDirect(hstmt, (SQLWCHAR*)Query, SQL_NTS);
	query_lock.unlock();

	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {


		SQLWCHAR sz_id[MAX_STR_LEN], sz_password[MAX_STR_LEN], sz_nickname[MAX_STR_LEN];
		float db_x, db_y, db_z;
		int db_hp, db_maxhp, db_item[4], db_connect, db_character, db_icount[4];
		SQLLEN cb_id = 0, cb_password = 0, cb_nickname = 0, cb_x = 0, cb_y = 0, cb_z = 0, cb_hp = 0, cb_maxhp = 0, cb_item[4]{ 0 }, cb_connect = 0, cb_character = 0, cb_ic[4]{ 0 };

		// Bind columns
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
		retcode = SQLBindCol(hstmt, 14, SQL_INTEGER, &db_character, MAX_STR_LEN, &cb_character);
		retcode = SQLBindCol(hstmt, 15, SQL_INTEGER, &db_icount[0], MAX_STR_LEN, &cb_ic[0]);
		retcode = SQLBindCol(hstmt, 16, SQL_INTEGER, &db_icount[1], MAX_STR_LEN, &cb_ic[1]);
		retcode = SQLBindCol(hstmt, 17, SQL_INTEGER, &db_icount[2], MAX_STR_LEN, &cb_ic[2]);
		retcode = SQLBindCol(hstmt, 18, SQL_INTEGER, &db_icount[3], MAX_STR_LEN, &cb_ic[3]);

		// Fetch and print each row of data. On an error, display a message and exit.  

		retcode = SQLFetch(hstmt);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

			clients[id].SetLock();
			clients[id].SetPos(db_x, db_y, db_z);
			clients[id].SetHp(db_hp);
			clients[id].SetMaxhp(db_maxhp);
			clients[id].SetItem(0, db_item[0]);
			clients[id].SetItem(1, db_item[1]);
			clients[id].SetItem(2, db_item[2]);
			clients[id].SetItem(3, db_item[3]);
			clients[id].SetDraw(true);
			clients[id].SetType(db_character);
			clients[id].item_count[0] = db_icount[0];
			clients[id].item_count[1] = db_icount[1];
			clients[id].item_count[2] = db_icount[2];
			clients[id].item_count[3] = db_icount[3];
			clients[id].SetUnlock();
			if (db_connect)
			{
				//������� ��
				return DB_ALREADY_LOGIN;
			}
			//connect 1�� �ٲٱ�
			return DB_LOGIN_SUCCESS;
		}
		else if (retcode == SQL_ERROR)
		{
			return DB_SIGNUP;
		}
	}
	if (retcode == SQL_ERROR)
	{

		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		return DB_LOGIN_FAIL;
	}
}

int Server::load_item()
{
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	WCHAR Query[MAX_BUFFER];

	query_lock.lock();
	wsprintf((LPWSTR)Query, L"EXEC dbo.get_item_info");

	retcode = SQLExecDirect(hstmt, (SQLWCHAR *)Query, SQL_NTS);
	query_lock.unlock();
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
				items[db_id].SetId(db_id);
				items[db_id].SetPos(db_x, db_y, db_z);
				items[db_id].SetType(db_type);
				items[db_id].SetDraw(db_draw);
			}
			else
				break;
		}
		if (items.size() <= 0)
		{
			return DB_NO_DATA;
		}
		else
		{
			return DB_LOGIN_SUCCESS;
		}
	}
}

void Server::insert_item_db(int id, int type, float x, float y, float z, int draw)
{
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	WCHAR Query[MAX_BUFFER];

	swprintf((LPWSTR)Query, L"EXEC dbo.insert_item %d, %d, %f, %f, %f, %d", id, type, x, y, z, draw);

	retcode = SQLExecDirect(hstmt, (SQLWCHAR *)Query, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{

	}
	if (retcode == SQL_ERROR)
	{
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);

	}
}

void Server::HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode)
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

void Server::db_set_pos(int id)
{
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	WCHAR Query[MAX_BUFFER];

	wchar_t * game_id = ConvertCtoWC(clients[id].GetGameId());

	query_lock.lock();
	swprintf((LPWSTR)Query, L"EXEC dbo.user_set_pos %s, %f, %f, %f ", game_id, clients[id].GetXPos(), clients[id].GetYPos(), clients[id].GetZPos());

	retcode = SQLExecDirect(hstmt, (SQLWCHAR *)Query, SQL_NTS);
	query_lock.unlock();
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{

	}
	if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
	{
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);

	}
}

void Server::db_set_character(int id)
{
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	WCHAR Query[MAX_BUFFER];

	wchar_t * game_id = ConvertCtoWC(clients[id].GetGameId());

	query_lock.lock();
	swprintf((LPWSTR)Query, L"EXEC dbo.user_login %s, %d", game_id, clients[id].GetType());
	retcode = SQLExecDirect(hstmt, (SQLWCHAR*)Query, SQL_NTS);
	query_lock.unlock();
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{

	}
	if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
	{
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);

	}
}

void Server::autosave_info_db()
{
	while (true)
	{
		for (int i = 1; i <= MAX_USER; ++i)
		{
			if (clients[i].sock.connected&&clients[i].GetId() != 0)
			{
				if (clients[i].save_time + 10s <= std::chrono::high_resolution_clock::now())
				{
					db_set_pos(i);
					clients[i].save_time = std::chrono::high_resolution_clock::now();
				}
			}
		}
	}
}

wchar_t* Server::ConvertCtoWC(const char *str)
{
	assert(str);
	size_t nLen = strlen(str) + 1;
	WCHAR* wText = (LPWSTR)new WCHAR[sizeof(WCHAR)*nLen];
	int RetVal = MultiByteToWideChar(949, 0, str, -1, wText, nLen);
	return wText;
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