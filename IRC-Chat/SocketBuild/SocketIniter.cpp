#pragma execution_character_set("utf-8")

/**
 * @file SocketIniter.cpp
 * @brief SocketIniter.h 구현부.
 * @author 최성락
 * @date 2025-06-17
 */

#include "SocketIniter.h"
#include "MemoryLeakHelper.h"
#include "DebugHelper.h"
#include "sstream"
#include "minwindef.h"

SocketIniter::SocketIniter()
{
    // _wsaData를 초기화합니다.
    this->_wsaData = {};
}

SocketIniter::~SocketIniter()
{
    // Winsock을 해제합니다.
    WSACleanup();
    LOG_INFO("Winsock을 해제합니다.");
}

SocketIniter::Result SocketIniter::init()
{
    LOG_INFO("Winsock 초기화를 시작합니다.");
    // 결과를 기록할 함수.
    int result;

    // Winsock 초기화합니다.
    result = WSAStartup(MAKEWORD(2, 2), &this->_wsaData);

    // 초기화 성공 확인합니다.
    if (result != 0)
    {
        LOG_ERROR("Winsock 초기화 실패, 에러 코드 : " + std::to_string(result));
        return (Result::FAIL_SOCKET);
    }
 
    LOG_INFO("Winsock 초기화 성공");

    // 초기화 성공 시 DEBUG모드 일경우 _wsaData를 출력합니다.
    LOG_DEBUG(wsaDataToString());

    return (Result::SUCCESS_SOCKET);
}

std::string SocketIniter::wsaDataToString() const
{
    // ostringsteam을 통해서 _wsaData 데이터를 정리하여 저장합니다.
    std::ostringstream oss;

    // WORD는 unsigned short.
    oss << "\nWSAData{\n";
    oss << "\tver=" << (int)LOBYTE(this->_wsaData.wVersion) << "." << (int)HIBYTE(this->_wsaData.wVersion);
    oss << ", highVer=" << (int)LOBYTE(this->_wsaData.wHighVersion) << "." << (int)HIBYTE(this->_wsaData.wHighVersion) << '\n';
    oss << "\tmaxSockets=" << this->_wsaData.iMaxSockets;
    oss << ", maxUdpDg=" << this->_wsaData.iMaxUdpDg << '\n';
    oss << "\tdesc=\"" << this->_wsaData.szDescription << "\"";
    oss << ", status=\"" << this->_wsaData.szSystemStatus << "\"";
    oss << "\n}";
    
    return oss.str();
}
