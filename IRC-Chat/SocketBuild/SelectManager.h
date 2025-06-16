#pragma once

#include <WinSock2.h>

/*
** Ŭ������	:	SelectManager.
** ����		:	select ��� ��Ƽ �÷����� �ٽ� ��� ���.
*				select �Լ� ���, fd_set�� ����.
** �ʱ�ȭ	:	�⺻ �����ڸ� ���.
*/
class SelectManager
{
public:

	// �ش� Ŭ������ �޼ҵ��� ����� ��Ÿ���� enum class�Դϴ�.
	enum class Result
	{
		SUCCESS,		// select ����, Ȱ�� ���� ����.
		TIMEOUT,		// select Ÿ�Ӿƿ� �߻�.
		FAIL_SELECT,	// select �Լ� ����.
						// (fd_set���� ������ ������ ���� �����̰ų�, �ʱ�ȭ���� ���� ����).
		NO_SOCKETS		// ������ ������ ����.
	};

public:

	/*
	** �Լ���	:	SelectManager.
	** ����		:	SelectManager ��ü ������.
	*				fd_set�� �ʱ�ȭ�մϴ�.
	** ����		:	����.
	** ��ȯ��	:	����.
	*/
	SelectManager();

	/*
	** �Լ���	:	~SelectManager.
	** ����		:	SelectManager ��ü �Ҹ���.
	** ����		:	����.
	** ��ȯ��	:	����.
	*/
	~SelectManager();

	// ���� ������ �� ���� �Ҵ� ������ ����.
	SelectManager(const SelectManager& obj) = delete;
	SelectManager& operator=(const SelectManager& obj) = delete;

	// �̵� ������ �� �̵� �Ҵ� ������ ����.
	SelectManager(SelectManager&& obj) = delete;
	SelectManager& operator=(SelectManager&& obj) = delete;

public:

	/*
	** �Լ���	:	setupFdSet.
	** ����		:	SelectManager�� ��� ���� ��(_originSet, _socketCount)�� �ʱ�ȭ�մϴ�.
	** ����		:	����.
	** ��ȯ��	:	����.
	*/
	void setupFdSet();

	/*
	** �Լ���	:	addSocket.
	** ����		:	������ ������ fd_set�� �߰��մϴ�.
	** ����		:	SOCKET socket : �߰��� ����.
	** ��ȯ��	:	bool : �����ϸ� true, �����ϸ� false.
	*/
	bool addSocket(SOCKET socket);

	/*
	** �Լ���	:	excuteSelect.
	** ����		:	fd_set�� �����Ͽ� select �Լ��� �����մϴ�.
	** ����		:	int timeout_sec : Ÿ�Ӿƿ� ��(default = 1).
	** ��ȯ��	:	SelectManager::Result : select ���� ���.
	*/
	SelectManager::Result excuteSelect(int timeout_sec = 1);


	/*
	** �Լ���	:	isSocketReady.
	** ����		:	���ڷ� �Ѿ�� ��Ĺ�� �б� �غ� ���������� Ȯ���մϴ�.
	** ����		:	SOCKET socket : Ȯ���� ����.
	** ��ȯ��	:	bool : �غ�Ǿ����� true, �ƴϸ� false.
	*/
	bool isSocketReady(SOCKET socket) const;

private:

	// ���� fd_set(_originSet�� _copySet�� �����մϴ�.). 
	fd_set _originSet;

	// _originSet�� �����Ͽ� select�� ���޵˴ϴ�.
	fd_set _copySet;

	// ���� ���� ���� ���� ����.
	int  _socketCount;
};

