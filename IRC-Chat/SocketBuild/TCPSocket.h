#pragma once
#pragma execution_character_set("utf-8")

/**
 * @file TCPSocket.h
 * @brief TCP 리스닝 소켓을 제어하는 TCPSocket 클래스를 선언합니다.
 * @author 최성락
 * @date 2025-06-17
 * 
 * @details
 * TCPSocket 클래스는 소켓 생성, 포트 바인딩, 연결 대기, 클라이언트 수락, 소켓 종료와 같은 서버 소켓 설정 작업을 수행하는 메서드들을 제공합니다.
 * <br>이를 통해 저수준의 소켓 호출을 단순한 인터페이스로 추상화할 수 있습니다.
 */

#include <WinSock2.h>

 /**
  * @class TCPSocket
  * @brief TCP 서버 소켓과 그 기본 동작(생성, 바인드, 연결 대기, 연결 수락, 종료)을 캡슐화합니다.
  *
  * @details
  * 이 클래스는 서버에서 클라이언트의 접속을 받는 리스닝 소켓을 처리하는 데 사용됩니다. 
  * <br>각 단계별 문제를 표시하는 오류 상태 값(Result)을 제공합니다.
  * <br>TCPSocket을 사용하면 상위 수준의 서버 코드(MultiServer)가 네트워킹을 보다 높은 추상화 수준에서 처리할 수 있습니다.
  */
class TCPSocket
{
	public :
		/**
		 * @enum TCPSocket::Result
		 * @brief TCPSocket 연산에 대한 결과 상태 값입니다.
		 */
		enum class Result
		{
			SUCCESS,		///< 연산 성공.
			FAIL_CREATE,	///< 소켓 생성 실패.
			FAIL_BIND,		///< 소켓 바인드 실패.
			FAIL_LISTEN,	///< 리스닝 모드 전환 실패.
			FAIL_ACCEPT		///< 새 연결 수락 실패.
		};
		
		/**
		 * @fn TCPSocket::TCPSocket()
		 * @brief TCPSocket 객체를 생성하고 소켓 핸들을 INVALID_SOCKET으로 초기화합니다.
		 * @return 없음.
		 * 
		 * @details
		 * 내부 SOCKET을 INVALID_SOCKET으로 설정합니다.
		 
		 * @note 
		 * 생성 후 실제 소켓을 만들려면 createTCPSocket()을 호출해야합니다.
		 */
		TCPSocket();

		/**
		 * @fn TCPSocket::~TCPSocket()
		 * @brief 소멸자입니다. 소켓이 열려 있는 경우 해당 소켓을 닫습니다.
		 * @return 없음.
		 */
		~TCPSocket();

		// 복사 생성자 및 복사 할당 연산자 삭제.
		TCPSocket(const TCPSocket& obj) = delete;
		TCPSocket& operator=(const TCPSocket& obj) = delete;

		// 이동 생성자 및 이동 할당 연산자 삭제.
		TCPSocket(TCPSocket&& obj) = delete;
		TCPSocket& operator=(TCPSocket&& obj) = delete;

	public:
		/**
		 * @fn TCPSocket::Result TCPSocket::createTCPSocket()
		 * @brief 새로운 TCP 소켓을 생성합니다.
		 * @return TCPSocket::Result : 소켓 생성에 성공하면 SUCCESS, 실패하면 FAIL_CREATE를 반환합니다.
		 *
		 * @note
		 * 내부적으로 socket(...) 함수(winsock API)를 호출합니다. 
		 * <br>이 메서드는 바인드나 리스닝을 수행하기 전에 호출되어야 합니다.
		 */
		TCPSocket::Result createTCPSocket();

		/**
		 * @fn TCPSocket::Result TCPSocket::bindTCPSocket(int port) const
		 * @brief 소켓을 로컬 포트에 바인드하여 연결 요청을 받을 수 있게 합니다.
		 * @param[IN] int port : 소켓을 바인드할 포트 번호입니다.
		 * @return TCPSocket::Result : 바인드에 성공하면 SUCCESS, 실패하면 FAIL_BIND를 반환합니다.
		 *
		 * @note
		 * 주어진 포트로 모든 네트워크 인터페이스(INADDR_ANY)에 바인드합니다. 
		 * <br>이 함수를 호출하기 전에 소켓이 createTCPSocket()으로 생성되어 있어야 합니다.
		 */
		TCPSocket::Result bindTCPSocket(int port) const;

		/**
		 * @fn TCPSocket::Result TCPSocket::startListen()
		 * @brief 소켓을 수신 대기 모드로 전환하여 들어오는 연결을 받을 수 있게 합니다.
		 * @return TCPSocket::Result : 리스닝을 시작하는 데 성공하면 SUCCESS, 오류가 발생하면 FAIL_LISTEN을 반환합니다.
		 * 
		 * @note 
		 * 이 함수를 호출하기 전에 소켓이 생성되고 바인드되어 있어야 합니다.
		 */
		TCPSocket::Result startListen();

		/**
		 * @fn SOCKET TCPSocket::acceptConnection()
		 * @brief 들어오는 클라이언트 연결을 수락합니다.
		 * @return SOCKET : 수락된 연결에 대한 새로운 클라이언트 소켓을 반환합니다. 
		 * <br>연결이 수락되지 않았거나 오류가 발생한 경우 INVALID_SOCKET을 반환합니다.
		 *
		 * @note
		 * 들어오는 연결을 대기합니다. (리스닝 소켓에 대해 select()가 읽기 가능 이벤트를 나타낸 후 호출되어야 합니다.)
		 * <br>성공 시 유효한 클라이언트용 SOCKET을 반환하며, 이 소켓은 ClientManager가 관리해야 합니다.
		 */
		SOCKET acceptConnection();

		/**
		 * @fn void TCPSocket::closeTCPSocket()
		 * @brief 리스닝 소켓을 닫습니다.
		 * @return 없음.
		 * 
		 * @note 
		 * 서버를 종료하거나 더 이상 연결을 받지 않을 때 이 함수를 호출해야 합니다.
		 */
		void closeTCPSocket();

		/**
		 * @fn bool TCPSocket::isValid() const
		 * @brief 소켓이 유효한지 확인합니다.
		 * @return bool : 소켓이 유효하여(INVALID_SOCKET이 아닌 경우) true를 반환하고, 생성되지 않았거나 이미 닫힌 경우 false를 반환합니다.
		 */
		bool isValid() const;

		/**
		 * @fn SOCKET TCPSocket::getSocket() const
		 * @brief 내부 소켓 핸들을 가져옵니다.
		 * @return SOCKET : 해당 객체가 관리하는 Socket을 반환합니다.
		 */
		SOCKET getSocket() const;

	private:
		/// TCP 서버 소켓(WinSock 소켓).
		SOCKET _tcpSocket;

		/**
		 * @fn void TCPSocket::logClientInfo(const sockaddr_in& addr)
		 * @brief 연결된 클라이언트의 IP 주소와 포트 정보를 로그에 출력합니다.
		 * @param[IN] const sockaddr_in& addr : 클라이언트의 주소 정보가 담긴 sockaddr_in 구조체입니다.
		 * @return 없음.
		 *
		 * @details
		 * 연결된 클라이언트의 IP 주소와 포트 번호를 출력합니다.
		 */
		void logClientInfo(const sockaddr_in& addr);
};
