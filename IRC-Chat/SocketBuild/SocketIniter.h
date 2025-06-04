#pragma once
#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

/*
** Ŭ������	: SocketIniter
** ����		: Socket�� ����� �� �ֵ��� dll�� os�� ���� �κ��� �ʱ�ȭ ���ݴϴ�.
** �ʱ�ȭ	: defalut �ʱ�ȭ�� ���.
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