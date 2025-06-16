#include "ClientManager.h"
#include "DebugHelper.h"

ClientManager::ClientManager()
    : _clientSockets(), _availableList(), _connectedSocketCount(0)
{
    initalizeClientSockets();
    initalizeAvailableList();
    LOG_DEBUG("ClientManager 객체를 생성합니다.");
}

ClientManager::~ClientManager()
{
    // 클라이언트 소켓 배열 정리.
    for (int i = 0; i < ClientManager::MAX_CLIENTS; ++i)
    {
        if (this->_clientSockets[i] != INVALID_SOCKET)
        {
            closesocket(this->_clientSockets[i]);
        }
    }
    LOG_DEBUG("ClientManager 객체를 삭제합니다.");
}

int ClientManager::addClient(SOCKET client_socket)
{
    if (client_socket == INVALID_SOCKET)
    {
        LOG_WARN("유효하지 않은 소켓입니다.");
        return (-1);
    }

    // 사용 가능한 인덱스 가져오기.
    int index = this->getNextIndex();
    if (index == -1)
    {
        LOG_WARN("클라이언트 추가 실패: 최대 클라이언트 수 초과");
        return (-1);
    }

    // 클라이언트 소켓 저장.
    this->_clientSockets[index] = client_socket;
    this->_connectedSocketCount = this->_connectedSocketCount + 1;

    LOG_INFO("클라이언트 추가 성공(player_" + std::to_string(index) + ")\n" + std::to_string(this->_connectedSocketCount) + "명");
    return (index);
}

bool ClientManager::removeClient(int client_index)
{
    // 유효한 인덱스 범위인지를 체크.
    if (this->isValidIndex(client_index) == false)
    {
        LOG_WARN("클라이언트 제거 실패: 유효하지 않은 인덱스 " + std::to_string(client_index));
        return (false);
    }

    // 클라이언트가 연결되어 있는지 확인.
    if (this->_clientSockets[client_index] == INVALID_SOCKET)
    {
        LOG_WARN("클라이언트 제거 실패: 이미 제거된 클라이언트 " + std::to_string(client_index));
        return (false);
    }

    // 소켓 정리.
    closesocket(this->_clientSockets[client_index]);
    this->_clientSockets[client_index] = INVALID_SOCKET;
    this->_connectedSocketCount = this->_connectedSocketCount - 1;

    // 삭제한 소켓의 인덱스를 재사용 큐에 추가.
    this->_availableList.push(client_index);

    LOG_INFO("클라이언트 제거 완료(player_" + std::to_string(client_index) + ")\n남은 접속자: " + std::to_string(this->_connectedSocketCount) + "명");
    return (true);
}

SOCKET ClientManager::getClientSocket(int client_index) const
{
    if (this->isValidIndex(client_index) == false)
    {
        return (INVALID_SOCKET);
    }

    return (this->_clientSockets[client_index]);
}

int ClientManager::getConnectedClientCount() const
{
    return (this->_connectedSocketCount);
}

bool ClientManager::isValidIndex(int client_index) const
{
    return (client_index >= 0 && client_index < ClientManager::MAX_CLIENTS);
}

int ClientManager::getAllSockets(SOCKET* sockets, int max_count) const
{
    int count = 0;

    for (int i = 0; i < ClientManager::MAX_CLIENTS && count < max_count; ++i)
    {
        if (this->_clientSockets[i] != INVALID_SOCKET)
        {
            sockets[count] = this->_clientSockets[i];
            count = count + 1;
        }
    }

    return (count);
}

std::string ClientManager::getClientNickname(int client_index) const
{
    // 유효하지 않은 인덱스 범위이거나.
    if (this->isValidIndex(client_index) == false)
    {
        return ("Player_Unknown");
    }

    // 해당 소켓이 유효하지 않다면
    if (this->_clientSockets[client_index] == INVALID_SOCKET)
    {
        return ("Player_Unknown");
    }

    // 항상 Player_{NUMBER} 형식으로 반환.
    return ("Player_" + std::to_string(client_index));
}

void ClientManager::initalizeClientSockets()
{
    // 클라이언트 소켓배열의 요소들을 INVALID_SOCKET으로 초기화합니다.
    for (int i = 0; i < ClientManager::MAX_CLIENTS; ++i)
    {
        this->_clientSockets[i] = INVALID_SOCKET;
    }
    LOG_DEBUG("클라이언트 소켓을 완료.");
}

void ClientManager::initalizeAvailableList()
{
    // 우선순위 큐에 0~9까지 숫자를 넣어놓습니다.
    for (int i = 0; i < ClientManager::MAX_CLIENTS; ++i)
    {
        this->_availableList.push(i);
    }
    LOG_DEBUG("사용 가능한 인덱스 큐 초기화 완료.");
}

int ClientManager::getNextIndex()
{
    // 우선 순위 큐가 비었다면.
    if (this->_availableList.empty())
    {
        return (-1);
    }

    int index = this->_availableList.top();
    this->_availableList.pop();

    return (index);
}
