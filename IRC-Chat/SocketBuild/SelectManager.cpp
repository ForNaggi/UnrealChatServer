#include "SelectManager.h"
#include "DebugHelper.h"

static timeval make_timer(long sec, long usec)
{
    timeval timeout;
    timeout.tv_sec = sec;
    timeout.tv_usec = usec;
    return (timeout);
}

SelectManager::SelectManager()
    : _originSet(), _copySet(), _socketCount(0)
{
    LOG_DEBUG("SelectManager 객체를 생성합니다.");
}

SelectManager::~SelectManager()
{
    LOG_DEBUG("SelectManager 객체를 삭제합니다.");
}

void SelectManager::setupFdSet()
{
    // fd_set 초기화.
    FD_ZERO(&this->_originSet);
    this->_socketCount = 0;

    LOG_DEBUG("fd_set과 소켓 count를 초기화합니다.");
}

bool SelectManager::addSocket(SOCKET socket)
{
    // 유효한 소켓인지 확인합니다.
    if (socket == INVALID_SOCKET)
    {
        LOG_WARN("클라이언트 소켓 등록 시 유효하지 않은 소켓이 확인되었습니다.");
        return (false);
    }

    // fd_set에 클라이언트 소켓을 추가합니다.
    FD_SET(socket, &this->_originSet);
    this->_socketCount = this->_socketCount + 1;

    LOG_DEBUG("클라이언트 소켓이 fd_set에 추가되었습니다.:\n현재 소켓 수 : " + std::to_string(this->_socketCount));
    return (true);
}

SelectManager::Result SelectManager::excuteSelect(int timeout_sec)
{
    // 감시할 소켓이 없으면 바로 탈출합니다.
    if (this->_socketCount == 0)
    {
        LOG_DEBUG("감시할 소켓이 없습니다.");
        return (SelectManager::Result::NO_SOCKETS);
    }

    // fd_set을 복사합니다.
    // select 함수가 인자의 fd_set을 수정하므로 원본을 수정하지 않기 위해 복사해서 사용합니다.
    this->_copySet = this->_originSet;

    // select에 사용할 timeout 설정.
    timeval timeout = make_timer(timeout_sec, 0);

    // Windows에서는 select함수의 첫번 째 매개변수가 무시됩니다.(원래는 최대 값의 소켓 id).
    // 읽기 준비가 된 소켓을 수를 리턴합니다.
    int result = select(0, &this->_copySet, nullptr, nullptr, &timeout);

    // select 결과에 따른 분기.
    if (result == SOCKET_ERROR)
    {
        LOG_ERROR("slect 함수 실행 실패\n 에러 코드: " + std::to_string(WSAGetLastError()));
        return (SelectManager::Result::FAIL_SELECT);
    }
    else if (result == 0)
    {
        // 타임아웃 발생.
        LOG_DEBUG("select 타임아웃 발생");
        return (SelectManager::Result::TIMEOUT);
    }
    else
    {
        // 하나 이상의 소켓이 준비됨.
        LOG_DEBUG("select 성공, 준비된 소켓 수: " + std::to_string(result));
        return (SelectManager::Result::SUCCESS);
    }
}

bool SelectManager::isSocketReady(SOCKET socket) const
{
    // FD_ISSET를 사용하여 fd_set에서 소켓이 읽기가 준비되었는지 체크합니다.
    // select() 실행 후 복사된 fd_set에는 읽기가 준비된 소켓만 남아있습니다.
    return (FD_ISSET(socket, &this->_copySet));
}
