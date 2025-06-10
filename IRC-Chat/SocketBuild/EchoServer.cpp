#include "EchoServer.h"
#include "DebugHelper.h"

EchoServer::EchoServer(int port)
    : _port(port), _tcpSocket(), _isRunning(false)
{
    LOG_INFO("EchoServer 객체가 생성되었습니다.\n포트번호: " + std::to_string(port));
}

EchoServer::~EchoServer()
{
    this->stop();
    LOG_INFO("EchoServer 객체가 소멸되었습니다.");
}

EchoServer::Result EchoServer::startServer()
{
    LOG_INFO("Echo 서버를 시작합니다");

    // IPv4, tcp 소켓을 생성합니다.
    if (this->_tcpSocket.createTCPSocket() != TCPSocket::Result::SUCCESS)
    {
        return (EchoServer::Result::FAIL_START);
    }

    // 네트워크 주소(IP/Port)와 소켓을 바인딩합니다.
    if (this->_tcpSocket.bindTCPSocket(this->_port) != TCPSocket::Result::SUCCESS)
    {
        return (EchoServer::Result::FAIL_START);
    }

    // 소켓을 수신 대기 상태로 전환합니다.
    if (this->_tcpSocket.startListen() != TCPSocket::Result::SUCCESS)
    {
        return (EchoServer::Result::FAIL_START);
    }

    // 서버가 클라이언트와 통신할 준비가 되었음을 표시.
    this->_isRunning = true;
    return (EchoServer::Result::SUCCESS);
}

EchoServer::Result EchoServer::awaitClientConnect()
{
    SOCKET client_socket = INVALID_SOCKET;

    LOG_INFO("클라이언트 연결을 기다리고 있습니다..");

    // 클라이언트와 연결이 완료되면 client_socket을 리턴받습니다.
    client_socket = this->_tcpSocket.acceptConnection();
    if (client_socket == INVALID_SOCKET)
    {
        return (EchoServer::Result::FAIL_ACCEPT);
    }

    // 클라이언트 소켓을 통해 클라이언트와 메세지를 주고 받습니다. (내부 코드에서 루핑 중).
    // 클라이언트가 정상종료되거나 비정상 연결이 될 경우 클라이언트 소켓을 닫습니다.
    EchoServer::Result handle_result = this->handleClientMessages(client_socket);
    closesocket(client_socket);

    return (handle_result);
}

void EchoServer::stop()
{
    // Echo 서버의 _isRunning이 true일 경우 false로 초기화합니다.
    if (this->_isRunning)
    {
        this->_isRunning = false;
        LOG_INFO("서버를 중단합니다.");
    }
}

bool EchoServer::isRunning() const
{
    // Echo 서버가 실행 중인지 체크합니다.
    return (this->_isRunning);
}

EchoServer::Result EchoServer::handleClientMessages(SOCKET client_socket)
{
    LOG_INFO("클라이언트와의 통신을 시작합니다. ('quit' 입력시 종료)");

    // 메세지 처리를 담당할 MessageManager 객체를 생성합니다.
    MessageManager handler(client_socket);

    // 특정 조건(클라이언트가 quit 메세지를 보내거나, 클라이언트와 비정상적으로 끊어진 경우)이 아닌경우 루프합니다.
    while (this->_isRunning)
    {
        // 메세지 매니저 객체가 클라이언트 소켓의 리드 버퍼를 읽습니다.
        MessageManager::Result recv_result = handler.receiveMessage();

        // recv_result에 따라 정상진행과 오류를 처리합니다.
        // 메세지 매니저 객체가 클라이언트 소켓의 샌드 버퍼에 바이트를 전송합니다.
        EchoServer::Result process_result = this->processMessage(handler, recv_result);

        // send와 recv 간 오류가 발생하면 에러 결과를 리턴합니다.
        if (process_result != EchoServer::Result::SUCCESS)
        {
            return (process_result);
        }
    }

    return (EchoServer::Result::SUCCESS);
}

EchoServer::Result EchoServer::processMessage(MessageManager& handler, MessageManager::Result result)
{
    switch (result)
    {
        case MessageManager::Result::SUCCESS:
            if (handler.sendEcho() != MessageManager::Result::SUCCESS)
            {
                return Result::CLIENT_ERROR;
            }
            return (EchoServer::Result::SUCCESS);
    
        // 정상 종료의 경우. (quit).
        case MessageManager::Result::CLIENT_QUIT:
            LOG_INFO("클라이언트가 연결 종료를 요청했습니다.");
            handler.sendGoodbye();
            return (EchoServer::Result::CLIENT_HANDLED);

        case MessageManager::Result::CLIENT_DISCONNECTED:
            return (EchoServer::Result::CLIENT_HANDLED);

        case MessageManager::Result::FAIL_RECEIVE:
            LOG_ERROR("메시지 수신 중 오류가 발생했습니다.");
            return (EchoServer::Result::CLIENT_ERROR);

        default:
            return (EchoServer::Result::CLIENT_ERROR);
    }
}