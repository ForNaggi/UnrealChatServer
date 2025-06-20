#pragma once
#pragma execution_character_set("utf-8")

/**
 * @file MultiServer.h
 * @brief 다중 클라이언트 서버 로직을 수행하는 MultiServer 클래스를 정의합니다.
 * @author 최성락
 * @date 2025-06-17
 * 
 * @details
 * TCP 리스닝 소켓, 클라이언트 관리, 메시지 송수신 등을 하나로 묶어 select() 기반의 멀티클라이언트 서버를 구현합니다.
 * <br>서버 시작/정지와 메인 루프 제어 기능을 제공합니다.
 */

#include "TCPSocket.h"
#include "ClientManager.h"
#include "SelectManager.h"
#include "MessageSender.h"
#include "MessageReceiver.h"

/**
 * @class MultiServer
 * @brief select 기반 루프를 통해 다중 클라이언트 채팅 서버를 관리하는 클래스입니다.
 *
 * @details
 * MultiServer 클래스는 지정된 포트에서 서버를 시작합니다.
 * <br>신규 클라이언트 연결을 받아들입니다.
 * <br>등록된 클라이언트로부터 메시지를 수신합니다. (MessageReceiver 사용).
 * <br>등록된 클라이언트들에게 메시지를 발송합니다. (MessageSender 사용).
 * <br>서버 동작 전반을 관리합니다.
 * <br>각 단계에서의 오류를 나타내는 결과 상태 값(Result)을 제공합니다.
 */
class MultiServer
{
public:
    /**
     * @enum MultiServer::Result
     * @brief 서버 동작의 상태 값.
     */
    enum class Result
    {
        SUCCESS,        ///< 서버 작업(시작 또는 루프)이 성공적으로 완료됨.
        FAIL_START,     ///< 서버 시작에 실패함 (예: 포트를 바인드하거나 리슨하지 못함).
        FAIL_LOOP,      ///< 서버 메인 루프 실행 중 오류 발생.(예 : select 실패).
        SERVER_STOPPED  ///< 서버가 정상적으로 중지됨 (메인 루프 종료 신호로 사용).
    };

public:

    /**
     * @fn MultiServer::MultiServer(int port)
     * @brief 지정된 포트 번호로 MultiServer를 생성합니다.
     * @param[IN] int port : 서버가 클라이언트 연결을 대기할 TCP 포트 번호.
     * @return 없음.
     *
     * @details
     * 내부 구성 요소들(리스닝 소켓, 관리 객체 등)을 초기화합니다.
     * <br>서버는 아직 시작되지 않은 상태입니다.
     * <br>서버를 실제로 시작하려면 startServer()를 호출해야 합니다.
     */
    explicit MultiServer(int port);

    /**
     * @fn MultiServer::~MultiServer()
     * @brief 소멸자. 서버 리소스를 정리합니다.
     * @return 없음.
     * @note 소멸 시 서버가 중지되고 모든 소켓이 닫혀있어야 합니다.
     * <br>관리하던 구성 요소들은 각각의 소멸자를 통해 정리됩니다.
     */
    ~MultiServer();

    // 복사 생성자 및 복사 할당 연산자 삭제.
    MultiServer(const MultiServer& obj) = delete;
    MultiServer& operator=(const MultiServer& obj) = delete;

    // 이동 생성자 및 이동 할당 연산자 삭제.
    MultiServer(MultiServer&& obj) = delete;
    MultiServer& operator=(MultiServer&& obj) = delete;

public:
    /**
     * @fn MultiServer::Result MultiServer::startServer()
     * @brief 서버를 시작하여 리스닝 소켓을 초기화하고 클라이언트 수신을 시작합니다.
     * @return MultiServer::Result : 서버 리스닝을 시작했으면 SUCCESS, 실패하면 FAIL_START (예: 소켓 오류 등).
     *
     * @details
     * TCP 리스닝 소켓을 설정합니다(지정된 포트에 바인드하고 listen 호출).
     * <br>성공하면 서버가 클라이언트 연결 대기 상태가 됩니다.
     */
    MultiServer::Result startServer();

    /**
     * @fn MultiServer::Result MultiServer::runServerLoop()
     * @brief 메인 서버 루프를 실행하여 클라이언트 연결 및 메시지 처리를 수행합니다.
     * @return MultiServer::Result : 정상 종료 시 SERVER_STOPPED, 오류로 루프가 종료되면 FAIL_LOOP.
     *
     * @details
     * 클라이언트 추가, 메시지 송수신, 에러 처리 등을 수행합니다.
     * select()를 사용하여 소켓들의 상태를 감시하는 루프에 진입합니다.
     * - 새로운 클라이언트 연결을 받아들입니다. (ClientManager에 등록). 
     * - 기존 클라이언트들의 메시지를 수신합니다. (MessageReceiver 사용).
     * - 클라이언트에게 받은 메시지를 다른 클라이언트들에게 전달합니다. (MessageSender 사용).
     * `stop()`이 호출되거나 치명적인 오류가 발생할 때까지 루프를 지속합니다.
     */
    MultiServer::Result runServerLoop();

    /**
     * @fn void MultiServer::stop()
     * @brief 서버에 더 이상 새로운 연결을 받지 말고 메인 루프를 종료하도록 신호를 보냅니다.
     * @return 없음.
     * 
     * @note 이 함수를 호출하면 내부적으로 서버 종료 플래그를 설정합니다.
     * <br>루프가 종료되고 자원이 해제됩니다.
     */
    void stop();

    /**
     * @fn bool MultiServer::isRunning() const
     * @brief 서버가 현재 실행 중인지 여부를 확인합니다.
     * @return bool : 서버가 시작되어 아직 중지되지 않았으면 true, 그 외의 경우 false.
     */
    bool isRunning() const;

private:
    /// 서버가 수신 대기하는 TCP 포트 번호.
    int _port;
    /// 리스닝(클라이언트 받기용) TCP 소켓(create, bind, listen, accept 관리).
    TCPSocket _tcpSocket;
    /// 연결된 클라이언트 소켓들과 별칭을 관리하는 객체.
    ClientManager _clientManager;
    /// fd_set 구성 및 select() 호출하는 객체.
    SelectManager _selectManager;
    /// 서버에서 클라이언트들에게 메시지를 보내는 객체.
    MessageSender _messageSender;
    /// 서버 루프 실행 여부를 나타내는 플래그.
    bool _isRunning;

private:
    /**
     * @fn bool MultiServer::handleNewConnection()
     * @brief 새로운 클라이언트 연결을 수락하여 ClientManager에 등록합니다.
     * @return bool : 새로운 클라이언트를 성공적으로 추가했으면 true, accept 실패 또는 최대 클라이언트 초과 시 false.
     *
     * @details
     * 메인 루프 내부에서 새로운 연결 발생이 감지되었을 때 호출됩니다.
     * <br>TCPSocket을 통해 연결을 accept하고 ClientManager에 등록합니다.
     * <br>새 클라이언트에게 환영 메시지를 보내고 다른 클라이언트들에게 참여를 알립니다.
     */
    bool handleNewConnection();

    /**
     * @fn bool MultiServer::handleClientMessage(int client_index)
     * @brief 지정된 인덱스의 클라이언트로부터 들어온 메시지를 처리합니다.
     * @param[IN] int client_index : ClientManager에서 관리하는 클라이언트의 인덱스.
     * @return bool : 클라이언트 메시지를 성공적으로 처리한 경우 true를 반환하고, 연결 종료가 필요하면 false를 반환합니다 (예: "quit").
     *
     * @details
     * 메인 루프에서 클라이언트 소켓에 읽기 이벤트가 발생하면 호출됩니다.
     * <br>MessageReceiver를 사용하여 해당 클라이언트의 메시지를 받아옵니다.
     * <br>클라이언트가 quit 명령을 보내거나 연결이 끊어진 경우 적절히 처리합니다.
     * <br>외에 정상적인 메세지는 MessageSender를 통해 다른 클라이언트들에게 메시지를 전달합니다.
     * <br>함수가 false를 반환하면 해당 클라이언트를 제거해야 함을 의미합니다.
     */
    bool handleClientMessage(int client_index);

    /**
     * @fn void MultiServer::sendWelcomeMessage(int client_index)
     * @brief 새로 연결된 클라이언트에게 환영 메시지를 보냅니다.
     * @param[IN] int client_index : ClientManager에서 할당한 새 클라이언트의 인덱스.
     * @return 없음.
     *
     * @details
     * 새로 연결된 클라이언트의 닉네임과 현재 접속 중인 클라이언트 수를 포함한 
     * <br>환영 메시지를 생성하여 해당 클라이언트에게 전송합니다.
     */
    void sendWelcomeMessage(int client_index);

    /**
     * @fn void MultiServer::announceJoin(int client_index)
     * @brief 새로운 클라이언트가 참가했음을 모든 클라이언트에게 알립니다.
     * @param[IN] int client_index : 새로 참가한 클라이언트의 인덱스.
     * @return 없음.
     * 
     * @details 
     * 새로 참여한 클라이언트를 제외한 나머지 모든 클라이언트들에게 
     * <br>해당 클라이언트가 채팅방에 참여했음을 알리는 메시지를 전송합니다.
     */
    void announceJoin(int client_index);

    /**
     * @fn void MultiServer::announceLeave(int client_index)
     * @brief 특정 클라이언트가 떠났음을 모든 클라이언트에게 알립니다.
     * @param[IN] int client_index : 떠나는 클라이언트의 인덱스.
     * @return 없음.
     *
     * @details
     * 나머지 클라이언트들에게 주어진 인덱스의 클라이언트가 채팅을 떠났음을 알리는 메시지를 방송합니다.
     */
    void announceLeave(int client_index);

    /**
     * @fn std::string MultiServer::makeWecomeMessage(const std::string& nickname, int connectedClientCount)
     * @brief 새로 연결된 클라이언트를 위한 환영 메시지를 생성합니다.
     * @param[IN] const std::string& nickname : 새 클라이언트의 닉네임.
     * @param[IN] int connectedClientCount : 현재 연결된 클라이언트의 총 수 (이번 클라이언트 포함).
     * @return std::string 새 클라이언트를 위한 환영 메시지 문자열.
     *
     * @details
     * === 채팅 서버에 오신 것을 환영합니다! ===
     * <br>현재 접속자 수: <connectedClientCount>명
     * <br>'quit'를 입력하면 종료됩니다.
     */
    std::string makeWecomeMessage(const std::string& nickname, int connectedClientCount);
};