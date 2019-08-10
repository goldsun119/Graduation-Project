#pragma once
#include "Obj.h"
#include <winsock2.h>
#include <mutex>
#include <chrono>
#include "protocol.h"

#define MAX_BUFFER        10000

enum EVENT_TYPE {
	EV_RECV,
	EV_SEND,
	EV_MONSTER_POS,
	EV_MONSTER_DEAD,
	EV_MONSTER_REVIVE

};

struct OVER_EX
{
	WSAOVERLAPPED		overlapped;
	WSABUF					dataBuffer;
	char							messageBuffer[MAX_BUFFER];
	EVENT_TYPE			event;

};

struct SOCKETINFO // 클라이언트 정보 - 연결이 되어있으면 무조건 소켓이 있는 것이다.
{
	bool			connected; // 
	OVER_EX	over;
	SOCKET	socket;
	char			packet_buf[MAX_BUFFER]; // 
	int				prev_size;
	std::mutex send_lock;
};

class Player: public Obj
{
private:
	vec3 rotation;
	float vertical;
	float horizontal;
	int animator;
	float dirX;
	float dirZ;
	int hp;
	int maxhp;
	int id;
	char nickname[10];
	char game_password[10];
	char game_id[10];
	int item[4];

public:
	SOCKETINFO sock;
	std::chrono::high_resolution_clock::time_point save_time;
	Player();
	~Player();

	void init(int id);
	int GetHp();
	void SetHp(int a);


	int GetItem(int index);
	void SetItem(int index, int a);

	int GetMaxhp();
	void SetMaxhp(int a);

	vec3 GetRotation();
	void SetRotation(vec3 a);

	float GetVertical();
	void SetVertical(float a);
	
	float GetHorizontal();
	void SetHorizontal(float a);
	
	int GetAnimator();
	void SetAnimator(int a);

	float GetDirX();
	void SetDirX(float a);

	float GetDirZ();
	void SetDirZ(float a);

	char* GetNickname();
	void SetNickname(char *a);

	char* GetGameId();
	void SetGameId(char *a);

	char* GetGamePassword();
	void SetGamePassword(char *a);
	
	int GetId();
	
};