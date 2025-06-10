#include "DebugHelper.h"
#include "MemoryLeakHelper.h"
#include "Program.h"

int main()
{
	RUN_MEMORY_LEAK_CHECK;

	LOG_INFO("프로그램을 시작합니다.");

	Program TCPServer;
	
	int result = TCPServer.run();

	LOG_INFO("프로그램을 종료합니다.");

	return (result);

}
