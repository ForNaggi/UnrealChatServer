#pragma once

#include "SocketIniter.h"
#include "EchoServer.h"

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
		*				내부 멤버 변수 (_socketIniter, _echoServer)를 초기화합니다.
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

		// socket을 이용해서 echo 서버를 생성 관리하는 객체.
		EchoServer _echoServer;

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
		** 함수명	: startTCPServer.
		** 설명		: echo 서버를 시작합니다.
		** 인자		: 없음.
		** 반환값	: Program::Result.
		*/
		Program::Result startTCPServer();


		/*
		** 함수명	:	runServerLoop.
		** 설명		:	서버 메인 루프.
		** 인자		:	없음.
		** 반환값	:	없음.
		*/
		void runServerLoop();

		/*
		** 함수명	:	handleClientResult.
		** 설명		:	클라이언트 소켓과의 종료 결과에 따라 해당하는 분기를 선택합니다.
		** 인자		:	EchoServer::Result result : 클라이언트 소켓 종료 처리 결과.
		** 반환값	:	없음.
		*/
		void handleClientResult(EchoServer::Result result);
};