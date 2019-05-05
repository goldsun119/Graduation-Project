#pragma once
#include "Obj.h"
#include <winsock2.h>
#include "protocol.h"

#define MAX_BUFFER        1024

struct OVER_EX
{
	WSAOVERLAPPED		overlapped;
	WSABUF					dataBuffer;
	char							messageBuffer[MAX_BUFFER];
	bool							is_recv;

};

struct SOCKETINFO // 클라이언트 정보 - 연결이 되어있으면 무조건 소켓이 있는 것이다.
{
	bool			connected; // 
	OVER_EX	over;
	SOCKET	socket;
	char			packet_buf[MAX_BUFFER]; // 
	int				prev_size;
};

class Player: public Obj
{
private:
	Vec3 rotation;
	float vertical;
	float horizontal;
	int animator;
	int hp;
	int id;

public:
	SOCKETINFO sock;
	Player();
	~Player();

	void init(int id);
	int GetHp();
	void SetHp(int a);

	Vec3 GetRotation();
	void SetRotation(Vec3 a);

	float GetVertical();
	void SetVertical(float a);
	
	float GetHorizontal();
	void SetHorizontal(float a);
	
	int GetAnimator();
	void SetAnimator(int a);
	
	int GetId();
	
};