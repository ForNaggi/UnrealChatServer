#pragma once

#include <winsock2.h>
#include <string>

/*
** 클래스명	:	MessageManager.
** 설명		:	클라이언트와의 메시지 송수신을 담당하는 클래스입니다.
*               메시지 수신, 전송, 명령어 처리를 담당합니다.
** 초기화	:	SOCKET을 매개변수로 받는 생성자만 허용.
*/
class MessageManager
{
    public:
        // 해당 클래스의 메소드의 결과를 나타내는 enum class입니다.
        enum class Result
        {
            SUCCESS,
            FAIL_RECEIVE,
            FAIL_SEND,
            CLIENT_QUIT,
            CLIENT_DISCONNECTED
        };

        /*
        ** 메소드명	:	MessageManager.
        ** 설명		:	MessageManager 객체 생성자.
        ** 인자값	:	SOCKET client_socket : 클라이언트 소켓.
        ** 출력값	:	없음.
        */
        explicit MessageManager(SOCKET client_socket);
    
        /*
        ** 메소드명	:	~MessageManager.
        ** 설명		:	MessageManager 객체 소멸자.
        ** 인자값	:	없음.
        ** 출력값	:	없음.
        */
        ~MessageManager();
    
        /*
        ** 메소드명	:	receiveMessage.
        ** 설명		:	클라이언트로부터 메세지를 수신합니다.
        ** 인자값	:	없음.
        ** 출력값	:	MessageManager::Result : 성공/실패 결과.
        */
        MessageManager::Result receiveMessage();
        
        /*
        ** 메소드명	:	sendEcho.
        ** 설명		:	받은 메시지를 에코로 전송합니다.
        ** 인자값	:	없음.
        ** 출력값	:	MessageManager::Result : 성공/실패 결과.
        */
        MessageManager::Result sendEcho();

        /*
        ** 메소드명	:	sendGoodbye.
        ** 설명		:	종료 메시지를 전송합니다.
        ** 인자값	:	없음.
        ** 출력값	:	MessageManager::Result : 성공/실패 결과.
        */
        MessageManager::Result sendGoodbye();

        /*
        ** 메소드명	:	getLastMessage.
        ** 설명		:	마지막으로 받은 메시지를 반환합니다.
        ** 인자값	:	없음.
        ** 출력값	:	const std::string& : 마지막 메시지.
        */
        const std::string& getLastMessage() const;
    
    private:
        // 특정 클라이언트와 통신하는 소켓.
        SOCKET _clientSocket;

        // 클라이언트에서 받은 메세지를 저장하는 멤버변수.
        std::string _lastMessage;
    
        // 상수 버퍼 사이즈.(recv한 바이트를 저장할 배열의 크기).
        static const int BUFFER_SIZE = 1024;
    
        /*
        ** 메소드명	:	cleanMessage.
        ** 설명		:	메시지에서 캐리지 리턴을 제거합니다.
        ** 인자값	:	std::string& message : 정리할 메시지.
        ** 출력값	:	없음.
        */
        void cleanMessage(std::string& message);
 
        /*
        ** 메소드명	:	isQuitCommand.
        ** 설명		:	메시지가 "quit"인지 확인합니다.
        ** 인자값	:	const std::string& message : "quit"인지 확인할 메시지.
        ** 출력값	:	bool : "quit"이면 true, 아니면 false.
        */
        bool isQuitCommand(const std::string& message);
};