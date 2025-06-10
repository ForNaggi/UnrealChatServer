#pragma once

// MSVC(컴파일러)일 경우.
#ifdef _MSC_VER
	// malloc, calloc, realloc, free 같은 표준 c 메모리 함수들이 디버그용 버전으로 치환됩니다.
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>

	// 디버그 모드일 경우.
	#ifdef _DEBUG
		/*
		** 함수명	:	DBG_NEW(NEW).
		** 설명		:	new를 사용시 DBG_NEW로 치환되며 파일과 라인 추척이 가능하다.
						(MSVC(CRT 디버깅 라이브러리)의 디버그용 new 연산자 오버로드를 호출하는 방식).
		** 인자		:	_NORMAL_BLOCK : 할당된 블록.
						__FILE__ : 이 매크로가 쓰인 소스 파일 이름 자동 치환.
						__LINE__ : 이 매크로가 쓰인 코드 줄 번호.			
		** 반환값	:	없음.
		*/
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW

		/*
		** 함수명	:	MemoryLeakCheck.
		** 설명		:	프로그램 종료 시 메모리 leak을 체크해줍니다.
		** 인자		:	없음.
		** 반환값	:	없음.
		*/
		inline void memory_leak_check()
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		}

		// memory_leak_check 함수를 매크로로 사용할 수 있게 해준다.
		#define RUN_MEMORY_LEAK_CHECK memory_leak_check()

	#else
		// 디버그 모드가 아니면 작동 안되게 하려고 심어놓은 함수.
		inline void memory_leak_check() {}

		// 해당 표현은 컴파일 단계에서 완전히 삭제됩니다.
		#define RUN_MEMORY_LEAK_CHECK ((void)0)

	#endif
#endif