#pragma once

// MSVC(�����Ϸ�)�� ���.
#ifdef _MSC_VER
	// malloc, calloc, realloc, free ���� ǥ�� c �޸� �Լ����� ����׿� �������� ġȯ�˴ϴ�.
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>

	// ����� ����� ���.
	#ifdef _DEBUG
		/*
		** �Լ���	:	DBG_NEW(NEW)
		** ����		:	new�� ���� DBG_NEW�� ġȯ�Ǹ� ���ϰ� ���� ��ô�� �����ϴ�.
						(MSVC(CRT ����� ���̺귯��)�� ����׿� new ������ �����ε带 ȣ���ϴ� ���)
		** ����		:	_NORMAL_BLOCK : �Ҵ�� ���
						__FILE__ : �� ��ũ�ΰ� ���� �ҽ� ���� �̸� �ڵ� ġȯ.
						__LINE__ : �� ��ũ�ΰ� ���� �ڵ� �� ��ȣ.			
		** ��ȯ��	:	����
		*/
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW

		/*
		** �Լ���	:	MemoryLeakCheck
		** ����		:	���α׷� ���� �� �޸� leak�� üũ���ݴϴ�.
		** ����		:	����
		** ��ȯ��	:	����
		*/
		inline void memory_leak_check()
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		}

		// memory_leak_check �Լ��� ��ũ�η� ����� �� �ְ� ���ش�.
		#define RUN_MEMORY_LEAK_CHECK memory_leak_check()

	#else
		// ����� ��尡 �ƴϸ� �۵� �ȵǰ� �Ϸ��� �ɾ���� �Լ�.
		inline void memory_leak_check() {}

		// �ش� ǥ���� ������ �ܰ迡�� ������ �����˴ϴ�.
		#define RUN_MEMORY_LEAK_CHECK ((void)0)

	#endif
#endif