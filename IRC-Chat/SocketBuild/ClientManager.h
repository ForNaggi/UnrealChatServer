#pragma once

#include <WinSock2.h>
#include <array>
#include <string>
#include <queue>

/*
** 클래스명	:	ClientManager.
** 설명		:	클라이언트 소켓 관리를 담당하는 클래스입니다.
*               클라이언트 소켓 추가, 제거, 조회 기능.
*				게임과 채팅에서 사용할 닉네임을 생성합니다.
** 초기화	:	기본 생성자만 사용.
*/
class ClientManager
{
	public:
		
		// 최대 클라이언트 수.
		static const int MAX_CLIENTS = 10;

	public:

		/*
		** 함수명	:	ClientManager.
		** 설명		:	ClientManager 객체 생성자.
		*				_clientSockets, _availableList. _connectedSocketCount를 초기화합니다.
		** 인자		:	없음.
		** 반환값	:	없음.
		*/
		ClientManager();

		/*
		** 함수명	:	~ClientManager.
		** 설명		:	ClientManager 객체 소멸자.
		*				_clientSockets의 소켓들을 close해줍니다.
		** 인자		:	없음.
		** 반환값	:	없음.
		*/
		~ClientManager();

		// 복사 생성자 및 복사 할당 연산자 삭제.
		ClientManager(const ClientManager& obj) = delete;
		ClientManager& operator=(const ClientManager& obj) = delete;

		// 이동 생성자 및 이동 할당 연산자 삭제.
		ClientManager(ClientManager&& obj) = delete;
		ClientManager& operator=(ClientManager&& obj) = delete;

	public:

		/*
		** 함수명	:	addClient.
		** 설명		:	새로운 클라이언트 소켓을 추가합니다.
		*				클라이언트의 닉네임을 할당합니다. (Player_{NUMBER} 형식).
		** 인자		:	SOCKET client_socket : 추가할 클라이언트 소켓.
		** 반환값	:	int : 할당된 인덱스, -1이면 실패.
		*/
		int addClient(SOCKET client_socket);

		/*
		** 함수명	:	removeClient.
		** 설명		:	인자로 넘어온 인덱스의 클라이언트 소켓을 제거합니다.
		** 인자		:	int client_index : 제거할 클라이언트의 인덱스.
		** 반환값	:	bool : 성공하면 true, 실패하면 false.
		*/
		bool removeClient(int client_index);

		/*
		** 함수명	:	getClientSocket.
		** 설명		:	인자로 넘어온 인덱스에 해당하는 클라이언트 소켓을 리턴합니다.
		** 인자		:	int client_index : 클라이언트 인덱스.
		** 반환값	:	SOCKET : 인덱스에 해당하는 클라이언트 소켓.
		*/
		SOCKET getClientSocket(int client_index) const;

		/*
		** 함수명	:	getConnectedClientCount.
		** 설명		:	현재 연결된 클라이언트 수를 반환합니다.
		** 인자		:	없음.
		** 반환값	:	int : 연결된 클라이언트 수.
		*/
		int getConnectedClientCount() const;

		/*
		** 함수명	:	isValidIndex.
		** 설명		:	인덱스가 유효한지 확인합니다.
		** 인자		:	int client_index : 확인할 인덱스.
		** 반환값	:	bool : 유효하면 true, 아니면 false.
		*/
		bool isValidIndex(int client_index) const;

		/*
		** 함수명	:	getAllSockets.
		** 설명		:	모든 유효한 클라이언트 소켓을 배열로 반환합니다.
		** 인자		:	SOCKET* sockets : 클라이언트 소켓을 저장할 배열.
		*				int max_count : 배열의 최대 크기.
		** 반환값	:	int : 복사된 소켓 수.
		*/
		int getAllSockets(SOCKET* sockets, int max_count) const;

		/*
		** 함수명	:	getClientNickname.
		** 설명		:	클라이언트의 생성된 닉네임을 반환합니다.
		** 인자		:	int client_index : 클라이언트 인덱스.
		** 반환값	:	std::string : 클라이언트 닉네임.
		*/
		std::string getClientNickname(int client_index) const;

	private:
		// 클라이언트 소켓 배열.
		std::array<SOCKET, MAX_CLIENTS> _clientSockets;

		// 재사용 가능한 인덱스를 관리하는 우선순위 큐.
		std::priority_queue<int, std::vector<int>, std::greater<int>> _availableList;

		// 현재 연결된 클라이언트 소켓 수.
		int _connectedSocketCount;

	private:

		/*
		** 함수명	:	initalizeClientSockets.
		** 설명		:	클라이언트 소켓 배열을 초기화합니다.
		** 인자		:	없음.
		** 반환값	:	엾음.
		*/
		void initalizeClientSockets();

		/*
		** 함수명	:	initalizeAvailableList.
		** 설명		:	사용 가능한 인덱스 큐를 초기화합니다.
		** 인자		:	없음.
		** 반환값	:	엾음.
		*/
		void initalizeAvailableList();

		/*
		** 함수명	:	getNextIndex.
		** 설명		:	클라이언트 소켓 배열에서 다음으로 사용 가능한 인덱스를 반환합니다.
		** 인자		:	없음.
		** 반환값	:	int : 사용 가능한 인덱스, 없으면 -1.
		*/
		int getNextIndex();
};