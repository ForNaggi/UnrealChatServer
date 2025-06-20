#pragma execution_character_set("utf-8")

/**
 * @file MessageReceiver.cpp
 * @brief MessageReceiver.h 구현부.
 * @author 최성락
 * @date 2025-06-17
 */

#include "MessageReceiver.h"
#include "DebugHelper.h"

MessageReceiver::MessageReceiver(SOCKET client_socket)
    : _clientSocket(client_socket), _lastMessage("")
{
    LOG_DEBUG("MessageReceiver 객체를 생성합니다.");
}

MessageReceiver::~MessageReceiver()
{
    LOG_DEBUG("MessageManager 객체를 삭제합니다.");
}

MessageReceiver::Result MessageReceiver::receiveMessage()
{
    char buffer[MessageReceiver::BUFFER_SIZE];

    //recv 함수는 클라이언트 소켓의 수신 버퍼에서 데이터를 읽어옵니다.
    //  인자 설명:.
    //      첫 번째 인자: 데이터를 읽을 대상 소켓 (_clientSocket).
    //      두 번째 인자: 수신 데이터를 저장할 버퍼 (buffer).
    //      세 번째 인자: 최대 읽을 바이트 수 (BUFFER_SIZE - 1).
    //      네 번째 인자: 플래그 (일반적으로 0 사용).
    //          MSG_PEEK : 데이터를 읽더라도 버퍼를 안지움 (헤더 확인 시 사용할 수 있음).
    //          MSG_OOB : 대역 외 데이터 수신.
    //          MSG_WAITALL : 전체 버퍼가 채워질 때까지 대기.
    //  반환값인 receive_result는 다음을 의미합니다:.
    //      > 0: 실제로 읽은 바이트 수.
    //      == 0: 연결이 정상적으로 종료됨.
    //      < 0: 오류 발생 (예: 연결 끊김 또는 네트워크 오류).
    int receive_result = recv(this->_clientSocket, buffer, BUFFER_SIZE - 1, 0);

    // 수신이 제대로 이루어진 경우.
    if (receive_result > 0)
    {
        buffer[receive_result] = '\0';
        this->_lastMessage = std::string(buffer);

        LOG_INFO("수신: " + this->_lastMessage);
        // 수신 받은 문자열의 케리지 리턴을 제거합니다.
        cleanMessage(this->_lastMessage);

        // 수신받은 문자열이 quit인지를 체크합니다.
        if (isQuitCommand(this->_lastMessage))
        {
            // 맞다면.
            return (MessageReceiver::Result::CLIENT_QUIT);
        }

        return (MessageReceiver::Result::SUCCESS);
    }
    // 연결이 정상적으로 종료.
    else if (receive_result == 0)
    {
        LOG_INFO("클라이언트가 연결을 종료했습니다.");
        return (MessageReceiver::Result::CLIENT_DISCONNECTED);
    }
    // 오류 발생.
    else
    {
        LOG_ERROR("메시지 수신을 실패했습니다.\n에러 코드: " + std::to_string(WSAGetLastError()));
        return (MessageReceiver::Result::FAIL_RECEIVE);
    }
}

const std::string& MessageReceiver::getLastMessage() const
{
    // 마지막 문자열을 반환합니다.
    return (this->_lastMessage);
}

bool MessageReceiver::isQuitCommand(const std::string& message) const
{
    // quit가 맞으면 true, 틀리면 false.
    return (message == "quit");
}

void MessageReceiver::cleanMessage(std::string& message)
{
    // 캐리지 리턴을 제거합니다.
    if (!message.empty() && message.back() == '\n')
    {
        message.pop_back();
    }
    if (!message.empty() && message.back() == '\r')
    {
        message.pop_back();
    }
}
