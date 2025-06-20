#pragma execution_character_set("utf-8")


/**
 * @file Program.cpp
 * @brief Program.h 구현부.
 * @author 최성락
 * @date 2025-06-17
 */

#include "Program.h"
#include "DebugHelper.h"

Program::Program()
    : _socketIniter(), _multiServer(Program::SERVER_PORT)
{
    LOG_INFO("Select 기반 멀티클라이언트 서버 프로그램을 시작합니다.");
}

Program::~Program()
{
    LOG_INFO("Select 기반 멀티클라이언트 서버 프로그램을 종료합니다.");
}

int Program::run()
{
    // 서버 프로그램을 돌리기 전 필요한 초기화를 진행합니다.
    if (this->initialize() == Program::Result::FAIL)
    {
        return (-1);
    }

    // 서버를 부팅합니다.
    if (this->startMultiServer() == Program::Result::FAIL)
    {
        return (-1);
    }

    // 멀티 서버 가동중.
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

Program::Result Program::startMultiServer()
{
    // 멀티클라이언트 서버를 부팅하고 소켓을 listen 대기로 합니다.
    if (this->_multiServer.startServer() != MultiServer::Result::SUCCESS)
    {
        LOG_ERROR("멀티클라이언트 서버 시작에 실패했습니다.");
        return (Program::Result::FAIL);
    }

    LOG_INFO("Select 기반 멀티클라이언트 서버가 성공적으로 시작되었습니다.");
    return (Program::Result::SUCCESS);
}

void Program::runServerLoop()
{
    LOG_INFO("멀티클라이언트 서버 메인 루프를 시작합니다.");
    LOG_INFO("최대 " + std::to_string(ClientManager::MAX_CLIENTS) + "명의 클라이언트가 동시 접속 가능합니다.");

    // 멀티 서버 메인 루프 실행
    MultiServer::Result result = this->_multiServer.runServerLoop();

    // 결과에 따른 로그 출력
    switch (result)
    {
    case MultiServer::Result::SUCCESS:
        LOG_INFO("멀티클라이언트 서버가 정상적으로 종료되었습니다.");
        break;
    case MultiServer::Result::FAIL_LOOP:
        LOG_ERROR("서버 루프 실행 중 오류가 발생했습니다.");
        break;
    case MultiServer::Result::SERVER_STOPPED:
        LOG_INFO("서버가 사용자 요청에 의해 중지되었습니다.");
        break;
    default:
        LOG_WARN("알 수 없는 서버 종료 상태입니다.");
        break;
    }
}