#pragma execution_character_set("utf-8")

/**
 * @file main.cpp
 * @brief 멀티 클라이언트 서버 프로그램 main.
 * @author 최성락
 * @date 2025-06-17
 */

#include "DebugHelper.h"
#include "MemoryLeakHelper.h"
#include "Program.h"
#include <Windows.h>
#include <io.h>
#include <fcntl.h>

int main()
{
	// 콘솔 입출력 인코딩을 UTF-8로 설정.
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	// 프로그램 종료 시 메모리 릭을 체크합니다.
	RUN_MEMORY_LEAK_CHECK;

	LOG_INFO("프로그램을 시작합니다.");

	Program TCPServer;
	
	int result = TCPServer.run();

	LOG_INFO("프로그램을 종료합니다.");

	return (result);

}
