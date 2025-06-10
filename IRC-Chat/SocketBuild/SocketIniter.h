#pragma once
#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

/*
** 클래스명	:	SocketIniter.
** 설명		:	Socket을 사용할 수 있도록 dll과 os에 소켓 부분을 초기화 해줍니다.
** 초기화	:	defalut 초기화만 허용.
*/
class SocketIniter
{
	public:
	
		// 해당 클래스의 메소드의 결과를 나타내는 enum class입니다.
		enum class Result
		{
			FAIL_SOCKET,
			SUCCESS_SOCKET
		};
	
	public:

		/*
		** 메소드명	:	SocketIniter.
		** 설명		:	SocketIniter 객체 생성자.
		*				wsaData 멤버변수 값을 기본값으로 초기화합니다.
		** 인자값	:	없음.
		** 출력값	:	없음.
		*/
		SocketIniter();
		/*
		** 메소드명	:	~SocketIniter.
		** 설명		:	~SocketIniter 객체 소멸자.
		*				WSACleanup을 호출해서 winsock을 해제합니다.
		** 인자값	:	없음.
		** 출력값	:	없음.
		*/
		~SocketIniter();
	
		/*
		** 메소드명	:	init.
		** 설명		:	Winsock을 사용할 수 있게 WSAStartup 함수를 호출합니다. 
		*				성공 실패 log를 출력하고, 성공 시 wsaData 정보를 출력합니다.
		** 인자값	:	없음.
		** 출력값	:	true, false.
		*/
		SocketIniter::Result init();
	
		/*
		** 메소드명	:	wsaDataToString.
		** 설명		:	wsaData의 정보들을 문자열로 만들어서 리턴합니다.
		** 인자값	:	없음.
		** 출력값	:	string(wsaData의 정보들을 문자열).
		*/
		std::string wsaDataToString() const;
	
	private:
		// winsock 정보를 담고 있는 객체.
		WSADATA _wsaData;
};