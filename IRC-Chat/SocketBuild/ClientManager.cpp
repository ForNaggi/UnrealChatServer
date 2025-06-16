#include "ClientManager.h"
#include "DebugHelper.h"

ClientManager::ClientManager()
    : _clientSockets(), _availableList(), _connectedSocketCount(0)
{
    initalizeClientSockets();
    initalizeAvailableList();
    LOG_DEBUG("ClientManager ��ü�� �����մϴ�.");
}

ClientManager::~ClientManager()
{
    // Ŭ���̾�Ʈ ���� �迭 ����.
    for (int i = 0; i < ClientManager::MAX_CLIENTS; ++i)
    {
        if (this->_clientSockets[i] != INVALID_SOCKET)
        {
            closesocket(this->_clientSockets[i]);
        }
    }
    LOG_DEBUG("ClientManager ��ü�� �����մϴ�.");
}

int ClientManager::addClient(SOCKET client_socket)
{
    if (client_socket == INVALID_SOCKET)
    {
        LOG_WARN("��ȿ���� ���� �����Դϴ�.");
        return (-1);
    }

    // ��� ������ �ε��� ��������.
    int index = this->getNextIndex();
    if (index == -1)
    {
        LOG_WARN("Ŭ���̾�Ʈ �߰� ����: �ִ� Ŭ���̾�Ʈ �� �ʰ�");
        return (-1);
    }

    // Ŭ���̾�Ʈ ���� ����.
    this->_clientSockets[index] = client_socket;
    this->_connectedSocketCount = this->_connectedSocketCount + 1;

    LOG_INFO("Ŭ���̾�Ʈ �߰� ����(player_" + std::to_string(index) + ")\n" + std::to_string(this->_connectedSocketCount) + "��");
    return (index);
}

bool ClientManager::removeClient(int client_index)
{
    // ��ȿ�� �ε��� ���������� üũ.
    if (this->isValidIndex(client_index) == false)
    {
        LOG_WARN("Ŭ���̾�Ʈ ���� ����: ��ȿ���� ���� �ε��� " + std::to_string(client_index));
        return (false);
    }

    // Ŭ���̾�Ʈ�� ����Ǿ� �ִ��� Ȯ��.
    if (this->_clientSockets[client_index] == INVALID_SOCKET)
    {
        LOG_WARN("Ŭ���̾�Ʈ ���� ����: �̹� ���ŵ� Ŭ���̾�Ʈ " + std::to_string(client_index));
        return (false);
    }

    // ���� ����.
    closesocket(this->_clientSockets[client_index]);
    this->_clientSockets[client_index] = INVALID_SOCKET;
    this->_connectedSocketCount = this->_connectedSocketCount - 1;

    // ������ ������ �ε����� ���� ť�� �߰�.
    this->_availableList.push(client_index);

    LOG_INFO("Ŭ���̾�Ʈ ���� �Ϸ�(player_" + std::to_string(client_index) + ")\n���� ������: " + std::to_string(this->_connectedSocketCount) + "��");
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
    // ��ȿ���� ���� �ε��� �����̰ų�.
    if (this->isValidIndex(client_index) == false)
    {
        return ("Player_Unknown");
    }

    // �ش� ������ ��ȿ���� �ʴٸ�
    if (this->_clientSockets[client_index] == INVALID_SOCKET)
    {
        return ("Player_Unknown");
    }

    // �׻� Player_{NUMBER} �������� ��ȯ.
    return ("Player_" + std::to_string(client_index));
}

void ClientManager::initalizeClientSockets()
{
    // Ŭ���̾�Ʈ ���Ϲ迭�� ��ҵ��� INVALID_SOCKET���� �ʱ�ȭ�մϴ�.
    for (int i = 0; i < ClientManager::MAX_CLIENTS; ++i)
    {
        this->_clientSockets[i] = INVALID_SOCKET;
    }
    LOG_DEBUG("Ŭ���̾�Ʈ ������ �Ϸ�.");
}

void ClientManager::initalizeAvailableList()
{
    // �켱���� ť�� 0~9���� ���ڸ� �־�����ϴ�.
    for (int i = 0; i < ClientManager::MAX_CLIENTS; ++i)
    {
        this->_availableList.push(i);
    }
    LOG_DEBUG("��� ������ �ε��� ť �ʱ�ȭ �Ϸ�.");
}

int ClientManager::getNextIndex()
{
    // �켱 ���� ť�� ����ٸ�.
    if (this->_availableList.empty())
    {
        return (-1);
    }

    int index = this->_availableList.top();
    this->_availableList.pop();

    return (index);
}
