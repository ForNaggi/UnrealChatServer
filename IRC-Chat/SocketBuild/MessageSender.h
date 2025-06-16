#pragma once

#include <WinSock2.h>
#include <string>

/*
** 클래스명	:	MessageSender.
** 설명		:	메세지 전송을 담당하는 클래스.
*				Broadcast, Multicast, unicast 방식의 전송.
** 초기화	:	기본 생성자만 사용.
*/
class MessageSender
{
	public:
		static const char* NEW_LINE;
	public:
	
		// 해당 클래스의 메소드의 결과를 나타내는 enum class입니다.
		enum class Result
		{
			SUCCESS,		// 대상으로 한 모든 대상에게 전송 성공.
			PARTIAL_FAIL,	// 일부 대상에게만 전송 성공.
			TOTAL_FAIL,		// 모든 대상에게 전송 실패.
		};

	public:

		/*
		** 함수명	:	MessageSender.
		** 설명		:	MessageSender 객체 생성자.
		** 인자		:	없음.
		** 반환값	:	없음.
		*/
		MessageSender();

		/*
		** 함수명	:	~MessageSender.
		** 설명		:	MessageSender 객체 소멸자.
		** 인자		:	없음.
		** 반환값	:	없음.
		*/
		~MessageSender();

		// 복사 생성자 및 복사 할당 연산자 삭제.
		MessageSender(const MessageSender& obj) = delete;
		MessageSender& operator=(const MessageSender& obj) = delete;

		// 이동 생성자 및 이동 할당 연산자 삭제.
		MessageSender(MessageSender&& obj) = delete;
		MessageSender& operator=(MessageSender&& obj) = delete;

	public:

		/*
		** 함수명	:	broadcast.
		** 설명		:	모든 클라이언트에게 메세지를 브로드캐스트합니다.
		** 인자		:	const std::string& message : 전송할 메세지.
		*				SOCKET* sockets : 메세지를 전송할 클라이언트 소켓 배열.
		*				int socket_count : 소켓 개수.
		** 반환값	:	MessageSender::Result : 전송 결과.
		*/
		MessageSender::Result broadcast(const std::string& message, SOCKET* sockets, int socket_count);

		/*
		** 함수명	:	multicast.
		** 설명		:	except_socket을 제외한 나머지 클라이언트 소켓(들)에게 멀티캐스트합니다.
		** 인자		:	const std::string& message : 전송할 메세지.
		*				SOCKET* sockets : 메세지를 전송할 클라이언트 소켓 배열.
		*				int socket_count : 소켓 개수.
		*				SOCKET except_socket : 메세지 전송을 제외할 소켓.
		** 반환값	:	MessageSender::Result : 전송 결과.
		*/
		MessageSender::Result multicast(const std::string& message, SOCKET* sockets, int socket_count, SOCKET except_socket);
	
		/*
		** 함수명	:	unicast.
		** 설명		:	특정 소켓에게만 메세지를 전송합니다.
		** 인자		:	const std::string& message : 전송할 메세지.
		*				SOCKET target_socket : 메세지를 전송할 소켓.
		** 반환값	:	MessageSender::Result : bool : 성공하면 true, 실패하면 false.
		*/
		bool unicast(const std::string& message, SOCKET target_socket);

	private:

		/*
		** 함수명	:	formatMessage.
		** 설명		:	전송할 메세지에 개행 문자를 추가합니다.
		** 인자		:	const std::string& message : 전송할 메세지.
		** 반환값	:	std::string : 포멧팅된 메세지.
		*/
		std::string formatMessage(const std::string& message) const;

		/*
		** 함수명	:	sendMessage.
		** 설명		:	실제 타겟 클라이언트 소켓에게 메세지를 전송합니다.
		** 인자		:	const std::string& formatted_mssage : 전송할 포맷된 메세지.
		*				SOCKET target_socket : 메세지를 전송할 클라이언트 소켓.
		** 반환값	:	bool : 성공하면 true, 실패하면 false.
		*/
		bool sendMessage(const std::string& formatted_mssage, SOCKET target_socket);
};