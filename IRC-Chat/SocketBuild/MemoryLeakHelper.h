#pragma once
#pragma execution_character_set("utf-8")

/**
 * @file MemoryLeakHelper.h
 * @brief 디버그 모드(MSVC 전용)의 메모리 누수 감지 유틸리티.
 * @author 최성락
 * @date 2025-06-17
 *
 * @details
 * 디버그 모드에서 메모리 할당 추적과 종료 시 누수 체크를 가능하게 하는 매크로와 함수를 정의합니다.
 * <br>(릴리스 모드에서는 효과가 없습니다.)
 */

// MSVC(컴파일러)일 경우.
#ifdef _MSC_VER
	// malloc, calloc, realloc, free 같은 표준 c 메모리 함수들이 디버그용 버전으로 치환됩니다.
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>

	// 디버그 모드일 경우.
	#ifdef _DEBUG

		/**
		 * @def DBG_NEW
		 * @brief new 연산자를 대체하여 파일과 라인 정보를 포함시키는 매크로 (디버그 모드).
		 *
		 * 디버그 빌드에서는 MSVC의 디버그 힙 할당을 사용하여 파일/라인 정보를 기록합니다.
		 * <br>예: `new MyObject()`를 사용하면 `DBG_NEW MyObject()`로 확장됩니다.
		 */
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		/// @brief 디버그 빌드에서 전역 new를 DBG_NEW로 재정의.
		#define new DBG_NEW

		/**
		 * @fn void memory_leak_check()
		 * @brief 프로그램 종료 시 메모리 누수를 확인합니다 (디버그 모드 전용).
		 * @return 없음.
		 *
		 * @details
		 * 이 함수를 (또는 RUN_MEMORY_LEAK_CHECK 매크로를) 프로그램 종료 시 호출하면
		 * 메모리 누수 리포트를 출력합니다.
		 * <br>내부적으로 CRT 플래그를 설정하여 종료 시 메모리 누수를 덤프합니다.
		 */
		inline void memory_leak_check()
		{
			// _CRTDBG_ALLOC_MEM_DF : 메모리 할당 추적 정보를 기록.
			// _CRTDBG_LEAK_CHECK_DF : 프로그램이 종료될 때 메모리 누수 보고서를 출력.
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		}

		/**
		 * @def RUN_MEMORY_LEAK_CHECK
		 * @brief memory_leak_check() 함수를 실행하는 매크로 (디버그 모드 전용).
		 *
		 * @details
		 * 이 매크로를 추가하면 프로그램 종료 시 자동으로 메모리 누수 체크를 수행합니다.
		 */
		#define RUN_MEMORY_LEAK_CHECK memory_leak_check()

	#else
		// 디버그 모드가 아니면 작동 안되게 하려고 심어놓은 함수.
		inline void memory_leak_check() {}

		// 해당 표현은 컴파일 단계에서 완전히 삭제됩니다.
		#define RUN_MEMORY_LEAK_CHECK ((void)0)

	#endif
#endif