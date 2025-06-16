#pragma once

#include "TCPSocket.h"
#include "ClientManager.h"
#include "SelectManager.h"
#include "MessageSender.h"
#include "MessageReceiver.h"

/*
** 클래스명	:	MultiServer.
** 설명		:	여러 컴포넌트를 조합해 멀티클라이언트 서버를 구현합니다.
*               각 컴포넌트의 기능을 조합하여 select 기반 서버를 만듭니다.
** 초기화	:	int(포트번호)를 매개변수로 받는 생성자만 가능.
*/
class MultiServer
{
public:

    // 서버 결과를 나타내는 enum class입니다.
    enum class Result
    {
        SUCCESS,
        FAIL_START,
        FAIL_LOOP,
        SERVER_STOPPED
    };

public:
    /*
    ** 함수명	:	MultiServer.
    ** 설명		:	MultiServer 객체 생성자.
    ** 인자		:	port(int) : 서버 포트 번호.
    ** 반환값	:	없음.
    */
    explicit MultiServer(int port);

    /*
    ** 함수명	:	~MultiServer.
    ** 설명		:	MultiServer 객체 소멸자.
    ** 인자		:	없음.
    ** 반환값	:	없음.
    */
    ~MultiServer();

    // 복사 생성자 및 복사 할당 연산자 삭제.
    MultiServer(const MultiServer& obj) = delete;
    MultiServer& operator=(const MultiServer& obj) = delete;

    // 이동 생성자 및 이동 할당 연산자 삭제.
    MultiServer(MultiServer&& obj) = delete;
    MultiServer& operator=(MultiServer&& obj) = delete;

public:
    /*
    ** 함수명	:	startServer.
    ** 설명		:	서버를 시작합니다.
    ** 인자		:	없음.
    ** 반환값	:	MultiServer::Result : 성공/실패 결과.
    */
    MultiServer::Result startServer();

    /*
    ** 함수명	:	runServerLoop.
    ** 설명		:	서버 메인 루프를 실행합니다.
    ** 인자		:	없음.
    ** 반환값	:	MultiServer::Result : 실행 결과.
    */
    MultiServer::Result runServerLoop();

    /*
    ** 함수명	:	stop.
    ** 설명		:	서버를 중지합니다.
    ** 인자		:	없음.
    ** 반환값	:	없음.
    */
    void stop();

    /*
    ** 함수명	:	isRunning.
    ** 설명		:	서버 실행 상태를 확인합니다.
    ** 인자		:	없음.
    ** 반환값	:	bool : 실행 중이면 true, 아니면 false.
    */
    bool isRunning() const;

private:
    int _port;
    TCPSocket _tcpSocket;
    ClientManager _clientManager;
    SelectManager _selectManager;
    MessageSender _messageSender;
    bool _isRunning;

private:
    /*
    ** 함수명	:	handleNewConnection.
    ** 설명		:	새로운 클라이언트 연결을 처리합니다.
    ** 인자		:	없음.
    ** 반환값	:	bool : 성공하면 true, 실패하면 false.
    */
    bool handleNewConnection();

    /*
    ** 함수명	:	handleClientMessage.
    ** 설명		:	클라이언트 메시지를 처리합니다.
    ** 인자		:	int client_index : 클라이언트 인덱스.
    ** 반환값	:	bool : 연결 유지하면 true, 연결 종료하면 false.
    */
    bool handleClientMessage(int client_index);

    /*
    ** 함수명	:	sendWelcomeMessage.
    ** 설명		:	새 클라이언트에게 환영 메시지를 보냅니다.
    ** 인자		:	int client_index : 클라이언트 인덱스.
    ** 반환값	:	없음.
    */
    void sendWelcomeMessage(int client_index);

    /*
    ** 함수명	:	announceJoin.
    ** 설명		:	새 클라이언트 참여를 알립니다.
    ** 인자		:	int client_index : 새로 참여한 클라이언트 인덱스.
    ** 반환값	:	없음.
    */
    void announceJoin(int client_index);

    /*
    ** 함수명	:	announceLeave.
    ** 설명		:	클라이언트 떠남을 알립니다.
    ** 인자		:	int client_index : 떠난 클라이언트 인덱스.
    ** 반환값	:	없음.
    */
    void announceLeave(int client_index);
};