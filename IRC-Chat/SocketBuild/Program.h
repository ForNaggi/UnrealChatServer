#pragma once
#pragma execution_character_set("utf-8")

/**
 * @file Program.h
 * @brief 채팅 서버 프로그램을 초기화하고 실행하는 클래스를 정의합니다.
 * @author 최성락
 * @date 2025-06-17
 * 
 * @details
 * Program 클래스는 프로그램의 시작과 종료를 캡슐화합니다. 
 * <br>WinSock을 초기화하고, MultiServer를 시작하며, 서버 루프를 실행합니다.
 * <br>이 클래스는 `main` 함수에서 전체적인 프로그램 흐름을 관리하는 데 사용됩니다.
 */

#include "SocketIniter.h"
#include "MultiServer.h"

/**
 * @class Program
 * @brief 채팅 서버 프로그램의 시작, 실행 및 종료를 제어합니다.
 *
 * @details
 * Program 클래스는 필요한 하위 시스템(WinSock 초기화 등)을 설정합니다. 
 * <br>MultiServer를 인스턴스화합니다.
 * <br>서버를 시작하여 메인 루프를 실행하고 서버를 중지하는 순서를 관리합니다.
 * <br>`run()` 메서드를 제공하여 전체 과정을 시작할 수 있습니다.
 */
class Program
{
	/**
	 * @enum Program::Result
	 * @brief 내부 초기화 단계의 결과 상태 값입니다.
	 */
	enum class Result
	{
		FAIL,		///< 단계가 실패한 경우 (예: WinSock 초기화 또는 서버 시작 실패).
		SUCCESS		///< 단계가 성공적으로 완료된 경우.
	};

public:
	/**
	 * @fn Program::Program()
	 * @brief Program 객체를 생성하고 하위 구성 요소를 초기화합니다.
	 * @return 없음.
	 *
	 * @details
	 * SocketIniter(WinSock 초기화 담당)와 MultiServer를 초기화합니다.
	 */
	Program();

	/**
	 * @fn Program::~Program()
	 * @brief Program의 소멸자.
	 * @return 없음.
	 * 
	 * @note 모든 구성 요소가 올바르게 종료되도록 보장합니다.
	 * <br>소켓 정리는 SocketIniter와 MultiServer의 소멸자에서 처리됩니다.
	 */
	~Program();

	// 복사 생성자 및 복사 할당 연산자 삭제.
	Program(const Program& obj) = delete;
	Program& operator=(const Program& obj) = delete;

	// 이동 생성자 및 이동 할당 연산자 삭제.
	Program(Program&& obj) = delete;
	Program& operator=(Program&& obj) = delete;

public:
	/**
	 * @fn int Program::run()
	 * @brief 서버 프로그램을 실행하여 초기화하고, 서버 루프에 진입합니다.
	 * @return int : 프로그램 종료 상태 값 (정상 종료 시 0, 오류 발생 시 -1).
	 *
	 * @details
	 * 이 메서드는 다음과 같은 주요 단계들을 수행합니다:
	 * - initialize()를 호출하여 네트워킹(WinSock)을 초기화합니다.
	 * - startMultiServer()를 호출하여 서버를 시작합니다.
	 * - 서버가 성공적으로 시작되면 runServerLoop()를 호출하여 서버 루프에 진입하고 클라이언트 상호 작용을 처리합니다.
	 * - 루프가 종료되거나 오류가 발생하면 정수 상태 코드를 반환하고, 이 값은 프로세스 종료 코드로 사용될 수 있습니다.
	 */
	int run();

private:
	/// WinSock(Windows Sockets API)의 초기화와 정리를 담당하는 객체.
	SocketIniter _socketIniter;

	/// 클라이언트 연결과 메시지를 처리하는 다중 클라이언트 서버 객체.
	MultiServer _multiServer;

	/// 서버가 사용할 TCP 포트 번호 (기본값: 5500).
	static const int SERVER_PORT = 5500;

private:
	/**
	 * @fn Program::Result Program::initialize()
	 * @brief 프로그램에 필요한 리소스를 초기화합니다 (예: WinSock).
	 * @return Program::Result : 초기화에 성공하면 SUCCESS, 실패하면 FAIL.
	 *
	 * @note
	 * WinSock을 시작하기 위해 SocketIniter::init()을 호출합니다. 
	 * <br>이 호출이 실패하면 서버를 실행할 수 없습니다.
	 */
	Program::Result initialize();

	/**
	 * @fn Program::Result Program::startMultiServer()
	 * @brief MultiServer 인스턴스를 생성하고 시작합니다.
	 * @return Program::Result : 서버 시작에 성공하면 SUCCESS, 서버 시작 실패 시 FAIL.
	 *
	 * @details
	 * SERVER_PORT로 MultiServer를 초기화합니다.
	 * <br>MultiServer::startServer()를 호출합니다. 
	 * <br>서버가 클라이언트의 접속을 받을 준비를 마칩니다.
	 */
	Program::Result startMultiServer();

	/**
	 * @fn void Program::runServerLoop()
	 * @brief 서버를 계속 실행하며 이벤트에 응답하도록 서버 메인 루프에 진입합니다.
	 * @return 없음.
	 *
	 * @details
	 * MultiServer::runServerLoop()를 호출하여 클라이언트 연결과 메시지를 처리합니다. 
	 * <br>이 함수는 서버 루프가 종료될 때까지(오류 발생 또는 중지 신호 수신 시) loop합니다.
	 */
	void runServerLoop();
};