#include "MessageSender.h"
#include "DebugHelper.h"

const char* MessageSender::NEW_LINE = "\r\n";

MessageSender::MessageSender()
{
    LOG_DEBUG("MessageSender °´Ã¼¸¦ »ı¼ºÇÕ´Ï´Ù.");
}

MessageSender::~MessageSender()
{
    LOG_DEBUG("MessageSender °´Ã¼¸¦ »èÁ¦ÇÕ´Ï´Ù.");
}

MessageSender::Result MessageSender::broadcast(const std::string& message, SOCKET* sockets, int socket_count)
{
    // Àü¼ÛÇÒ ¼ÒÄÏÀÌ ¾ø´Â °æ¿ì.
    if (socket_count == 0)
    {
        LOG_DEBUG("Àü¼ÛÇÒ Å¬¶óÀÌ¾ğÆ®°¡ ¾ø½À´Ï´Ù.");
        return (MessageSender::Result::TOTAL_FAIL);
    }

    // Å¬¶óÀÌ¾ğÆ®·Î º¸³¾ ¸Ş¼¼Áö·Î Æ÷¸äÇÕ´Ï´Ù.
    std::string formatted_message = this->formatMessage(message);

    // Àü¼Û ¼º°ø ¼ö.
    int success_count = 0;

    for (int i = 0; i < socket_count; ++i)
    {
        if (this->sendMessage(formatted_message, sockets[i]))
        {
            success_count = success_count + 1;
        }
    }

    // °á°ú¿¡ µû¸¥ ºĞ±â.
    if (success_count == socket_count)
    {
        LOG_INFO("ºê·ÎµåÄ³½ºÆ® ¼º°ø: " + message);
        return (MessageSender::Result::SUCCESS);
    }
    else if (success_count > 0)
    {
        LOG_WARN("ºê·ÎµåÄ³½ºÆ® ºÎºĞ ½ÇÆĞ: " + std::to_string(success_count) + "/" + std::to_string(socket_count));
        return (MessageSender::Result::PARTIAL_FAIL);
    }
    else
    {
        LOG_ERROR("ºê·ÎµåÄ³½ºÆ® ½ÇÆĞ");
        return (MessageSender::Result::TOTAL_FAIL);
    }
}

MessageSender::Result MessageSender::multicast(const std::string& message, SOCKET* sockets, int socket_count, SOCKET except_socket)
{
    // Àü¼ÛÇÒ ¼ÒÄÏÀÌ ¾ø´Â °æ¿ì.
    if (socket_count == 0)
    {
        LOG_DEBUG("Àü¼ÛÇÒ Å¬¶óÀÌ¾ğÆ®°¡ ¾ø½À´Ï´Ù.");
        return (MessageSender::Result::TOTAL_FAIL);
    }

    // Å¬¶óÀÌ¾ğÆ®·Î º¸³¾ ¸Ş¼¼Áö·Î Æ÷¸äÇÕ´Ï´Ù.
    std::string formatted_message = this->formatMessage(message);
    // Á¦¿ÜµÈ Å¬¶óÀÌ¾ğÆ®¸¦ Á¦¿ÜÇÏ°í °¹¼ö¸¦ ¸Â­ ¾ßÇÏ±â ¶§¹®¿¡ µÎ°³ÀÇ º¯¼ö·Î ³ª´²¼­ ÀúÀåÇÑ´Ù.
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

    // °á°ú¿¡ µû¸¥ ºĞ±â.
    if (target_count == 0)
    {
        LOG_DEBUG("Àü¼ÛÇÒ ´ë»óÀÌ ¾ø½À´Ï´Ù.");
        return (MessageSender::Result::SUCCESS);
    }
    else if (success_count == target_count)
    {
        LOG_INFO("¸ÖÆ¼Ä³½ºÆ® ¼º°ø: " + message);
        return (MessageSender::Result::SUCCESS);
    }
    else if (success_count > 0)
    {
        LOG_WARN("¸ÖÆ¼Ä³½ºÆ® ºÎºĞ ½ÇÆĞ: " + std::to_string(success_count) + "/" + std::to_string(target_count));
        return (MessageSender::Result::PARTIAL_FAIL);
    }
    else
    {
        LOG_ERROR("¸ÖÆ¼Ä³½ºÆ® ½ÇÆĞ.");
        return (MessageSender::Result::TOTAL_FAIL);
    }
}

bool MessageSender::unicast(const std::string& message, SOCKET target_socket)
{
    if (target_socket == INVALID_SOCKET)
    {
        LOG_WARN("À¯È¿ÇÏÁö ¾ÊÀº ¼ÒÄÏ¿¡ ¸Ş¼¼Áö Àü¼Û ½Ãµµ");
        return (false);
    }

    std::string formatted_message = this->formatMessage(message);
    return (sendMessage(formatted_message, target_socket));
}

std::string MessageSender::formatMessage(const std::string& message) const
{
    // ¸Ş¼¼Áö¿¡ °³Çà ¹®ÀÚ¸¦ Ãß°¡ÇÕ´Ï´Ù.
    return (message + MessageSender::NEW_LINE);
}

bool MessageSender::sendMessage(const std::string& formatted_mssage, SOCKET target_socket)
{
    // Àü¼ÛÇÒ Å¬¶óÀÌ¾ğÆ® ¼ÒÄÏÀ» È®ÀÎÇÕ´Ï´Ù.
    if (target_socket == INVALID_SOCKET)
    {
        return (false);
    }

    // send ÇÔ¼ö·Î ¸Ş¼¼Áö¸¦ Àü¼ÛÇÕ´Ï´Ù.
    int send_result = send(target_socket, formatted_mssage.c_str(), (int)formatted_mssage.length(), 0);

    if (send_result == SOCKET_ERROR)
    {
        LOG_DEBUG("¸Ş¼¼Áö Àü¼Û ½ÇÆĞ - ¼ÒÄÏ: " + std::to_string(target_socket) + ", ¿¡·¯: " + std::to_string(WSAGetLastError()));
        return (false);
    }

    LOG_DEBUG("¸Ş¼¼Áö Àü¼Û ¼º°ø - ¹ÙÀÌÆ®: " + std::to_string(send_result));
    return (true);
}
