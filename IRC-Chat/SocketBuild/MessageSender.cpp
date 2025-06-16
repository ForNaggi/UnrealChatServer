#include "MessageSender.h"
#include "DebugHelper.h"

const char* MessageSender::NEW_LINE = "\r\n";

MessageSender::MessageSender()
{
    LOG_DEBUG("MessageSender ��ü�� �����մϴ�.");
}

MessageSender::~MessageSender()
{
    LOG_DEBUG("MessageSender ��ü�� �����մϴ�.");
}

MessageSender::Result MessageSender::broadcast(const std::string& message, SOCKET* sockets, int socket_count)
{
    // ������ ������ ���� ���.
    if (socket_count == 0)
    {
        LOG_DEBUG("������ Ŭ���̾�Ʈ�� �����ϴ�.");
        return (MessageSender::Result::TOTAL_FAIL);
    }

    // Ŭ���̾�Ʈ�� ���� �޼����� �����մϴ�.
    std::string formatted_message = this->formatMessage(message);

    // ���� ���� ��.
    int success_count = 0;

    for (int i = 0; i < socket_count; ++i)
    {
        if (this->sendMessage(formatted_message, sockets[i]))
        {
            success_count = success_count + 1;
        }
    }

    // ����� ���� �б�.
    if (success_count == socket_count)
    {
        LOG_INFO("��ε�ĳ��Ʈ ����: " + message);
        return (MessageSender::Result::SUCCESS);
    }
    else if (success_count > 0)
    {
        LOG_WARN("��ε�ĳ��Ʈ �κ� ����: " + std::to_string(success_count) + "/" + std::to_string(socket_count));
        return (MessageSender::Result::PARTIAL_FAIL);
    }
    else
    {
        LOG_ERROR("��ε�ĳ��Ʈ ����");
        return (MessageSender::Result::TOTAL_FAIL);
    }
}

MessageSender::Result MessageSender::multicast(const std::string& message, SOCKET* sockets, int socket_count, SOCKET except_socket)
{
    // ������ ������ ���� ���.
    if (socket_count == 0)
    {
        LOG_DEBUG("������ Ŭ���̾�Ʈ�� �����ϴ�.");
        return (MessageSender::Result::TOTAL_FAIL);
    }

    // Ŭ���̾�Ʈ�� ���� �޼����� �����մϴ�.
    std::string formatted_message = this->formatMessage(message);
    // ���ܵ� Ŭ���̾�Ʈ�� �����ϰ� ������ �­����ϱ� ������ �ΰ��� ������ ������ �����Ѵ�.
    int success_count = 0;
    int target_count = 0;

    for (int i = 0; i < socket_count; ++i)
    {
        if (sockets[i] != except_socket)
        {
            target_count = target_count + 1;
            if (this->sendMessage(formatted_message, sockets[i]))
            {
                success_count = success_count + 1;
            }
        }
    }

    // ����� ���� �б�.
    if (target_count == 0)
    {
        LOG_DEBUG("������ ����� �����ϴ�.");
        return (MessageSender::Result::SUCCESS);
    }
    else if (success_count == target_count)
    {
        LOG_INFO("��Ƽĳ��Ʈ ����: " + message);
        return (MessageSender::Result::SUCCESS);
    }
    else if (success_count > 0)
    {
        LOG_WARN("��Ƽĳ��Ʈ �κ� ����: " + std::to_string(success_count) + "/" + std::to_string(target_count));
        return (MessageSender::Result::PARTIAL_FAIL);
    }
    else
    {
        LOG_ERROR("��Ƽĳ��Ʈ ����.");
        return (MessageSender::Result::TOTAL_FAIL);
    }
}

bool MessageSender::unicast(const std::string& message, SOCKET target_socket)
{
    if (target_socket == INVALID_SOCKET)
    {
        LOG_WARN("��ȿ���� ���� ���Ͽ� �޼��� ���� �õ�");
        return (false);
    }

    std::string formatted_message = this->formatMessage(message);
    return (sendMessage(formatted_message, target_socket));
}

std::string MessageSender::formatMessage(const std::string& message) const
{
    // �޼����� ���� ���ڸ� �߰��մϴ�.
    return (message + MessageSender::NEW_LINE);
}

bool MessageSender::sendMessage(const std::string& formatted_mssage, SOCKET target_socket)
{
    // ������ Ŭ���̾�Ʈ ������ Ȯ���մϴ�.
    if (target_socket == INVALID_SOCKET)
    {
        return (false);
    }

    // send �Լ��� �޼����� �����մϴ�.
    int send_result = send(target_socket, formatted_mssage.c_str(), (int)formatted_mssage.length(), 0);

    if (send_result == SOCKET_ERROR)
    {
        LOG_DEBUG("�޼��� ���� ���� - ����: " + std::to_string(target_socket) + ", ����: " + std::to_string(WSAGetLastError()));
        return (false);
    }

    LOG_DEBUG("�޼��� ���� ���� - ����Ʈ: " + std::to_string(send_result));
    return (true);
}
