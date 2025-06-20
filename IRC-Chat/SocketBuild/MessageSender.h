#pragma once
#pragma execution_character_set("utf-8")

/**
 * @file MessageSender.h
 * @brief 다양한 방식으로 클라이언트들에게 메시지를 전송하는 MessageSender 클래스를 선언합니다.
 * @author 최성락
 * @date 2025-06-17
 * 
 * @details
 * 모든 클라이언트에게 메시지를 보내는 브로드캐스트, 
 * <br>한 클라이언트를 제외한 모두에게 보내는 멀티캐스트,
 * <br>특정 클라이언트에게만 보내는 유니캐스트 기능을 제공합니다.
 * <br>메시지 포맷팅과 전송 결과 추적을 위한 유틸리티들을 포함합니다.
 */

#include <WinSock2.h>
#include <string>

/**
 * @class MessageSender
 * @brief 서버에서 클라이언트 소켓으로 메시지를 보내는 기능을 제공하는 클래스입니다.
 *
 * @details
 * 이 클래스는 서버 메시지의 다양한 전달 모드를 구현합니다.
 * - Broadcast : 모든 연결된 클라이언트에게 메시지 전송.
 * - Multicast : 특정 클라이언트를 제외한 다수에게 메시지 전송.
 * - Unicast : 한 클라이언트에게만 메세지 전송.
 * 
 * 내부적으로 개행 문자(NEW_LINE 상수)를 메시지 끝에 추가하여 포맷팅합니다.
 * <br>전송 작업 결과를 나타내는 Result 열거형을 사용합니다.
 */
class MessageSender
{
	public:
		/// 메시지 끝에 붙일 개행 구분자 (소스 파일에서 정의됨).
		static const char* NEW_LINE;

	public:
		/**
		 * @enum MessageSender::Result
		 * @brief 메시지 전송 작업의 결과 상태 값.
		 */
		enum class Result
		{
			
			SUCCESS,		///< 모든 대상에게 메시지를 성공적으로 전송함.
			PARTIAL_FAIL,	///< 메시지가 일부 대상에게만 전송되고 일부 대상에게는 전송 실패함.
			TOTAL_FAIL,		///< 메시지를 어떤 대상에게도 보내지 못함.
		};

	public:
		/**
		 * @fn MessageSender::MessageSender()
		 * @brief MessageSender 생성자.
		 * @return 없음.
		 * @note 이 생성자에서는 특별한 초기화가 일어나지 않습니다.
		 */
		MessageSender();

		/**
		 * @fn MessageSender::~MessageSender()
		 * @brief 소멸자.
		 * @return 없음.
		 * @note 이 소멸자에서는 해제할 동적 자원이 없습니다.
		 */
		~MessageSender();

		// 복사 생성자 및 복사 할당 연산자 삭제.
		MessageSender(const MessageSender& obj) = delete;
		MessageSender& operator=(const MessageSender& obj) = delete;

		// 이동 생성자 및 이동 할당 연산자 삭제.
		MessageSender(MessageSender&& obj) = delete;
		MessageSender& operator=(MessageSender&& obj) = delete;

	public:
		
		/**
		 * @fn MessageSender::Result MessageSender::broadcast(const std::string& message, SOCKET* sockets, int socket_count)
		 * @brief 모든 클라이언트에게 메시지를 전송합니다.
		 * @param[IN] const std::string& message : 보낼 메시지 텍스트.
		 * @param[IN] SOCKET* sockets : 메시지를 보낼 클라이언트 소켓들의 배열.
		 * @param[IN] int socket_count : 배열에 포함된 소켓 개수 (전송할 클라이언트 수).
		 * @return MessageSender::Result : 전송 작업 결과 상태 값 (SUCCESS, PARTIAL_FAIL 또는 TOTAL_FAIL).
		 *
		 * @details
		 * 주어진 메시지를 배열에 있는 모든 클라이언트 소켓에 전송합니다.
		 * <br>하나 이상의 전송에 실패하면 결과 코드가 부분 실패 또는 전체 실패로 표시됩니다.
		 */
		MessageSender::Result broadcast(const std::string& message, SOCKET* sockets, int socket_count);

		/**
		 * @fn MessageSender::Result MessageSender::multicast(const std::string& message, SOCKET* sockets, int socket_count, SOCKET except_socket)
		 * @brief 특정 클라이언트를 제외한 모든 클라이언트에게 메시지를 보냅니다.
		 * @param[IN] const std::string& message : 보낼 메시지 텍스트.
		 * @param[IN] SOCKET* sockets : 메시지를 보낼 클라이언트 소켓들의 배열.
		 * @param[IN] int socket_count : 배열에 포함된 소켓 개수 (전송할 클라이언트 수).
		 * @param[IN] SOCKET except_socket : 메시지를 보내지 않을 클라이언트의 소켓.
		 * @return MessageSender::Result : 전송 작업 결과 상태 값 (SUCCESS, PARTIAL_FAIL 또는 TOTAL_FAIL).
		 *
		 * @details
		 * 소켓 리스트에서 `except_socket`으로 지정된 소켓을 제외한 모든 소켓에 메시지를 전송합니다.
		 * <br>한 클라이언트를 제외한 다른 클라이언트에게 메시지를 전달할 때 사용합니다. 
		 */
		MessageSender::Result multicast(const std::string& message, SOCKET* sockets, int socket_count, SOCKET except_socket);
		
		/**
		 * @fn bool MessageSender::unicast(const std::string& message, SOCKET target_socket)
		 * @brief 하나의 클라이언트 소켓에만 메시지를 전송합니다.
		 * @param[IN] const std::string& message : 보낼 메시지 텍스트.
		 * @param[IN] SOCKET target_socket : 메시지를 보낼 대상 클라이언트의 소켓.
		 * @return : bool 메시지 전송에 성공하면 true, 실패하면 false.
		 */
		bool unicast(const std::string& message, SOCKET target_socket);

	private:
		/**
		 * @fn std::string MessageSender::formatMessage(const std::string& message) const
		 * @brief 주어진 메시지 끝에 개행 문자를 추가한 문자열을 반환합니다.
		 * @param[IN] const std::string& message : 원본 메시지 텍스트.
		 * @return std::string : 개행 문자가 추가된 메시지 문자열.
		 * @note NEW_LINE 상수를 주어진 메시지 끝에 추가한 문자열을 반환합니다.
		 */
		std::string formatMessage(const std::string& message) const;

		/**
		 * @fn bool MessageSender::sendMessage(const std::string& formatted_message, SOCKET target_socket)
		 * @brief 이미 포맷된 메시지를 특정 클라이언트 소켓으로 전송합니다.
		 * @param[IN] const std::string& formatted_mssage : 개행 문자까지 포함된 메시지 문자열.
		 * @param[IN] SOCKET target_socket : 메시지를 보낼 대상 클라이언트 소켓.
		 * @return bool : 메시지 전송에 성공하면 true, 실패하면 false.
		 * 
		 * @note broadcast, multicast, unicast 함수 내부에서 실제 전송을 담당하는 핵심 구현 함수입니다.
		 */
		bool sendMessage(const std::string& formatted_mssage, SOCKET target_socket);
};