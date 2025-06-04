#include "SocketIniter.h"
#include "MemoryLeakHelper.h"
#include "DebugHelper.h"
#include "sstream"
#include "minwindef.h"

SocketIniter::SocketIniter()
{
    // _wsaData�� �ʱ�ȭ�մϴ�.
    this->_wsaData = {};
}

SocketIniter::~SocketIniter()
{
    // Winsock�� �����մϴ�.
    WSACleanup();
    LOG_INFO("Winsock�� �����մϴ�.");
}

bool SocketIniter::init()
{
    LOG_INFO("Winsock �ʱ�ȭ�� �����մϴ�.");
    // ����� ����� �Լ�.
    int iResult;

    // Winsock �ʱ�ȭ�մϴ�.
    iResult = WSAStartup(MAKEWORD(2, 2), &this->_wsaData);

    // �ʱ�ȭ ���� Ȯ���մϴ�.
    if (iResult != 0)
    {
        LOG_ERROR("�ʱ�ȭ ����");
        return (false);
    }
    else
    {
        // �ʱ�ȭ ���� �� DEBUG��� �ϰ�� _wsaData�� ����մϴ�.
        LOG_DEBUG(wsaDataToString());
    }
}

std::string SocketIniter::wsaDataToString() const
{
    // ostringsteam�� ���ؼ� _wsaData �����͸� �����Ͽ� �����մϴ�.
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
