#include "MultiServer.h"
#include "DebugHelper.h"

MultiServer::MultiServer(int port)
    : _port(port), _tcpSocket(), _clientManager(), _selectManager(), _messageSender(), _isRunning(false)
{
    LOG_INFO("MultiServer 객체가 생성되었습니다.\n포트번호: " + std::to_string(port));
}

MultiServer::~MultiServer()
{
    this->stop();
    LOG_INFO("MultiServer 객체가 소멸되었습니다.");
}

MultiServer::Result MultiServer::startServer()
{
    LOG_INFO("멀티클라이언트 서버를 시작합니다");

    // TCP 소켓 생성
    if (this->_tcpSocket.createTCPSocket() != TCPSocket::Result::SUCCESS)
    {
        return (MultiServer::Result::FAIL_START);
    }

    // 포트에 바인딩
    if (this->_tcpSocket.bindTCPSocket(_port) != TCPSocket::Result::SUCCESS)
    {
        return (MultiServer::Result::FAIL_START);
    }

    // 리슨 시작
    if (this->_tcpSocket.startListen() != TCPSocket::Result::SUCCESS)
    {
        return (MultiServer::Result::FAIL_START);
    }

    this->_isRunning = true;
    LOG_INFO("멀티클라이언트 서버가 성공적으로 시작되었습니다");

    return (MultiServer::Result::SUCCESS);
}

MultiServer::Result MultiServer::runServerLoop()
{
    LOG_INFO("서버 메인 루프를 시작합니다");

    while (this->_isRunning)
    {
        // SelectManager를 사용해 fd_set 설정
        this->_selectManager.setupFdSet();

        // 서버 소켓 추가
        this->_selectManager.addSocket(this->_tcpSocket.getSocket());

        // 모든 클라이언트 소켓 추가
        SOCKET client_sockets[ClientManager::MAX_CLIENTS];
        int socket_count = _clientManager.getAllSockets(client_sockets, ClientManager::MAX_CLIENTS);

        for (int i = 0; i < socket_count; ++i)
        {
            this->_selectManager.addSocket(client_sockets[i]);
        }

        // select 실행
        SelectManager::Result select_result = this->_selectManager.excuteSelect(1);

        // select 결과 처리
        switch (select_result)
        {
            case SelectManager::Result::SUCCESS:
                // 활성 소켓 처리
                break;

            case SelectManager::Result::TIMEOUT:
                // 타임아웃 - 다음 루프로 계속
                continue;

            case SelectManager::Result::FAIL_SELECT:
                LOG_ERROR("select 실행 실패");
                return (MultiServer::Result::FAIL_LOOP);

            case SelectManager::Result::NO_SOCKETS:
                // 소켓이 없으면 잠시 대기
                Sleep(100);
                continue;
        }

        // 서버 소켓 확인 (새로운 연결)
        if (this->_selectManager.isSocketReady(this->_tcpSocket.getSocket()))
        {
            if (this->handleNewConnection() == false)
            {
                LOG_WARN("새로운 연결 처리 실패");
            }
        }

        // 각 클라이언트 소켓 확인
        for (int i = 0; i < ClientManager::MAX_CLIENTS; ++i)
        {
            SOCKET client_socket = this->_clientManager.getClientSocket(i);
            if (client_socket != INVALID_SOCKET && this->_selectManager.isSocketReady(client_socket))
            {
                if (this->handleClientMessage(i) == false)
                {
                    // 클라이언트 연결 종료
                    this->announceLeave(i);
                    this->_clientManager.removeClient(i);
                }
            }
        }
    }

    LOG_INFO("서버 메인 루프가 종료되었습니다");
    return (MultiServer::Result::SUCCESS);
}

void MultiServer::stop()
{
    if (this->_isRunning)
    {
        this->_isRunning = false;
        LOG_INFO("멀티클라이언트 서버를 중지합니다");
    }
}

bool MultiServer::isRunning() const
{
    return (this->_isRunning);
}

bool MultiServer::handleNewConnection()
{
    // 새로운 클라이언트 연결 수락
    SOCKET client_socket = this->_tcpSocket.acceptConnection();
    if (client_socket == INVALID_SOCKET)
    {
        return (false);
    }

    // 클라이언트 추가
    int client_index = this->_clientManager.addClient(client_socket);
    if (client_index == -1)
    {
        // 최대 클라이언트 수 초과
        std::string reject_message = "서버가 가득 찼습니다. 나중에 다시 시도해주세요.";
        this->_messageSender.unicast(reject_message, client_socket);
        closesocket(client_socket);
        return (false);
    }

    // 환영 메시지 전송
    this->sendWelcomeMessage(client_index);

    // 다른 클라이언트들에게 참여 알림
    this->announceJoin(client_index);

    LOG_INFO("새로운 클라이언트 연결 완료 - 인덱스: " + std::to_string(client_index));
    return (true);
}

bool MultiServer::handleClientMessage(int client_index)
{
    SOCKET client_socket = this->_clientManager.getClientSocket(client_index);
    if (client_socket == INVALID_SOCKET)
    {
        return (false);
    }

    // MessageRecevier로 메시지 수신
    MessageRecevier receiver(client_socket);
    MessageRecevier::Result recv_result = receiver.receiveMessage();

    switch (recv_result)
    {
    case MessageRecevier::Result::SUCCESS:
    {
        std::string message = receiver.getLastMessage();

        // quit 명령 확인
        if (receiver.isQuitCommand(message))
        {
            std::string goodbye_message = "안녕히 가세요!";
            this->_messageSender.unicast(goodbye_message, client_socket);
            return (false); // 연결 종료
        }

        // 브로드캐스트 메시지 생성
        std::string broadcast_message = "[클라이언트" + std::to_string(client_index) + "]: " + message;

        // 모든 클라이언트에게 브로드캐스트 (발송자 제외)
        SOCKET all_sockets[ClientManager::MAX_CLIENTS];
        int socket_count = this->_clientManager.getAllSockets(all_sockets, ClientManager::MAX_CLIENTS);
        this->_messageSender.multicast(broadcast_message, all_sockets, socket_count, client_socket);

        return (true);
    }

    case MessageRecevier::Result::CLIENT_DISCONNECTED:
        LOG_INFO("클라이언트 연결 해제 - 인덱스: " + std::to_string(client_index));
        return (false);

    case MessageRecevier::Result::FAIL_RECEIVE:
        LOG_ERROR("클라이언트 메시지 수신 실패 - 인덱스: " + std::to_string(client_index));
        return (false);

    default:
        return (false);
    }
}

void MultiServer::sendWelcomeMessage(int client_index)
{
    SOCKET client_socket = this->_clientManager.getClientSocket(client_index);
    if (client_socket == INVALID_SOCKET) return;

    std::string welcome_message = "=== 채팅 서버에 오신 것을 환영합니다! ===";
    welcome_message += "\n당신은 클라이언트" + std::to_string(client_index) + "입니다.";
    welcome_message += "\n현재 접속자 수: " + std::to_string(this->_clientManager.getConnectedClientCount()) + "명";
    welcome_message += "\n'quit'를 입력하면 종료됩니다.";
    welcome_message += "\n==========================================";

    this->_messageSender.unicast(welcome_message, client_socket);
}

void MultiServer::announceJoin(int client_index)
{
    std::string join_message = "[시스템] 클라이언트" + std::to_string(client_index) + "님이 채팅방에 참여했습니다.";

    SOCKET all_sockets[ClientManager::MAX_CLIENTS];
    int socket_count = this->_clientManager.getAllSockets(all_sockets, ClientManager::MAX_CLIENTS);

    SOCKET new_client_socket = this->_clientManager.getClientSocket(client_index);
    this->_messageSender.multicast(join_message, all_sockets, socket_count, new_client_socket);
}

void MultiServer::announceLeave(int client_index)
{
    std::string leave_message = "[시스템] 클라이언트" + std::to_string(client_index) + "님이 채팅방을 떠났습니다.";

    SOCKET all_sockets[ClientManager::MAX_CLIENTS];
    int socket_count = this->_clientManager.getAllSockets(all_sockets, ClientManager::MAX_CLIENTS);

    this->_messageSender.broadcast(leave_message, all_sockets, socket_count);
}