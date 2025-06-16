#pragma once

#include <WinSock2.h>

/*
** 클래스명	:	TCPSocket.
** 설명		:	TCPSocket을 create, listen, bind, accept는 클래스입니다.
** 초기화	:	defalut 생성자만 허용.
*/
class TCPSocket
{
	public :
		enum class Result
		{
			SUCCESS,
			FAIL_CREATE,
			FAIL_BIND,
			FAIL_LISTEN,
			FAIL_ACCEPT
		};
		
		/*
		** 메소드명	:	TCPSocket.
		** 설명		:	TCPSocket 객체 생성자.
		*				_tcpSocket 멤버변수 값을 INVALID_SOCKET으로 초기화합니다.
		** 인자값	:	없음.
		** 출력값	:	없음.
		*/
		TCPSocket();

		/*
		** 메소드명	:	~TCPSocket.
		** 설명		:	~TCPSocket 객체 소멸자.
		*				소켓이 열여있다면 닫아줍니다.
		** 인자값	:	없음.
		** 출력값	:	없음.
		*/
		~TCPSocket();

		// 복사 생성자 및 복사 할당 연산자 삭제.
		TCPSocket(const TCPSocket& obj) = delete;
		TCPSocket& operator=(const TCPSocket& obj) = delete;

		// 이동 생성자 및 이동 할당 연산자 삭제.
		TCPSocket(TCPSocket&& obj) = delete;
		TCPSocket& operator=(TCPSocket&& obj) = delete;

	public:
		/*
		** 함수명	:	createTCPSocket.
		** 설명		:	tcp 소켓을 생성합니다.
		** 인자		:	없음.
		** 반환값	:	TCPSocket::Result : 성공/실패 결과.
		*/
		TCPSocket::Result createTCPSocket();

		/*
		** 함수명	:	bindSocket.
		** 설명		:	소켓을 지정된 포트에 바인드합니다. (네트워크 주소 IP + Port에 등록).
		** 인자		:	int port - 바인드할 포트 번호.
		** 반환값	:	TCPSocket::Result : 성공/실패 결과.
		*/
		TCPSocket::Result bindTCPSocket(int port) const;

		/*
		** 함수명	:	startListen.
		** 설명		:	소켓을 listen상태로 만듭니다.
		** 인자		:	없음.
		** 반환값	:	TCPSocket::Result : 성공/실패 결과.
		*/
		TCPSocket::Result startListen();

		/*
		** 함수명	:	acceptConnection.
		** 설명		:	연결을 요청한 클라이언트를 연결하고 클라이언트와 연결된 소켓을 생성합니다.
		** 인자		:	없음.
		** 반환값	:	SOCKET : 클라이언트 소켓 (실패 시 INVALID_SOCKET).
		*/
		SOCKET acceptConnection();

		/*
		** 함수명	:	closeSocket.
		** 설명		:	소켓을 닫습니다.
		** 인자		:	없음.
		** 반환값	:	없음.
		*/
		void closeTCPSocket();

		/*
		** 함수명	:	isValid.
		** 설명		:	소켓이 유효한지 확인합니다.
		** 인자		:	없음.
		** 반환값	:	bool : 유효하면 true, 아니면 false.
		*/
		bool isValid() const;

		/*
		** 함수명	:	getSocket.
		** 설명		:	내부 소켓 핸들을 반환합니다.
		** 인자		:	없음.
		** 반환값	:	SOCKET : 소켓 핸들.
		*/
		SOCKET getSocket() const;

	private:
		SOCKET _tcpSocket;

		/*
		** 함수명	:	logClientInfo.
		** 설명		:	클라이언트 정보를 로그에 출력합니다.
		** 인자		:	const sockaddr_in& addr : 클라이언트 주소 정보.
		** 반환값	:	없음.
		*/
		void logClientInfo(const sockaddr_in& addr);
};
