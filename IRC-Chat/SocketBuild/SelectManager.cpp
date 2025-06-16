#include "SelectManager.h"
#include "DebugHelper.h"

static timeval make_timer(long sec, long usec)
{
    timeval timeout;
    timeout.tv_sec = sec;
    timeout.tv_usec = usec;
    return (timeout);
}

SelectManager::SelectManager()
    : _originSet(), _copySet(), _socketCount(0)
{
    LOG_DEBUG("SelectManager ��ü�� �����մϴ�.");
}

SelectManager::~SelectManager()
{
    LOG_DEBUG("SelectManager ��ü�� �����մϴ�.");
}

void SelectManager::setupFdSet()
{
    // fd_set �ʱ�ȭ.
    FD_ZERO(&this->_originSet);
    this->_socketCount = 0;

    LOG_DEBUG("fd_set�� ���� count�� �ʱ�ȭ�մϴ�.");
}

bool SelectManager::addSocket(SOCKET socket)
{
    // ��ȿ�� �������� Ȯ���մϴ�.
    if (socket == INVALID_SOCKET)
    {
        LOG_WARN("Ŭ���̾�Ʈ ���� ��� �� ��ȿ���� ���� ������ Ȯ�εǾ����ϴ�.");
        return (false);
    }

    // fd_set�� Ŭ���̾�Ʈ ������ �߰��մϴ�.
    FD_SET(socket, &this->_originSet);
    this->_socketCount = this->_socketCount + 1;

    LOG_DEBUG("Ŭ���̾�Ʈ ������ fd_set�� �߰��Ǿ����ϴ�.:\n���� ���� �� : " + std::to_string(this->_socketCount));
    return (true);
}

SelectManager::Result SelectManager::excuteSelect(int timeout_sec)
{
    // ������ ������ ������ �ٷ� Ż���մϴ�.
    if (this->_socketCount == 0)
    {
        LOG_DEBUG("������ ������ �����ϴ�.");
        return (SelectManager::Result::NO_SOCKETS);
    }

    // fd_set�� �����մϴ�.
    // select �Լ��� ������ fd_set�� �����ϹǷ� ������ �������� �ʱ� ���� �����ؼ� ����մϴ�.
    this->_copySet = this->_originSet;

    // select�� ����� timeout ����.
    timeval timeout = make_timer(timeout_sec, 0);

    // Windows������ select�Լ��� ù�� ° �Ű������� ���õ˴ϴ�.(������ �ִ� ���� ���� id).
    // �б� �غ� �� ������ ���� �����մϴ�.
    int result = select(0, &this->_copySet, nullptr, nullptr, &timeout);

    // select ����� ���� �б�.
    if (result == SOCKET_ERROR)
    {
        LOG_ERROR("slect �Լ� ���� ����\n ���� �ڵ�: " + std::to_string(WSAGetLastError()));
        return (SelectManager::Result::FAIL_SELECT);
    }
    else if (result == 0)
    {
        // Ÿ�Ӿƿ� �߻�.
        LOG_DEBUG("select Ÿ�Ӿƿ� �߻�");
        return (SelectManager::Result::TIMEOUT);
    }
    else
    {
        // �ϳ� �̻��� ������ �غ��.
        LOG_DEBUG("select ����, �غ�� ���� ��: " + std::to_string(result));
        return (SelectManager::Result::SUCCESS);
    }
}

bool SelectManager::isSocketReady(SOCKET socket) const
{
    // FD_ISSET�� ����Ͽ� fd_set���� ������ �бⰡ �غ�Ǿ����� üũ�մϴ�.
    // select() ���� �� ����� fd_set���� �бⰡ �غ�� ���ϸ� �����ֽ��ϴ�.
    return (FD_ISSET(socket, &this->_copySet));
}
