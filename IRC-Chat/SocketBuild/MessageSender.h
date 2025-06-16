#pragma once

#include <WinSock2.h>
#include <string>

/*
** Ŭ������	:	MessageSender.
** ����		:	�޼��� ������ ����ϴ� Ŭ����.
*				Broadcast, Multicast, unicast ����� ����.
** �ʱ�ȭ	:	�⺻ �����ڸ� ���.
*/
class MessageSender
{
	public:
		static const char* NEW_LINE;
	public:
	
		// �ش� Ŭ������ �޼ҵ��� ����� ��Ÿ���� enum class�Դϴ�.
		enum class Result
		{
			SUCCESS,		// ������� �� ��� ��󿡰� ���� ����.
			PARTIAL_FAIL,	// �Ϻ� ��󿡰Ը� ���� ����.
			TOTAL_FAIL,		// ��� ��󿡰� ���� ����.
		};

	public:

		/*
		** �Լ���	:	MessageSender.
		** ����		:	MessageSender ��ü ������.
		** ����		:	����.
		** ��ȯ��	:	����.
		*/
		MessageSender();

		/*
		** �Լ���	:	~MessageSender.
		** ����		:	MessageSender ��ü �Ҹ���.
		** ����		:	����.
		** ��ȯ��	:	����.
		*/
		~MessageSender();

		// ���� ������ �� ���� �Ҵ� ������ ����.
		MessageSender(const MessageSender& obj) = delete;
		MessageSender& operator=(const MessageSender& obj) = delete;

		// �̵� ������ �� �̵� �Ҵ� ������ ����.
		MessageSender(MessageSender&& obj) = delete;
		MessageSender& operator=(MessageSender&& obj) = delete;

	public:

		/*
		** �Լ���	:	broadcast.
		** ����		:	��� Ŭ���̾�Ʈ���� �޼����� ��ε�ĳ��Ʈ�մϴ�.
		** ����		:	const std::string& message : ������ �޼���.
		*				SOCKET* sockets : �޼����� ������ Ŭ���̾�Ʈ ���� �迭.
		*				int socket_count : ���� ����.
		** ��ȯ��	:	MessageSender::Result : ���� ���.
		*/
		MessageSender::Result broadcast(const std::string& message, SOCKET* sockets, int socket_count);

		/*
		** �Լ���	:	multicast.
		** ����		:	except_socket�� ������ ������ Ŭ���̾�Ʈ ����(��)���� ��Ƽĳ��Ʈ�մϴ�.
		** ����		:	const std::string& message : ������ �޼���.
		*				SOCKET* sockets : �޼����� ������ Ŭ���̾�Ʈ ���� �迭.
		*				int socket_count : ���� ����.
		*				SOCKET except_socket : �޼��� ������ ������ ����.
		** ��ȯ��	:	MessageSender::Result : ���� ���.
		*/
		MessageSender::Result multicast(const std::string& message, SOCKET* sockets, int socket_count, SOCKET except_socket);
	
		/*
		** �Լ���	:	unicast.
		** ����		:	Ư�� ���Ͽ��Ը� �޼����� �����մϴ�.
		** ����		:	const std::string& message : ������ �޼���.
		*				SOCKET target_socket : �޼����� ������ ����.
		** ��ȯ��	:	MessageSender::Result : bool : �����ϸ� true, �����ϸ� false.
		*/
		bool unicast(const std::string& message, SOCKET target_socket);

	private:

		/*
		** �Լ���	:	formatMessage.
		** ����		:	������ �޼����� ���� ���ڸ� �߰��մϴ�.
		** ����		:	const std::string& message : ������ �޼���.
		** ��ȯ��	:	std::string : �����õ� �޼���.
		*/
		std::string formatMessage(const std::string& message) const;

		/*
		** �Լ���	:	sendMessage.
		** ����		:	���� Ÿ�� Ŭ���̾�Ʈ ���Ͽ��� �޼����� �����մϴ�.
		** ����		:	const std::string& formatted_mssage : ������ ���˵� �޼���.
		*				SOCKET target_socket : �޼����� ������ Ŭ���̾�Ʈ ����.
		** ��ȯ��	:	bool : �����ϸ� true, �����ϸ� false.
		*/
		bool sendMessage(const std::string& formatted_mssage, SOCKET target_socket);
};