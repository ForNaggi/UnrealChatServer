#pragma once

#include <WinSock2.h>
#include <array>
#include <string>
#include <queue>

/*
** Ŭ������	:	ClientManager.
** ����		:	Ŭ���̾�Ʈ ���� ������ ����ϴ� Ŭ�����Դϴ�.
*               Ŭ���̾�Ʈ ���� �߰�, ����, ��ȸ ���.
*				���Ӱ� ä�ÿ��� ����� �г����� �����մϴ�.
** �ʱ�ȭ	:	�⺻ �����ڸ� ���.
*/
class ClientManager
{
	public:
		
		// �ִ� Ŭ���̾�Ʈ ��.
		static const int MAX_CLIENTS = 10;

	public:

		/*
		** �Լ���	:	ClientManager.
		** ����		:	ClientManager ��ü ������.
		*				_clientSockets, _availableList. _connectedSocketCount�� �ʱ�ȭ�մϴ�.
		** ����		:	����.
		** ��ȯ��	:	����.
		*/
		ClientManager();

		/*
		** �Լ���	:	~ClientManager.
		** ����		:	ClientManager ��ü �Ҹ���.
		*				_clientSockets�� ���ϵ��� close���ݴϴ�.
		** ����		:	����.
		** ��ȯ��	:	����.
		*/
		~ClientManager();

		// ���� ������ �� ���� �Ҵ� ������ ����.
		ClientManager(const ClientManager& obj) = delete;
		ClientManager& operator=(const ClientManager& obj) = delete;

		// �̵� ������ �� �̵� �Ҵ� ������ ����.
		ClientManager(ClientManager&& obj) = delete;
		ClientManager& operator=(ClientManager&& obj) = delete;

	public:

		/*
		** �Լ���	:	addClient.
		** ����		:	���ο� Ŭ���̾�Ʈ ������ �߰��մϴ�.
		*				Ŭ���̾�Ʈ�� �г����� �Ҵ��մϴ�. (Player_{NUMBER} ����).
		** ����		:	SOCKET client_socket : �߰��� Ŭ���̾�Ʈ ����.
		** ��ȯ��	:	int : �Ҵ�� �ε���, -1�̸� ����.
		*/
		int addClient(SOCKET client_socket);

		/*
		** �Լ���	:	removeClient.
		** ����		:	���ڷ� �Ѿ�� �ε����� Ŭ���̾�Ʈ ������ �����մϴ�.
		** ����		:	int client_index : ������ Ŭ���̾�Ʈ�� �ε���.
		** ��ȯ��	:	bool : �����ϸ� true, �����ϸ� false.
		*/
		bool removeClient(int client_index);

		/*
		** �Լ���	:	getClientSocket.
		** ����		:	���ڷ� �Ѿ�� �ε����� �ش��ϴ� Ŭ���̾�Ʈ ������ �����մϴ�.
		** ����		:	int client_index : Ŭ���̾�Ʈ �ε���.
		** ��ȯ��	:	SOCKET : �ε����� �ش��ϴ� Ŭ���̾�Ʈ ����.
		*/
		SOCKET getClientSocket(int client_index) const;

		/*
		** �Լ���	:	getConnectedClientCount.
		** ����		:	���� ����� Ŭ���̾�Ʈ ���� ��ȯ�մϴ�.
		** ����		:	����.
		** ��ȯ��	:	int : ����� Ŭ���̾�Ʈ ��.
		*/
		int getConnectedClientCount() const;

		/*
		** �Լ���	:	isValidIndex.
		** ����		:	�ε����� ��ȿ���� Ȯ���մϴ�.
		** ����		:	int client_index : Ȯ���� �ε���.
		** ��ȯ��	:	bool : ��ȿ�ϸ� true, �ƴϸ� false.
		*/
		bool isValidIndex(int client_index) const;

		/*
		** �Լ���	:	getAllSockets.
		** ����		:	��� ��ȿ�� Ŭ���̾�Ʈ ������ �迭�� ��ȯ�մϴ�.
		** ����		:	SOCKET* sockets : Ŭ���̾�Ʈ ������ ������ �迭.
		*				int max_count : �迭�� �ִ� ũ��.
		** ��ȯ��	:	int : ����� ���� ��.
		*/
		int getAllSockets(SOCKET* sockets, int max_count) const;

		/*
		** �Լ���	:	getClientNickname.
		** ����		:	Ŭ���̾�Ʈ�� ������ �г����� ��ȯ�մϴ�.
		** ����		:	int client_index : Ŭ���̾�Ʈ �ε���.
		** ��ȯ��	:	std::string : Ŭ���̾�Ʈ �г���.
		*/
		std::string getClientNickname(int client_index) const;

	private:
		// Ŭ���̾�Ʈ ���� �迭.
		std::array<SOCKET, MAX_CLIENTS> _clientSockets;

		// ���� ������ �ε����� �����ϴ� �켱���� ť.
		std::priority_queue<int, std::vector<int>, std::greater<int>> _availableList;

		// ���� ����� Ŭ���̾�Ʈ ���� ��.
		int _connectedSocketCount;

	private:

		/*
		** �Լ���	:	initalizeClientSockets.
		** ����		:	Ŭ���̾�Ʈ ���� �迭�� �ʱ�ȭ�մϴ�.
		** ����		:	����.
		** ��ȯ��	:	����.
		*/
		void initalizeClientSockets();

		/*
		** �Լ���	:	initalizeAvailableList.
		** ����		:	��� ������ �ε��� ť�� �ʱ�ȭ�մϴ�.
		** ����		:	����.
		** ��ȯ��	:	����.
		*/
		void initalizeAvailableList();

		/*
		** �Լ���	:	getNextIndex.
		** ����		:	Ŭ���̾�Ʈ ���� �迭���� �������� ��� ������ �ε����� ��ȯ�մϴ�.
		** ����		:	����.
		** ��ȯ��	:	int : ��� ������ �ε���, ������ -1.
		*/
		int getNextIndex();
};