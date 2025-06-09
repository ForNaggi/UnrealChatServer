#include "DebugHelper.h"
#include "MemoryLeakHelper.h"
#include "SocketIniter.h"

int main()
{
	RUN_MEMORY_LEAK_CHECK;

	LOG_INFO("프로그램을 시작합니다.");

	SocketIniter socket_initer;
	if (socket_initer.init() == SocketIniter::Result::FAIL_SOCKET)
	{
		LOG_ERROR("소켓 초기화에 실패했습니다. 프로그램을 종료합니다.");
		return (-1);
	}

	LOG_INFO("프로그램이 정상적으로 동작했습니다.");
	LOG_INFO("프로그램을 종료합니다.");

	return (0);
}