#include "MessageReceiver.h"
#include "DebugHelper.h"

MessageRecevier::MessageRecevier(SOCKET client_socket)
    : _clientSocket(client_socket), _lastMessage("")
{
    LOG_DEBUG("MessageRecevier ��ü�� �����մϴ�.");
}

MessageRecevier::~MessageRecevier()
{
    LOG_DEBUG("MessageManager ��ü�� �����մϴ�.");
}

MessageRecevier::Result MessageRecevier::receiveMessage()
{
    char buffer[MessageRecevier::BUFFER_SIZE];

    //recv �Լ��� Ŭ���̾�Ʈ ������ ���� ���ۿ��� �����͸� �о�ɴϴ�.
    //  ���� ����:.
    //      ù ��° ����: �����͸� ���� ��� ���� (_clientSocket).
    //      �� ��° ����: ���� �����͸� ������ ���� (buffer).
    //      �� ��° ����: �ִ� ���� ����Ʈ �� (BUFFER_SIZE - 1).
    //      �� ��° ����: �÷��� (�Ϲ������� 0 ���).
    //          MSG_PEEK : �����͸� �д��� ���۸� ������ (��� Ȯ�� �� ����� �� ����).
    //          MSG_OOB : �뿪 �� ������ ����.
    //          MSG_WAITALL : ��ü ���۰� ä���� ������ ���.
    //  ��ȯ���� receive_result�� ������ �ǹ��մϴ�:.
    //      > 0: ������ ���� ����Ʈ ��.
    //      == 0: ������ ���������� �����.
    //      < 0: ���� �߻� (��: ���� ���� �Ǵ� ��Ʈ��ũ ����).
    int receive_result = recv(this->_clientSocket, buffer, BUFFER_SIZE - 1, 0);

    // ������ ����� �̷���� ���.
    if (receive_result > 0)
    {
        buffer[receive_result] = '\0';
        this->_lastMessage = std::string(buffer);

        LOG_INFO("����: " + this->_lastMessage);
        // ���� ���� ���ڿ��� �ɸ��� ������ �����մϴ�.
        cleanMessage(this->_lastMessage);

        // ���Ź��� ���ڿ��� quit������ üũ�մϴ�.
        if (isQuitCommand(this->_lastMessage))
        {
            // �´ٸ�.
            return (MessageRecevier::Result::CLIENT_QUIT);
        }

        return (MessageRecevier::Result::SUCCESS);
    }
    // ������ ���������� ����.
    else if (receive_result == 0)
    {
        LOG_INFO("Ŭ���̾�Ʈ�� ������ �����߽��ϴ�.");
        return (MessageRecevier::Result::CLIENT_DISCONNECTED);
    }
    // ���� �߻�.
    else
    {
        LOG_ERROR("�޽��� ������ �����߽��ϴ�.\n���� �ڵ�: " + std::to_string(WSAGetLastError()));
        return (MessageRecevier::Result::FAIL_RECEIVE);
    }
}

const std::string& MessageRecevier::getLastMessage() const
{
    // ������ ���ڿ��� ��ȯ�մϴ�.
    return (this->_lastMessage);
}

bool MessageRecevier::isQuitCommand(const std::string& message)
{
    // quit�� ������ true, Ʋ���� false.
    return (message == "quit");
}

void MessageRecevier::cleanMessage(std::string& message)
{
    // ĳ���� ������ �����մϴ�.
    if (!message.empty() && message.back() == '\n')
    {
        message.pop_back();
    }
    if (!message.empty() && message.back() == '\r')
    {
        message.pop_back();
    }
}
