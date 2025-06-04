#pragma once

#include <iostream> 
#include <string>
#include <ctime>

enum class LogLevel
{
    INFO,       // �Ϲ� ����.
    WARNNING,   // ���.  
    ERRORZ,      // ����.
    DEBUG       // ����� ����.
};

/*
** �Լ���	:	CurrentTime.
** ����		:	���� �ð��� "2025-06-04 00:00:00" ���� ���·� ��ȯ�� �ݴϴ�.
** ����		:	����.
** ��ȯ��	:	string.
*/
inline std::string current_time()
{
    time_t now = time(nullptr); // ���� �ð��� �� ������ ������.
    struct tm timeInfo;
    localtime_s(&timeInfo , &now);
    static char buf[64];        // �ð� ���� ������ ���� �迭 - static ��� ���Ҵ� ����.
    // �ش� �Լ��� (����, ���ۻ�����, �������, �ð�����)�� �ް� buf�� ������Ĵ�� �����մϴ�.
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeInfo);
    return std::string(buf);
}

/*
** �Լ���	:	LogLevelToString.
** ����		:	���ڷ� �Ѿ�� �α׿� �ش��ϴ� ���ڿ��� �����մϴ�.
*               �ش� ���ڿ��� �α��� ��̰� �˴ϴ�.
** ����		:	LogLevel <- ����ü Ŭ����.
** ��ȯ��	:	const char* <- �α׿� �ش��ϴ� ���ڿ�.
*/
inline const char* log_level_to_string(LogLevel level)
{
    switch (level) // �α׿� �ش��ϴ� ������ switch�մϴ�.
    {
    case LogLevel::INFO:  return " INFO ";
    case LogLevel::WARNNING:  return " WARNNING ";
    case LogLevel::ERRORZ: return " ERROR ";
    case LogLevel::DEBUG: return " DEBUG ";
    default: return " UNKNOWN ";
    }
}

/*
** �Լ���	:	Log.
** ����		:	���ڿ� ���� ��������� �����ϰ� �޼����� ����մϴ�.
*               Error�� Debug���� ���� ���ϰ� �ڵ� ���ε� ���� ����մϴ�.
** ����		:	level <- ����ü Ŭ����. �ش� �޼����� ���� ����.
*               message <- �α� �޼���.
*               file <- ���� �ش� �޼����� ������ �� ����.
*               Line <- �ش� �ڵ� ����.
** ��ȯ��	:	����.
*/
inline void log(
    LogLevel level,
    const std::string& message,
    const char* file = "",
    int line = 0
)
{
    // DEBUG ��尡 �ƴϸ� �ش� ����� ���ǵ� ���� ���� DEBUG�� ��� ���ϵ˴ϴ�.
#ifndef _DEBUG
    if (level == LogLevel::DEBUG)
    {
        return;
    }
#endif

    // �α��� ��������� �����մϴ�.
    std::ostream& out = (level == LogLevel::ERRORZ) ? std::cerr : std::cout;

    // �α� ���� ���ڿ��� �����ɴϴ�.
    const char* levelStr = log_level_to_string(level);

    // �ƿ�ǲ�� �α� Ÿ�԰� �ð��� �߰��մϴ�.
    out << "[" << levelStr << "] " << current_time();

    // ���ϰ� �ڵ���� ������ ERROR�� DEBUG���� �߰��մϴ�.
    if (level == LogLevel::ERRORZ || level == LogLevel::DEBUG)
    {
        out << " (" << file << ":" << line << ")";
    }

    // �ƿ�ǲ�� �޼����� �߰��մϴ�.(flush)
    out << " - " << message << std::endl;
}

// ��ũ�η� ����

// �Ϲ� �α�
#define LOG_INFO(message) log(LogLevel::INFO,  message)
// ��� �α�
#define LOG_WARN(message) log(LogLevel::WARNNING,  message)
// ���� �α�
#define LOG_ERROR(message) log(LogLevel::ERRORZ, message, __FILE__, __LINE__)
// ����� �α�
#define LOG_DEBUG(message) log(LogLevel::DEBUG, message, __FILE__, __LINE__)
