#pragma once

#include "SocketIniter.h"
#include "MultiServer.h"

class Program
{
	enum class Result
	{
		FAIL,
		SUCCESS
	};

public:
	/*
	** 메소드명	:	Program.
	** 설명		:	Program 객체 생성자.
	*				내부 멤버 변수 (_socketIniter, _multiServer)를 초기화합니다.
	** 인자값	:	없음.
	** 출력값	:	없음.
	*/
	Program();

	/*
	** 메소드명	:	~Program.
	** 설명		:	Program 객체 소멸자.
	** 인자값	:	없음.
	** 출력값	:	없음.
	*/
	~Program();

	// 복사 생성자 및 복사 할당 연산자 삭제.
	Program(const Program& obj) = delete;
	Program& operator=(const Program& obj) = delete;

	// 이동 생성자 및 이동 할당 연산자 삭제.
	Program(Program&& obj) = delete;
	Program& operator=(Program&& obj) = delete;

public:
	/*
	** 함수명	:	run.
	** 설명		:	서버 프로그램을 시작합니다.
	** 인자		:	없음.
	** 반환값	:	int - 프로그램 종료 flag.
	*/
	int run();

private:
	// winsock을 초기화를 담당하는 객체.
	SocketIniter _socketIniter;

	// 멀티클라이언트 서버 객체.
	MultiServer _multiServer;

	// 포트 번호.
	static const int SERVER_PORT = 5500;

private:
	/*
	** 함수명	:	initialize.
	** 설명		:	서버 프로그램을 초기화합니다.
	** 인자		:	없음.
	** 반환값	:	Program::Result.
	*/
	Program::Result initialize();

	/*
	** 함수명	: startMultiServer.
	** 설명		: 멀티클라이언트 서버를 시작합니다.
	** 인자		: 없음.
	** 반환값	: Program::Result.
	*/
	Program::Result startMultiServer();

	/*
	** 함수명	:	runServerLoop.
	** 설명		:	서버 메인 루프.
	** 인자		:	없음.
	** 반환값	:	없음.
	*/
	void runServerLoop();
};