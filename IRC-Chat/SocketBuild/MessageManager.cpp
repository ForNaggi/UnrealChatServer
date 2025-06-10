#include "MessageManager.h"
#include "DebugHelper.h"

MessageManager::MessageManager(SOCKET client_socket)
    : _clientSocket(client_socket)
    , _lastMessage("")
{
    LOG_DEBUG("MessageManager 객체를 생성합니다.");
}

MessageManager::~MessageManager()
{
    LOG_DEBUG("MessageManager 객체를 삭제합니다.");
}

MessageManager::Result MessageManager::receiveMessage()
{
  
    char buffer[BUFFER_SIZE];

    //recv 함수는 클라이언트 소켓의 수신 버퍼에서 데이터를 읽어옵니다.
    //  인자 설명:.
    //      첫 번째 인자: 데이터를 읽을 대상 소켓 (_clientSocket).
    //      두 번째 인자: 수신 데이터를 저장할 버퍼 (buffer).
    //      세 번째 인자: 최대 읽을 바이트 수 (BUFFER_SIZE - 1).
    //      네 번째 인자: 플래그 (일반적으로 0 사용).
    //          MSG_PEEK : 데이터를 읽더라도 버퍼를 안지움 (헤더 확인 시 사용할 수 있음).
    //          MSG_OOB : 대역 외 데이터 수신.
    //          MSG_WAITALL : 전체 버퍼가 채워질 때까지 대기.
    //  반환값인 bytes_received는 다음을 의미합니다:.
    //      > 0: 실제로 읽은 바이트 수.
    //      == 0: 연결이 정상적으로 종료됨.
    //      < 0: 오류 발생 (예: 연결 끊김 또는 네트워크 오류).
    int bytes_received = recv(_clientSocket, buffer, BUFFER_SIZE - 1, 0);

    // 수신이 제대로 이루어진 경우.
    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';
        this->_lastMessage = std::string(buffer);

        LOG_INFO("수신: " + this->_lastMessage);
        // 수신 받은 문자열의 케리지 리턴을 제거합니다.
        cleanMessage(this->_lastMessage);

        // 수신받은 문자열이 quit인지를 체크합니다.
        if (isQuitCommand(this->_lastMessage))
        {
            // 맞다면.
            return (MessageManager::Result::CLIENT_QUIT);
        }

        return Result::SUCCESS;
    }
    // 연결이 정상적으로 종료.
    else if (bytes_received == 0)
    {
        LOG_INFO("클라이언트가 연결을 종료했습니다.");
        return (MessageManager::Result::CLIENT_DISCONNECTED);
    }
    // 오류 발생.
    else
    {
        LOG_ERROR("메시지 수신을 실패했습니다.\n에러 코드: " + std::to_string(WSAGetLastError()));
        return (MessageManager::Result::FAIL_RECEIVE);
    }
}

MessageManager::Result MessageManager::sendEcho()
{
    // 저장하고 있는 _lastMessage에는 캐리지 리턴이 없기 때문에 추가하여 샌드해야합니다.
    std::string echo_response = "Echo: " + this->_lastMessage + "\r\n";

    // send 함수는 연결된 소켓을 통해 데이터를 전송합니다.
    //  인자 설명:.
    //      첫 번째 인자: 데이터를 전송할 대상 소켓 (_clientSocket).
    //      두 번째 인자: 전송할 데이터를 담은 버퍼 (buffer).
    //      세 번째 인자: 전송할 바이트 수 (예: strlen(buffer)).
    //      네 번째 인자: 플래그 (일반적으로 0 사용).
    //          MSG_OOB : 대역 외(Out-of-band) 데이터 전송.
    //          MSG_DONTROUTE : 라우팅 테이블을 거치지 않고 직접 전송 (로컬 테스트 용도, 거의 사용되지 않음).
    //  반환값인 bytes_sent는 다음을 의미합니다:.
    //      > 0: 실제로 전송한 바이트 수.
    //      == 0: (실제 사용 시 거의 없음, 특별한 의미 없음).
    //      < 0: 오류 발생 (예: 연결되지 않았거나 네트워크 오류 발생).
    //          오류 발생 시 WSAGetLastError()로 원인 확인 필요.
    int bytes_sent = send(this->_clientSocket, echo_response.c_str(), (int)echo_response.length(), 0);

    // 오류 발생.
    if (bytes_sent <= SOCKET_ERROR)
    {
        LOG_ERROR("메시지 전송을 실패했습니다.\n에러 코드: " + std::to_string(WSAGetLastError()));
        return (MessageManager::Result::FAIL_SEND);
    }

    // 송신 완료.
    // 송신이 완료되었다고 상대방이 해당 바이트를 받은 것은 아니다. 
    // 전송 큐에 바이트가 올라갔을 뿐입니다. 이 것은 내부적으로 알아서 일어나는 일입니다.(추상화).
    LOG_INFO("송신: " + echo_response);
    return (MessageManager::Result::SUCCESS);
}

MessageManager::Result MessageManager::sendGoodbye()
{
    std::string goodbye_msg = "Goodbye!\r\n";

    int bytes_sent = send(this->_clientSocket, goodbye_msg.c_str(), (int)goodbye_msg.length(), 0);

    if (bytes_sent == SOCKET_ERROR)
    {
        LOG_ERROR("종료 메시지 전송을 실패했습니다.\n에러 코드: " + std::to_string(WSAGetLastError()));
        return (MessageManager::Result::FAIL_SEND);
    }

    LOG_INFO("종료 메시지를 전송했습니다.");
    return (MessageManager::Result::SUCCESS);
}

const std::string& MessageManager::getLastMessage() const
{
    return (this->_lastMessage);
}

void MessageManager::cleanMessage(std::string& message)
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

bool MessageManager::isQuitCommand(const std::string& message)
{
    return (message == "quit");
}