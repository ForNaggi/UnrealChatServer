/**
 * @file ClientManager.h
 * @brief 다중 클라이언트 채팅 서버에서 클라이언트 소켓을 관리하는 ClientManager 클래스를 정의합니다.
 * @author 최성락
 * @date 2025-06-17
 *
 * @details
 * 클라이언트 연결 관리 기능: 새로운 클라이언트 추가, 클라이언트 제거, 소켓 조회,
 * 고유한 닉네임(예: "Player_1") 생성 등을 수행합니다.
 */

#pragma once

#pragma execution_character_set("utf-8")

#include <WinSock2.h>
#include <array>
#include <string>
#include <queue>

/**
 * @class ClientManager
 * @brief 서버에서 고정된 수의 클라이언트 소켓과 별칭을 관리하는 클래스입니다.
 *
 * @details
 * 이 클래스는 클라이언트 연결 추가/제거, 활성 소켓 추적,
 * 각 클라이언트에 고유 별칭 할당을 담당합니다.<br>
 * 최대 MAX_CLIENTS개의 클라이언트를 동시 관리할 수 있습니다.
 */
class ClientManager
{
	public:
		
		/// @brief 서버에서 동시에 관리할 수 있는 최대 클라이언트 수입니다.
		static const int MAX_CLIENTS = 10;

	public:

		/**
		 * @fn ClientManager::ClientManager()
		 * @brief ClientManager를 생성하고 내부 데이터를 초기화합니다.
		 * @note 클라이언트 소켓 배열과 사용 가능한 인덱스 목록을 초기화합니다.<br>
		 *       모든 클라이언트 슬롯은 초기에는 비어 있습니다.
		 */
		ClientManager();

		/**
		 * @fn ClientManager::~ClientManager()
		 * @brief ClientManager의 소멸자입니다. 관리하는 모든 클라이언트 소켓을 닫습니다.
		 * @note 배열에 열린 모든 클라이언트 소켓이 닫힙니다.<br>
		 *       서버를 종료할 때 ClientManager를 소멸시켜 자원을 해제해야 합니다.
		 */
		~ClientManager();

		// 복사 생성자 및 복사 할당 연산자 삭제.
		ClientManager(const ClientManager& obj) = delete;
		ClientManager& operator=(const ClientManager& obj) = delete;

		// 이동 생성자 및 이동 할당 연산자 삭제.
		ClientManager(ClientManager&& obj) = delete;
		ClientManager& operator=(ClientManager&& obj) = delete;

	public:

		/**
		 * @fn int ClientManager::addClient(SOCKET client_socket)
		 * @brief 새로운 클라이언트 소켓을 관리자에 추가하고 별칭을 할당합니다.
		 * @param[IN] SOCKET client_socket : 추가할 클라이언트 소켓 (accept된 연결 소켓).
		 * @return int : 새 클라이언트에 할당된 인덱스(0 ~ MAX_CLIENTS-1), 추가 실패 시 -1.
		 * @note 클라이언트에게 "Player_{N}" 형식의 별칭이 부여됩니다 (N은 인덱스).
		 */
		int addClient(SOCKET client_socket);


		/**
		 * @fn bool ClientManager::removeClient(int client_index)
		 * @brief 지정한 인덱스의 클라이언트 소켓을 제거합니다.
		 * @param[IN] int client_index : 제거할 클라이언트의 인덱스.
		 * @return bool : 제거에 성공하면 true, 인덱스가 잘못되었거나 실패 시 false.
		 */
		bool removeClient(int client_index);

		/**
		 * @fn SOCKET ClientManager::getClientSocket(int client_index) const
		 * @brief 주어진 클라이언트 인덱스에 대한 소켓을 가져옵니다.
		 * @param[IN] int client_index : 소켓을 요청할 클라이언트의 인덱스.
		 * @return SOCKET : 해당 인덱스의 클라이언트 소켓(유효하지 않으면 INVALID_SOCKET).
		 */
		SOCKET getClientSocket(int client_index) const;
		
		/**
		 * @fn int ClientManager::getConnectedClientCount() const
		 * @brief 현재 연결된(관리 중인) 클라이언트의 수를 반환합니다.
		 * @return int : 연결된 클라이언트 개수.
		 */
		int getConnectedClientCount() const;

		/**
		 * @fn bool ClientManager::isValidIndex(int client_index) const
		 * @brief 클라이언트 인덱스가 유효한 범위 내인지를 확인합니다.
		 * @param[IN] int client_index : 검증할 클라이언트 인덱스.
		 * @return bool : 인덱스가 유효한 범위(0 ~ MAX_CLIENTS - 1)이면 true, 아니면 false.
		 */
		bool isValidIndex(int client_index) const;

		/**
		 * @fn int ClientManager::getAllSockets(SOCKET* sockets, int max_count) const
		 * @brief 모든 활성 클라이언트 소켓을 주어진 배열에 복사합니다.
		 * @param[OUT] SOCKET* sockets : 클라이언트 소켓 핸들을 저장할 배열.
		 * @param[IN] int max_count : 배열이 담을 수 있는 최대 소켓 개수.
		 * @return int : 실제 배열에 복사된 클라이언트 소켓의 개수.
		 * @note 현재 모든 클라이언트 소켓 목록을 얻는 데 사용합니다.
		 */
		int getAllSockets(SOCKET* sockets, int max_count) const;

		/**
		 * @fn std::string ClientManager::getClientNickname(int client_index) const
		 * @brief 주어진 인덱스가 유효하다면 클라이언트 자동 생성 별칭을 반환합니다.
		 * @param[IN] int client_index : 클라이언트의 인덱스.
		 * @return std::string : 해당 클라이언트의 별칭 (예: "Player_5"), 인덱스가 유효하지 않으면 "Player_Unknown".
		 */
		std::string getClientNickname(int client_index) const;

	private:
		
		/// @brief 클라이언트 소켓 배열 (크기 MAX_CLIENTS). 사용되지 않은 슬롯에는 INVALID_SOCKET.
		std::array<SOCKET, MAX_CLIENTS> _clientSockets;

		/// @brief 재사용을 위한 가용 인덱스 목록 (우선순위 큐).
		std::priority_queue<int, std::vector<int>, std::greater<int>> _availableList;

		/// @brief 현재 연결된 클라이언트 소켓 개수.
		int _connectedSocketCount;

	private:
		
		/**
		 * @fn void ClientManager::initalizeClientSockets()
		 * @brief 클라이언트 소켓 배열의 모든 소켓을 INVALID_SOCKET으로 설정 초기화합니다.
		 * @return 없음.
		 */
		void initalizeClientSockets();

		/**
		 * @fn void ClientManager::initalizeAvailableList()
		 * @brief 사용 가능한 클라이언트 인덱스 (0 ~ MAX_CLIENTS-1) 목록을 초기화합니다.
		 * @return 없음.
		 */
		void initalizeAvailableList();

		/**
		 * @fn int ClientManager::getNextIndex()
		 * @brief 새 연결에 사용할 사용 가능한 클라이언트 인덱스를 가져옵니다.
		 * @return int : 사용 가능한 인덱스 (0 ~ MAX_CLIENTS-1) 또는 서버에 빈 슬롯이 없을 경우 -1.
		 */
		int getNextIndex();
};
