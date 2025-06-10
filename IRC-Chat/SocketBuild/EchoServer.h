#pragma once

#include "TCPSocket.h"
#include "MessageManager.h"

/*
** 클래스명	:	EchoServer.
** 설명		:	에코 서버의 전체적인 동작을 관리하는 클래스입니다.
**				서버 시작, 클라이언트 처리, 서버 종료를 담당합니다.
** 초기화	:	int(포트번호)를 매개변수로 받는 생성자만 가능.
*/
class EchoServer
{
	public:
		// 해당 클래스의 메소드의 결과를 나타내는 enum class입니다.
		enum class Result
		{
			SUCCESS,
			FAIL_START,
			FAIL_ACCEPT,
			CLIENT_HANDLED,
			CLIENT_ERROR
		};

	public:
		/*
		** 함수명	:	EchoServer.
		** 설명		:	EchoServer 객체 생성자.
		** 인자		:	port(int) : 서버 포트 번호.
		** 반환값	:	없음.
		*/
		explicit EchoServer(int port);

		/*
		** 함수명	:	~EchoServer.
		** 설명		:	EchoServer 객체 소멸자.
		** 인자		:	없음.
		** 반환값	:	없음.
		*/
		~EchoServer();


		// 복사 생성자 및 복사 할당 연산자 삭제.
		EchoServer(const EchoServer& obj) = delete;
		EchoServer& operator=(const EchoServer& obj) = delete;

		// 이동 생성자 및 이동 할당 연산자 삭제.
		EchoServer(EchoServer&& obj) = delete;
		EchoServer& operator=(EchoServer&& obj) = delete;

	public:
		/*
		** 함수명	:	startServer.
		** 설명		:	Echo 서버를 시작합니다.
		** 인자		:	없음.
		** 반환값	:	EchoServer::Result : 성공/실패 결과.
		*/
		EchoServer::Result startServer();
		
		/*
		** 함수명	:	awaitClientConnect.
		** 설명		:	클라이언트와 연결을 대기합니다, 클라이언트 연결을 수락하고 처리합니다.
		** 인자		:	없음.
		** 반환값	:	EchoServer::Result : 성공/실패 결과.
		*/
		EchoServer::Result awaitClientConnect();

		/*
		** 함수명	:	stop.
		** 설명		:	Echo 서버를 중단합니다.
		** 인자		:	없음.
		** 반환값	:	없음.
		*/
		void stop();

		/*
		** 함수명	:	isRunning.
		** 설명		:	서버 실행 상태를 확인합니다.
		** 인자		:	없음.
		** 반환값	:	bool : 실행 중이면 true, 아니면 false.
		*/
		bool isRunning() const;

	private:
		int _port;
		TCPSocket _tcpSocket;
		bool _isRunning;


		/*
		** 함수명	:	handleClientMessages.
		** 설명		:	클라이언트와 메시지를 주고받습니다.
		** 인자		:	SOCKET client_socket : 클라이언트 소켓.
		** 반환값	:	EchoServer::Result : 성공/실패 결과.
		*/
		EchoServer::Result handleClientMessages(SOCKET client_socket);

		/*
		** 함수명	:	processMessage.
		** 설명		:	받은 메시지를 처리합니다.
		** 인자		:	MessageManager& handler : 메시지 핸들러.
		*				MessageManager::Result result : 수신 결과.
		** 반환값	:	EchoServer::Result : 성공/실패 결과.
		*/
		EchoServer::Result processMessage(MessageManager& handler, MessageManager::Result result);
};