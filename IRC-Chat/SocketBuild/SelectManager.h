#pragma once

#include <WinSock2.h>

/*
** 클래스명	:	SelectManager.
** 설명		:	select 기반 멀티 플렉싱의 핵심 기능 담당.
*				select 함수 사용, fd_set을 관리.
** 초기화	:	기본 생성자만 사용.
*/
class SelectManager
{
public:

	// 해당 클래스의 메소드의 결과를 나타내는 enum class입니다.
	enum class Result
	{
		SUCCESS,		// select 성공, 활성 소켓 있음.
		TIMEOUT,		// select 타임아웃 발생.
		FAIL_SELECT,	// select 함수 실패.
						// (fd_set에서 감시할 소켓이 닫힌 소켓이거나, 초기화되지 않은 소켓).
		NO_SOCKETS		// 감시할 소켓이 없음.
	};

public:

	/*
	** 함수명	:	SelectManager.
	** 설명		:	SelectManager 객체 생성자.
	*				fd_set을 초기화합니다.
	** 인자		:	없음.
	** 반환값	:	없음.
	*/
	SelectManager();

	/*
	** 함수명	:	~SelectManager.
	** 설명		:	SelectManager 객체 소멸자.
	** 인자		:	없음.
	** 반환값	:	없음.
	*/
	~SelectManager();

	// 복사 생성자 및 복사 할당 연산자 삭제.
	SelectManager(const SelectManager& obj) = delete;
	SelectManager& operator=(const SelectManager& obj) = delete;

	// 이동 생성자 및 이동 할당 연산자 삭제.
	SelectManager(SelectManager&& obj) = delete;
	SelectManager& operator=(SelectManager&& obj) = delete;

public:

	/*
	** 함수명	:	setupFdSet.
	** 설명		:	SelectManager을 멤버 변수 값(_originSet, _socketCount)을 초기화합니다.
	** 인자		:	없음.
	** 반환값	:	없음.
	*/
	void setupFdSet();

	/*
	** 함수명	:	addSocket.
	** 설명		:	감시할 소켓을 fd_set에 추가합니다.
	** 인자		:	SOCKET socket : 추가할 소켓.
	** 반환값	:	bool : 성공하면 true, 실패하면 false.
	*/
	bool addSocket(SOCKET socket);

	/*
	** 함수명	:	excuteSelect.
	** 설명		:	fd_set을 복사하여 select 함수를 실행합니다.
	** 인자		:	int timeout_sec : 타임아웃 초(default = 1).
	** 반환값	:	SelectManager::Result : select 실행 결과.
	*/
	SelectManager::Result excuteSelect(int timeout_sec = 1);


	/*
	** 함수명	:	isSocketReady.
	** 설명		:	인자로 넘어온 소캣이 읽기 준비 상태인지를 확인합니다.
	** 인자		:	SOCKET socket : 확인할 소켓.
	** 반환값	:	bool : 준비되었으면 true, 아니면 false.
	*/
	bool isSocketReady(SOCKET socket) const;

private:

	// 원본 fd_set(_originSet을 _copySet에 복사합니다.). 
	fd_set _originSet;

	// _originSet을 복사하여 select에 전달됩니다.
	fd_set _copySet;

	// 현재 감시 중인 소켓 갯수.
	int  _socketCount;
};

