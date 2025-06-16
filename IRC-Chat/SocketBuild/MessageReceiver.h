#pragma once

#include <winsock2.h>
#include <string>

/*
** Ŭ������	:	MessageRecevier.
** ����		:	Ŭ���̾�Ʈ�κ��� �޼��� ������ ����ϴ� Ŭ����.
*               �޽��� ����, �Ľ�, ��ɾ� Ȯ�� �� ���� ���� ó�� ���.
** �ʱ�ȭ	:	SOCKET�� �Ű������� �޴� �����ڸ� ���.
*/
class MessageRecevier
{
    public:

        // �ش� Ŭ������ �޼ҵ��� ����� ��Ÿ���� enum class�Դϴ�.
        enum class Result
        {
            SUCCESS,
            FAIL_RECEIVE,
            FAIL_SEND,
            CLIENT_QUIT,
            CLIENT_DISCONNECTED
        };

        /*
        ** �޼ҵ��	:	MessageRecevier.
        ** ����		:	MessageRecevier ��ü ������.
        ** ���ڰ�	:	SOCKET client_socket : Ŭ���̾�Ʈ ����.
        ** ��°�	:	����.
        */
        explicit MessageRecevier(SOCKET client_socket);

        /*
        ** �޼ҵ��	:	~MessageRecevier.
        ** ����		:	MessageRecevier ��ü �Ҹ���.
        ** ���ڰ�	:	����.
        ** ��°�	:	����.
        */
        ~MessageRecevier();

        // ���� ������ �� ���� �Ҵ� ������ ����.
        MessageRecevier(const MessageRecevier& obj) = delete;
        MessageRecevier& operator=(const MessageRecevier& obj) = delete;

        // �̵� ������ �� �̵� �Ҵ� ������ ����.
        MessageRecevier(MessageRecevier&& obj) = delete;
        MessageRecevier& operator=(MessageRecevier&& obj) = delete;

    public:

        /*
        ** �޼ҵ��	:	receiveMessage.
        ** ����		:	Ŭ���̾�Ʈ�κ��� �޼����� �����մϴ�.
        ** ���ڰ�	:	����.
        ** ��°�	:	MessageRecevier::Result : ����/���� ���.
        */
        MessageRecevier::Result receiveMessage();

        /*
        ** �޼ҵ��	:	getLastMessage.
        ** ����		:	���������� ���� �޽����� ��ȯ�մϴ�.
        ** ���ڰ�	:	����.
        ** ��°�	:	const std::string& : ������ �޽���.
        */
        const std::string& getLastMessage() const;

        /*
        ** �޼ҵ��	:	isQuitCommand.
        ** ����		:	�޽����� "quit"���� Ȯ���մϴ�.
        ** ���ڰ�	:	const std::string& message : "quit"���� Ȯ���� �޽���.
        ** ��°�	:	bool : "quit"�̸� true, �ƴϸ� false.
        */
        bool isQuitCommand(const std::string& message);

    private:
        // Ư�� Ŭ���̾�Ʈ�� ����ϴ� ����.
        SOCKET _clientSocket;

        // Ŭ���̾�Ʈ���� ���� �޼����� �����ϴ� �������.
        std::string _lastMessage;

        // ��� ���� ������.(recv�� ����Ʈ�� ������ �迭�� ũ��).
        static const int BUFFER_SIZE = 1024;

        /*
        ** �޼ҵ��	:	cleanMessage.
        ** ����		:	�޽������� ĳ���� ������ �����մϴ�.
        ** ���ڰ�	:	std::string& message : ������ �޽���.
        ** ��°�	:	����.
        */
        void cleanMessage(std::string& message);
};