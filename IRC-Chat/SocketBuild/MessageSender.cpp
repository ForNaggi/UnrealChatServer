#pragma execution_character_set("utf-8")

/**
 * @file MessageSender.cpp
 * @brief MessageSender.h 구현부.
 * @author 최성락
 * @date 2025-06-17
 */

#include "MessageSender.h"
#include "DebugHelper.h"

const char* MessageSender::NEW_LINE = "\r\n";

MessageSender::MessageSender()
{
    LOG_DEBUG("MessageSender 객체를 생성합니다.");
}

MessageSender::~MessageSender()
{
    LOG_DEBUG("MessageSender 객체를 삭제합니다.");
}

MessageSender::Result MessageSender::broadcast(const std::string& message, SOCKET* sockets, int socket_count)
{
    // 전송할 소켓이 없는 경우.
    if (socket_count == 0)
    {
        LOG_DEBUG("전송할 클라이언트가 없습니다.");
        return (MessageSender::Result::TOTAL_FAIL);
    }

    // 클라이언트로 보낼 메세지로 포멧합니다.
    std::string formatted_message = this->formatMessage(message);

    // 전송 성공 수.
    int success_count = 0;

    for (int i = 0; i < socket_count; ++i)
    {
        if (this->sendMessage(formatted_message, sockets[i]))
        {
            success_count = success_count + 1;
        }
    }

    // 결과에 따른 분기.
    if (success_count == socket_count)
    {
        LOG_INFO("브로드캐스트 성공: " + message);
        return (MessageSender::Result::SUCCESS);
    }
    else if (success_count > 0)
    {
        LOG_WARN("브로드캐스트 부분 실패: " + std::to_string(success_count) + "/" + std::to_string(socket_count));
        return (MessageSender::Result::PARTIAL_FAIL);
    }
    else
    {
        LOG_ERROR("브로드캐스트 실패");
        return (MessageSender::Result::TOTAL_FAIL);
    }
}

MessageSender::Result MessageSender::multicast(const std::string& message, SOCKET* sockets, int socket_count, SOCKET except_socket)
{
    // 전송할 소켓이 없는 경우.
    if (socket_count == 0)
    {
        LOG_DEBUG("전송할 클라이언트가 없습니다.");
        return (MessageSender::Result::TOTAL_FAIL);
    }

    // 클라이언트로 보낼 메세지로 포멧합니다.
    std::string formatted_message = this->formatMessage(message);
    // 제외된 클라이언트를 제외하고 갯수를 맞췅야하기 때문에 두개의 변수로 나눠서 저장한다.
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

    // 결과에 따른 분기.
    if (target_count == 0)
    {
        LOG_DEBUG("전송할 대상이 없습니다.");
        return (MessageSender::Result::SUCCESS);
    }
    else if (success_count == target_count)
    {
        LOG_INFO("멀티캐스트 성공: " + message);
        return (MessageSender::Result::SUCCESS);
    }
    else if (success_count > 0)
    {
        LOG_WARN("멀티캐스트 부분 실패: " + std::to_string(success_count) + "/" + std::to_string(target_count));
        return (MessageSender::Result::PARTIAL_FAIL);
    }
    else
    {
        LOG_ERROR("멀티캐스트 실패.");
        return (MessageSender::Result::TOTAL_FAIL);
    }
}

bool MessageSender::unicast(const std::string& message, SOCKET target_socket)
{
    if (target_socket == INVALID_SOCKET)
    {
        LOG_WARN("유효하지 않은 소켓에 메세지 전송 시도");
        return (false);
    }

    std::string formatted_message = this->formatMessage(message);
    return (sendMessage(formatted_message, target_socket));
}

std::string MessageSender::formatMessage(const std::string& message) const
{
    // 메세지에 개행 문자를 추가합니다.
    return (message + MessageSender::NEW_LINE);
}

bool MessageSender::sendMessage(const std::string& formatted_mssage, SOCKET target_socket)
{
    // 전송할 클라이언트 소켓을 확인합니다.
    if (target_socket == INVALID_SOCKET)
    {
        return (false);
    }

    // send 함수로 메세지를 전송합니다.
    int send_result = send(target_socket, formatted_mssage.c_str(), (int)formatted_mssage.length(), 0);

    if (send_result == SOCKET_ERROR)
    {
        LOG_DEBUG("메세지 전송 실패 - 소켓: " + std::to_string(target_socket) + ", 에러: " + std::to_string(WSAGetLastError()));
        return (false);
    }

    LOG_DEBUG("메세지 전송 성공 - 바이트: " + std::to_string(send_result));
    return (true);
}
