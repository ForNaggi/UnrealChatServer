#pragma once
#pragma execution_character_set("utf-8")

/**
 * @file DebugHelper.h
 * @brief 로깅 및 디버깅을 위한 유틸리티 함수와 정의들을 제공합니다.
 * @author 최성락
 * @date 2025-06-17
 * 
 * @details
 * 로그 레벨 유형을 정의하고, 현재 시간, 로그 메시지 포맷을 위한 함수를 제공합니다.<br>
 * 로그 레벨 유형을 선택하여 로그를 남길 수 있는 매크로들을 제공합니다.
 */

#include <iostream> 
#include <string>
#include <ctime>

/**
 * @enum LogLevel
 * @brief 로그의 출력 유형을 정의합니다.
 *
 * @details
 * 각 레벨은 로그 메시지의 중요도나 유형을 나타냅니다.<br>
 * INFO는 일반, WARNING은 경고, ERROR는 오류, DEBUG는 디버깅용 메시지.
 */
enum class LogLevel
{
    INFO,       ///< 일반 메세지 레벨.
    WARNING,    ///< 경고 메시지 레벨.
    ERRORZ,     ///< 오류 메시지 레벨.
    DEBUG       ///< 디버그 메시지 레벨.
};

/**
 * @fn std::string current_time()
 * @brief 현재 시스템 시간을 포맷된 문자열로 반환합니다.
 * @return std::string : "YYYY-MM-DD HH:MM:SS" 형식의 현재 시간 문자열.
 * @note 예시 형식: "2025-06-04 13:45:30". 로그 타임스탬프를 남길 때 유용합니다.
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


/**
 * @fn const char* log_level_to_string(LogLevel level)
 * @brief LogLevel 열거값을 해당 문자열로 변환합니다.
 * @param[IN] LogLevel level : LogLevel 값 (예: LogLevel::INFO).
 * @return const char* : 로그 레벨에 대응하는 문자열 (예: "INFO", "ERROR" 등).
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

/**
 * @fn void log(LogLevel level, const std::string& message, const char* file = "", int line = 0)
 * @brief 주어진 로그 레벨, 메시지를 로그 유형에 따라 파일명, 라인위치 정보와 함께 로그로 출력합니다.
 * @param[IN] LogLevel level : 로그 레벨 (INFO, WARNING, ERRORZ, DEBUG).
 * @param[IN] const std::string& message : 로그로 출력할 메시지 텍스트.
 * @param[IN(default : "")] const char* file : 로그 발생 소스 파일 이름 (ERRORZ/DEBUG 레벨에서 사용).
 * @param[IN(default : 0)] int line : 로그 발생 소스 코드 줄 번호 (ERRORZ/DEBUG 레벨에서 사용).
 * @return 없음.
 *
 * @details
 * 로그 레벨에 따라 출력 스트림을 결정합니다 (오류는 std::cerr, 그 외는 std::cout).
 * 로그 앞부분에 타임스탬프와 레벨을 붙이고, ERRORZ와 DEBUG 레벨에서는 파일명과 라인 번호를 추가합니다.
 * 디버그 모드가 아닌 빌드에서는 DEBUG 레벨 메시지는 출력되지 않습니다.
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

/**
 * @def LOG_INFO(message)
 * @brief INFO 수준 로그 메시지를 출력하는 매크로.
 * @param[IN] const std::string& message : 로그로 남길 일반 메시지.
 * @see log()
 */
#define LOG_INFO(message) log(LogLevel::INFO,  message)
/**
 * @def LOG_WARN(message)
 * @brief WARNING 수준 로그 메시지를 출력하는 매크로.
 * @param[IN] const std::string& message : 로그로 남길 경고 메시지.
 * @see log()
 */
#define LOG_WARN(message) log(LogLevel::WARNING,  message)
/**
 * @def LOG_ERROR(message)
 * @brief ERRORZ 수준 로그 메시지를 출력하는 매크로.
 * @param[IN] const std::string& message : 로그로 남길 오류 메시지.
 * @see log()
 * @note 이 매크로는 로그 출력에 소스 파일과 줄 번호를 포함합니다.
 */
#define LOG_ERROR(message) log(LogLevel::ERRORZ, message, __FILE__, __LINE__)
/**
 * @def LOG_DEBUG(message)
 * @brief 디버그 수준 로그 메시지를 출력하는 매크로.
 * @param[IN] const std::string& message : 로그로 남길 디버그 메시지.
 * @see log()
 * @note 이 매크로는 로그 출력에 소스 파일과 줄 번호를 포함합니다.
 * <br>디버그 빌드에서만 활성됩니다.
 */
#define LOG_DEBUG(message) log(LogLevel::DEBUG, message, __FILE__, __LINE__)
