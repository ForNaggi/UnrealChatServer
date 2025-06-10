#include "Program.h"
#include "DebugHelper.h"

Program::Program()
    : _socketIniter(), _echoServer(Program::SERVER_PORT)
{
    LOG_INFO("에코 서버 프로그램을 시작합니다.");
}

Program::~Program()
{
    LOG_INFO("에코 서버 프로그램을 종료합니다.");
}

int Program::run()
{
    // 서버 프로그램을 돌리기 전 필요한 초기화를 진행합니다.
	if (this->initialize() == Program::Result::FAIL)
	{
		return (-1);
	}

    // 서버를 빌드합니다.
	if (this->startTCPServer() == Program::Result::FAIL)
	{
		return (-1);
	}
	
    // echo 서버 가동중.
	this->runServerLoop();
	return (0);
}

Program::Result Program::initialize()
{
    // 소켓 사용을 위해 초기화를 진행합니다.
	if (this->_socketIniter.init() == SocketIniter::Result::FAIL_SOCKET)
	{
        // 실패 시.
		LOG_ERROR("소켓 초기화에 실패했습니다. 프로그램을 종료합니다.");

		return (Program::Result::FAIL);
	}

	LOG_INFO("프로그램 초기화 완료했습니다.");

	return (Program::Result::SUCCESS);
}

Program::Result Program::startTCPServer()
{
    // echo서버를 빌드하고 소켓을 listen 대기로 합니다.
    if (this->_echoServer.startServer() != EchoServer::Result::SUCCESS)
    {
        LOG_ERROR("서버 시작에 실패했습니다.");
        return (Program::Result::FAIL);
    }

    LOG_INFO("에코 서버가 성공적으로 시작되었습니다.");
    return (Program::Result::SUCCESS);
}

void Program::runServerLoop()
{
    LOG_INFO("새로운 클라이언트 연결을 대기합니다..");

    // 서버 가동 중.
    while (this->_echoServer.isRunning())
    {

        // 클라이언트와의 연결을 대기하고 연결 시 문자를 주고 받습니다.
        EchoServer::Result result = this->_echoServer.awaitClientConnect();
        // 클라이언트와의 종료 조건을 구분합니다.
        this->handleClientResult(result);

        LOG_INFO("다음 클라이언트를 대기합니다..");
    }
}

void Program::handleClientResult(EchoServer::Result result)
{
    // 클라이언트와의 종료 조건을 구분합니다.
    switch (result)
    {
        // 클라이언트가 quit로 종료한 경우.
        case EchoServer::Result::CLIENT_HANDLED:
            LOG_INFO("클라이언트와의 통신이 정상적으로 종료되었습니다.");
            break ;
        case EchoServer::Result::FAIL_ACCEPT:
            LOG_ERROR("클라이언트 연결 수락에 실패했습니다.");
            break ;
        case EchoServer::Result::CLIENT_ERROR:
            LOG_ERROR("클라이언트 처리 중 오류가 발생했습니다.");
            break ;
        default:
            LOG_DEBUG("알 수 없는 클라이언트 처리 결과입니다.");
            break ;
    }
}
