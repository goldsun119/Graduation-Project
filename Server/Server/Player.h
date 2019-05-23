#pragma once
#include "Obj.h"
#include <winsock2.h>
#include <mutex>
#include "protocol.h"

#define MAX_BUFFER        10000

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
	int id;

public:
	SOCKETINFO sock;
	Player();
	~Player();

	void init(int id);
	int GetHp();
	void SetHp(int a);

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
	
	int GetId();
	
};