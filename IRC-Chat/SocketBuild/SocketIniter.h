#pragma once
#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

/*
** 클래스명	: SocketIniter
** 설명		: Socket을 사용할 수 있도록 dll과 os에 소켓 부분을 초기화 해줍니다.
** 초기화	: defalut 초기화만 허용.
*/
class SocketIniter
{
public:
	SocketIniter();
	~SocketIniter();

	bool init();

	std::string wsaDataToString() const;

private:
	WSADATA _wsaData;

};