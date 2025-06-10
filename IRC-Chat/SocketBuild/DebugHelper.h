#pragma once

#include <iostream> 
#include <string>
#include <ctime>

enum class LogLevel
{
    INFO,       // 일반 정보.
    WARNING,    // 경고.  
    ERRORZ,     // 에러.
    DEBUG       // 디버그 정보.
};

/*
** 함수명	:	CurrentTime.
** 설명		:	현재 시간을 "2025-06-04 00:00:00" 같은 형태로 반환해 줍니다.
** 인자		:	없음.
** 반환값	:	string.
*/
inline std::string current_time()
{
    time_t now = time(nullptr); // 현재 시간을 초 단위로 가져옴.
    struct tm timeInfo;
    localtime_s(&timeInfo , &now);
    static char buf[64];        // 시간 관련 정보를 담을 배열 - static 사용 재할당 안함.
    // 해당 함수는 (버퍼, 버퍼사이즈, 출력형식, 시간정보)를 받고 buf에 출력형식대로 저장합니다.
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeInfo);
    return std::string(buf);
}

/*
** 함수명	:	LogLevelToString.
** 설명		:	인자로 넘어온 로그에 해당하는 문자열을 리턴합니다.
*               해당 문자열은 로그의 어미가 됩니다.
** 인자		:	LogLevel <- 열거체 클래스.
** 반환값	:	const char* <- 로그에 해당하는 문자열.
*/
inline const char* log_level_to_string(LogLevel level)
{
    switch (level) // 로그에 해당하는 에러로 switch합니다.
    {
    case LogLevel::INFO:  return " INFO ";
    case LogLevel::WARNING:  return " WARNING ";
    case LogLevel::ERRORZ: return " ERROR ";
    case LogLevel::DEBUG: return " DEBUG ";
    default: return " UNKNOWN ";
    }
}

/*
** 함수명	:	Log.
** 설명		:	인자에 따라 출력형식을 결정하고 메세지를 출력합니다.
*               Error와 Debug같은 경우는 파일과 코드 라인도 같이 출력합니다.
** 인자		:	level <- 열거체 클래스. 해당 메세지의 종류 판정.
*               message <- 로그 메세지.
*               file <- 현재 해당 메세지가 나오게 한 파일.
*               Line <- 해당 코드 라인.
** 반환값	:	없음.
*/
inline void log(
    LogLevel level,
    const std::string& message,
    const char* file = "",
    int line = 0
)
{
    // DEBUG 모드가 아니면 해당 상수가 정의된 적이 없고 DEBUG일 경우 리턴됩니다.
#ifndef _DEBUG
    if (level == LogLevel::DEBUG)
    {
        return;
    }
#endif

    // 로그의 출력형식을 결정합니다.
    std::ostream& out = (level == LogLevel::ERRORZ) ? std::cerr : std::cout;

    // 로그 레벨 문자열을 가져옵니다.
    const char* levelStr = log_level_to_string(level);

    // 아웃풋에 로그 타입과 시간을 추가합니다.
    out << "[" << levelStr << "] " << current_time();

    // 파일과 코드라인 정보는 ERROR와 DEBUG에만 추가합니다.
    if (level == LogLevel::ERRORZ || level == LogLevel::DEBUG)
    {
        out << " (" << file << ":" << line << ")";
    }

    // 아웃풋에 메세지를 추가합니다.(flush).
    out << " - " << message << std::endl;
}

// 매크로로 정리.

// 일반 로그.
#define LOG_INFO(message) log(LogLevel::INFO,  message)
// 경고 로그.
#define LOG_WARN(message) log(LogLevel::WARNING,  message)
// 에러 로그.
#define LOG_ERROR(message) log(LogLevel::ERRORZ, message, __FILE__, __LINE__)
// 디버그 로그.
#define LOG_DEBUG(message) log(LogLevel::DEBUG, message, __FILE__, __LINE__)
