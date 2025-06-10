#include "TCPSocket.h"
#include "DebugHelper.h"
#include <ws2tcpip.h>

TCPSocket::TCPSocket()
	: _tcpSocket(INVALID_SOCKET)
{
}

TCPSocket::~TCPSocket()
{
	closeTCPSocket();
}

TCPSocket::Result TCPSocket::createTCPSocket()
{
	// socket함수를 사용해서 TCP 소켓을 생성합니다.
	// AF_INET - IPv4를 의미.
	// SOCK_STREAM - TCP 방식을 스트림 사용.
	// IPPROTO_TCP - TCP 프로토콜 사용.
	this->_tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 소켓이 제대로 생성되었는지 확인.
	if (this->_tcpSocket == INVALID_SOCKET)
	{
		LOG_ERROR("tcp소켓 생성을 실패했습니다.");
		return (TCPSocket::Result::FAIL_CREATE);
	}
	LOG_INFO("tcp소켓 생성을 성공했습니다.");
	return (TCPSocket::Result::SUCCESS);
}

TCPSocket::Result TCPSocket::bindTCPSocket(int port) const
{
	// 네트워크 주소 정보를 담는 구조체.
	// sin_family - 주소 체계 (IPv4, IPv6). AF_INET은 IPv4를 의미.
	// sin_addr.s_addr - 수신 IP, INADDR_ANY는 모든 IP에서 수신하겠다는 의미.
	// sin_port - 포트 번호. htons - 네트워크 바이트 순서(빅엔디안)으로 변환해주는 함수.
	sockaddr_in server_addr = {};
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	// 소켓과 네트워크 주소를 바인드해줍니다.
	// (sockaddr*)로 바꾸는 이유는 c언어가 다형성이 없기 때문입니다.
	if (bind(this->_tcpSocket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		LOG_ERROR("소켓 바인드 실패했습니다.\n에러코드: " + std::to_string(WSAGetLastError()));
		return (TCPSocket::Result::FAIL_BIND);
	}
	LOG_INFO("소켓이 포트 " + std::to_string(port) + "에 바인드되었습니다.");
	return (TCPSocket::Result::SUCCESS);
}

TCPSocket::Result TCPSocket::startListen()
{
	// 소켓을 수신 대기 상태로 전환해줍니다.
	// 매개변수 두번 째(int backlog)는 최대 연결 대기 큐의 크기를 의미힙니다.
	// 대기 큐가 크면 많은 클라이언트가 연결을 요청해도 대기한 뒤 연결할 수 있습니다. 
	// SOMAXCONN는 시스템에서 사용할 수 잇는 최대값입니다.
	if (listen(this->_tcpSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		LOG_ERROR("리슨 대기 시작이 실패했습니다.\n에러코드: " + std::to_string(WSAGetLastError()));
		return (TCPSocket::Result::FAIL_LISTEN);
	}

	LOG_INFO("소켓이 클라이언트 연결을 대기하고 있습니다.");
	return (TCPSocket::Result::SUCCESS);
}

SOCKET TCPSocket::acceptConnection()
{
	// 클라이언트와 연결될 소켓입니다.
	SOCKET clientSocket = INVALID_SOCKET;

	// 클라이언트 소켓에 대한 정보를 담을 구조체입니다.
	sockaddr_in client_addr = {};
	int addr_len = sizeof(client_addr);

	// accept 함수 실행 시 대기 큐에 있던 클라이언트 요청을 하나 꺼내옵니다.
	// accept 함수는 동기함수 임으로 클라이언트 연결까지 해당 함수에서 대기하게 됩니다.
	// 소켓 구조체를 새로 생성하여 커널에 등록합니다. 내부 정보를 클라이언트 IP/Port 정보를 입력합니다.
	// 소켓을 반환하고 반환된 소켓은 send()/recv()로 해당 클라이언트와 통신이 가능합니다.
	clientSocket = accept(this->_tcpSocket, (sockaddr*)&client_addr, &addr_len);
	if (clientSocket == INVALID_SOCKET)
	{
		LOG_ERROR("클라이언트와 연결에 실패했습니다.\n에러코드: " + std::to_string(WSAGetLastError()));
		return (INVALID_SOCKET);
	}

	// 생성된 클라이언트 소켓에 대한 내부정보를 출력합니다.
	this->logClientInfo(client_addr);
	return (clientSocket);
}

void TCPSocket::closeTCPSocket()
{
	// 소켓이 열여있는지 확인합니다.
	if (this->_tcpSocket != INVALID_SOCKET)
	{
		// 열여있다면 소켓을 닫고 INVALID_SOCKET으로 초기화합니다.
		closesocket(this->_tcpSocket);
		this->_tcpSocket = INVALID_SOCKET;
		LOG_DEBUG("소켓이 닫혔습니다");
	}
}

bool TCPSocket::isValid() const
{
	return (this->_tcpSocket != INVALID_SOCKET);
}

void TCPSocket::logClientInfo(const sockaddr_in& addr)
{
	// ip를 담을 배열.
	char client_ip[INET_ADDRSTRLEN];

	// inet_ntop는 바이트 배열을 리틀엔디안으로 변경합니다.
	inet_ntop(AF_INET, &addr.sin_addr, client_ip, INET_ADDRSTRLEN);

	// ntohs 역시 바이트 배열을 리틀엔디안으로 변경합니다.
	std::string client_info = std::string(client_ip) + ":" + std::to_string(ntohs(addr.sin_port));
	LOG_INFO("클라이언트가 연결되었습니다.\n네트워크 주소: " + client_info);
}
