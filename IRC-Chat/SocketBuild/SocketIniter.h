#pragma once
#pragma execution_character_set("utf-8")

/**
 * @file SocketIniter.h
 * @brief WinSock(Windows Sockets API)을 초기화하기 위한 SocketIniter 클래스를 정의합니다.
 * @author 최성락
 * @date 2025-06-17
 * 
 * @details
 * 이 클래스는 애플리케이션의 WinSock 시작과 종료를 담당합니다.
 * <br>SocketIniter 인스턴스를 생성하면 WSAStartup이 호출되어 네트워크 하부 시스템을 준비합니다.
 * <br>인스턴스가 소멸될 때 WSACleanup이 호출되어 정리를 수행합니다. 
 * <br>WinSock의 세부 정보를 문자열로 만들어 로그 등에 활용할 수 있습니다.
 */

#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

/**
 * @class SocketIniter
 * @brief 애플리케이션의 WinSock 초기화와 종료를 수행합니다.
 *
 * @details
 * 프로그램 시작 시 이 클래스를 사용하여 WSAStartup을 호출하고 필요한 소켓 DLL들을 로드합니다.
 * <br>객체가 소멸될 때 WSACleanup을 호출하여 WinSock을 종료합니다. 
 * <br>명시적으로 초기화를 수행하는 메서드와 디버깅을 위한 WinSock 정보 출력 메서드를 제공합니다.
 */
class SocketIniter
{
	public:
	
		/**
		 * @enum SocketIniter::Result
		 * @brief WinSock 초기화 결과 상태 값을 나타냅니다.
		 */
		enum class Result
		{
			FAIL_SOCKET,	///< WinSock 초기화 실패.
			SUCCESS_SOCKET	///< WinSock 초기화 성공.
		};
	
	public:
		/**
		 * @fn SocketIniter::SocketIniter()
		 * @brief SocketIniter 객체를 생성합니다 (이 시점에서는 WSAStartup을 호출하지 않습니다).
		 * @return 없음.
		 *
		 * @details
		 * 내부 WSADATA 구조체를 초기화합니다. 
		 * <br>생성 후 실제 WinSock 초기화를 위해 init()를 호출해야 합니다.
		 */
		SocketIniter();

		/**
		 * @fn SocketIniter::~SocketIniter()
		 * @brief 소멸자입니다. WSACleanup을 호출하여 WinSock을 정리합니다.
		 * @return 없음.
		 * 
		 * @note 
		 * 소멸자가 실행되기 전에 모든 소켓을 닫아야 합니다. 
		 * <br>WSACleanup은 WSAStartup에서 할당한 자원을 해제합니다.
		 */
		~SocketIniter();
	
		/**
		 * @fn SocketIniter::Result SocketIniter::init()
		 * @brief WSAStartup을 호출하여 WinSock을 초기화하고 결과를 로그로 남깁니다.
		 * @return SocketIniter::Result : 초기화에 성공하면 SUCCESS_SOCKET, 실패하면 FAIL_SOCKET을 반환합니다.
		 *
		 * @note
		 * 이 메서드는 프로그램 시작 시 한 번만 호출해야 합니다. 
		 * <br>WinSock 버전 2.2를 시작하도록 시도합니다.
		 * <br>성공하면 WinSock 구현 세부 정보를 얻기 위해 wsaDataToString()을 호출할 수 있습니다.
		 */
		SocketIniter::Result init();
		
		/**
		 * @fn std::string SocketIniter::wsaDataToString() const
		 * @brief WinSock 구현 버전에 대한 정보를 담은 문자열을 반환합니다.
		 * @return std::string : WSADATA 구조체의 세부 정보(예: 버전, 설명)를 담은 문자열입니다.
		 */
		std::string wsaDataToString() const;
	
	private:
		/// winsock 대한 정보를 담고 있습니다 (WSAStartup에 의해 설정됨).
		WSADATA _wsaData;
};