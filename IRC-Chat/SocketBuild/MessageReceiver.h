#pragma once
#pragma execution_character_set("utf-8")

/**
 * @file MessageReceiver.h
 * @brief 클라이언트로부터 들어오는 메시지를 처리하는 MessageReceiver 클래스를 선언합니다.
 * @author 최성락
 * @date 2025-06-17
 * 
 * @details
 * 단일 클라이언트 소켓에서 메시지를 수신하고 파싱하는 역할을 담당합니다.
 * 특정 명령(예: "quit")을 감지하고, 명령에 맞는 역할을 수행합니다.
 */

#include <winsock2.h>
#include <string>

/**
 * @class MessageReceiver
 * @brief 클라이언트 소켓으로부터 들어오는 메시지를 처리하는 클래스입니다.
 *
 * @details
 * 클라이언트 소켓을 전달받아 객체를 생성합니다.
 * <br>소켓으로부터 메시지를 수신하여 저장하고, 특별한 명령(예: 종료 요청)이 있는지 확인합니다. 
 * <br>고정 크기 버퍼를 사용하여 수신된 데이터를 처리합니다.
 */
class MessageReceiver
{
    public:

        /**
         * @enum MessageReceiver::Result
         * @brief 메시지 수신의 결과 상태 값.
         */
        enum class Result
        {
            SUCCESS,            ///< 메시지 수신에 성공함
            FAIL_RECEIVE,       ///< 메시지 수신에 실패함(예: recv 오류)
            FAIL_SEND,          ///< 필요한 응답 전송에 실패함
            CLIENT_QUIT,        ///< 클라이언트가 quit 명령을 보냄
            CLIENT_DISCONNECTED ///< 클라이언트 연결이 예기치 않게 끊어짐
        };
    public:
        /**
         * @fn MessageReceiver::MessageReceiver(SOCKET client_socket)
         * @brief 주어진 클라이언트 소켓에 대한 MessageReceiver 객체를 생성합니다.
         * @param[IN] SOCKET client_socket : 이 수신기가 메시지를 받을 클라이언트 소켓.
         * @return 없음.
         * @note SOCKET을 인자로 받는 이 생성자만 사용할 수 있으며, 내부 변수 초기화합니다.
         */
        explicit MessageReceiver(SOCKET client_socket);

        /**
         * @fn MessageReceiver::~MessageReceiver()
         * @brief 소멸자.
         * @return 없음.
         * @note 클라이언트 소켓은 외부에서 관리되므로 이 소멸자는 소켓을 닫지 않습니다.
         */
        ~MessageReceiver();

        // 복사 생성자 및 복사 할당 연산자 삭제.
        MessageReceiver(const MessageReceiver& obj) = delete;
        MessageReceiver& operator=(const MessageReceiver& obj) = delete;

        // 이동 생성자 및 이동 할당 연산자 삭제.
        MessageReceiver(MessageReceiver&& obj) = delete;
        MessageReceiver& operator=(MessageReceiver&& obj) = delete;

public:
        /**
         * @fn MessageReceiver::Result MessageReceiver::receiveMessage()
         * @brief 클라이언트 소켓으로부터 메시지를 수신하고 내부에 저장합니다.
         * @return MessageReceiver::Result 수신 동작의 결과 상태 값.
         *
         * @details
         * 클라이언트 소켓에서 데이터를 읽어들입니다.
         * <br>성공 시 해당 데이터를 내부에 저장하여 getLastMessage()로 접근할 수 있습니다.
         * 
         * @note
         * - 메시지가 정상 수신되면 SUCCESS.
         * - 수신된 메세지가 "quit" 이면 CLIENT_QUIT.
         * - 오류 발생 시 적절한 상태 값.
         */
        MessageReceiver::Result receiveMessage();


        /**
         * @fn const std::string& MessageReceiver::getLastMessage() const
         * @brief 마지막으로 수신된 메시지를 반환합니다.
         * @return const std::string& : 저장된 마지막 메시지 문자열에 대한 참조.
         */
        const std::string& getLastMessage() const;

        /**
         * @fn bool MessageReceiver::isQuitCommand(const std::string& message) const
         * @brief 주어진 메시지가 "quit" 명령인지 확인합니다.
         * @param[IN] const std::string& message : 검사할 메시지 문자열.
         * @return bool : 해당 메시지가 "quit"와 일치하면 true, 아니면 false.
         */
        bool isQuitCommand(const std::string& message) const;

    private:
        /// @brief 통신에 사용하는 클라이언트 소켓.
        SOCKET _clientSocket;

        /// @brief 가장 최근에 수신한 메시지.
        std::string _lastMessage;

        /// 데이터 수신에 사용하는 버퍼 크기(바이트 단위).
        static const int BUFFER_SIZE = 1024;

    private:
        /**
         * @fn void MessageReceiver::cleanMessage(std::string& message)
         * @brief 수신한 메시지 문자열에서 캐리지 리턴/뉴라인 문자를 제거합니다.
         * @param[IN, OUT] std::string& message : 제거할 메시지 문자열.
         * @return 없음.
         * @note 메시지를 수신한 후 호출되어, 메시지 끝의 개행 문자를 제거합니다.
         */
        void cleanMessage(std::string& message);
};