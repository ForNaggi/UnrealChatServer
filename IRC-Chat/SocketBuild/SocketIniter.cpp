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

bool SocketIniter::init()
{
    LOG_INFO("Winsock 초기화를 시작합니다.");
    // 결과를 기록할 함수.
    int iResult;

    // Winsock 초기화합니다.
    iResult = WSAStartup(MAKEWORD(2, 2), &this->_wsaData);

    // 초기화 성공 확인합니다.
    if (iResult != 0)
    {
        LOG_ERROR("초기화 실패");
        return (false);
    }
    else
    {
        // 초기화 성공 시 DEBUG모드 일경우 _wsaData를 출력합니다.
        LOG_DEBUG(wsaDataToString());
    }
}

std::string SocketIniter::wsaDataToString() const
{
    // ostringsteam을 통해서 _wsaData 데이터를 정리하여 저장합니다.
    std::ostringstream oss;

    oss << "WSAData{";
    oss << "ver=" << LOBYTE(this->_wsaData.wVersion) << "." << HIBYTE(this->_wsaData.wVersion);
    oss << ", highVer=" << LOBYTE(this->_wsaData.wHighVersion) << "." << HIBYTE(this->_wsaData.wHighVersion);

#ifdef _WIN64
    oss << ", maxSockets=" << this->_wsaData.iMaxSockets;
    oss << ", maxUdpDg=" << this->_wsaData.iMaxUdpDg;
#endif

    oss << ", desc=\"" << this->_wsaData.szDescription << "\"";
    oss << ", status=\"" << this->_wsaData.szSystemStatus << "\"";
    oss << "}";
    
    return oss.str();
}
