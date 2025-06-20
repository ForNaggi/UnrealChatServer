#pragma once
#pragma execution_character_set("utf-8")

/**
 * @file SelectManager.h
 * @brief 여러 소켓에 대한 `select()` 호출을 관리하는 SelectManager 클래스를 선언합니다.
 * @author 최성락
 * @date 2025-06-17
 * 
 * @details
 * 소켓 I/O 다중화를 위해 `select` 함수의 설정과 사용을 캡슐화합니다. 
 * 감시할 소켓 집합을 유지하고, 소켓이 준비되었는지, 타임아웃됐는지, 오류가 발생했는지를 나타내는 결과를 제공합니다.
 */

#include <WinSock2.h>

 /**
  * @class SelectManager
  * @brief 소켓 집합을 관리하고 select()를 사용하여 네트워크 I/O를 다중화합니다.
  *
  * @details
  * SelectManager는 select에 필요한 fd_set 구조체를 래핑합니다.
  * <br>소켓을 추가하여 select 연산을 수행하는 메서드를 제공합니다.
  * <br>어떤 소켓에 수신 데이터가 있는지(읽기 준비 완료 상태인지)를 확인합니다.
  * <br>타임아웃 상황을 처리할 수 있습니다.
  */
class SelectManager
{
public:

	/**
	 * @enum SelectManager::Result
	 * @brief select 연산에 대한 결과 상태 값입니다.
	 */
	enum class Result
	{
		SUCCESS,		///< select 호출에 성공, 적어도 하나의 소켓이 I/O 처리 준비가 된 경우.
		TIMEOUT,		///< 지정된 타임아웃 내에 아무 소켓에서도 활동이 없어 select가 타임아웃된 경우.
		FAIL_SELECT,	///< select 호출이 실패한 경우 (예: 잘못된 소켓 등 오류로 인해).
		NO_SOCKETS		///< 모니터링할 소켓이 하나도 설정되지 않은 경우 (select가 호출되지 않음).
	};

public:
	/**
	 * @fn SelectManager::SelectManager()
	 * @brief SelectManager를 생성하여 내부 소켓 집합을 초기화합니다.
	 * @return 없음.
	 *
	 * @details
	 * 빈 fd_set들을 준비합니다.
	 */
	SelectManager();

	/**
	 * @fn SelectManager::~SelectManager()
	 * @brief SelectManager의 소멸자.
	 * @return 없음.
	 * 
	 * @note 내부적으로 해제할 동적 리소스는 없습니다.
	 */
	~SelectManager();

	// 복사 생성자 및 복사 할당 연산자 삭제.
	SelectManager(const SelectManager& obj) = delete;
	SelectManager& operator=(const SelectManager& obj) = delete;

	// 이동 생성자 및 이동 할당 연산자 삭제.
	SelectManager(SelectManager&& obj) = delete;
	SelectManager& operator=(SelectManager&& obj) = delete;

public:

	/**
	 * @fn void SelectManager::setupFdSet()
	 * @brief 새로운 select 작업을 위해 fd_set을 초기화하여 준비합니다.
	 * @return 없음.
	 *
	 * 내부 소켓 집합을 모두 비우고 소켓 개수를 0으로 재설정합니다. 
	 * <br>필요한 경우 (예: 서버 루프의 각 반복마다) 소켓을 다시 추가하기 전에 이 메서드를 호출하십시오.
	 */
	void setupFdSet();

	/**
	 * @fn bool SelectManager::addSocket(SOCKET socket)
	 * @brief select()로 모니터링할 소켓 집합에 소켓을 추가합니다.
	 * @param[IN] SOCKET socket : 감시 목록에 추가할 소켓입니다.
	 * @return bool : 소켓이 정상적으로 추가되어 유효하면 true, 추가할 수 없으면 false를 반환합니다 (예: 소켓 개수 초과 또는 잘못된 소켓인 경우).
	 *
	 * @details
	 * 해당 소켓을 내부 fd_set에 추가합니다.
	 */
	bool addSocket(SOCKET socket);

	/**
	 * @fn SelectManager::Result SelectManager::executeSelect(int timeout_sec)
	 * @brief 현재 설정된 소켓 집합에 대해 select() 시스템 호출을 수행합니다.
	 * @param[IN] int timeout_sec : 이벤트를 기다릴 시간(초)입니다 (기본값: 1초).
	 * @return SelectManager::Result : select 연산의 결과를 반환합니다 (SUCCESS, TIMEOUT, FAIL_SELECT 또는 NO_SOCKETS).
	 *
	 * @details
	 * 내부적으로 소켓 집합을 임시 복사본으로 만들고 select()를 호출합니다. 
	 * <br>하나 이상의 소켓이 준비되었다면 SUCCESS를 반환합니다.
	 * <br>집합에 소켓이 없다면 select를 호출하지 않고 NO_SOCKETS를 반환합니다. 
	 * <br>타임아웃 시 TIMEOUT을, 오류 발생 시 FAIL_SELECT를 반환합니다.
	 */
	SelectManager::Result executeSelect(int timeout_sec = 1);

	/**
	 * @fn bool SelectManager::isSocketReady(SOCKET socket) const
	 * @brief 해당 소켓이 "읽기 준비 완료"로 표시되었는지 확인합니다.
	 * @param[IN] SOCKET socket : 확인할 소켓입니다.
	 * @return bool : 소켓에 읽을 데이터가 있어(select에 의해 fd_set에 설정된 경우) true를 반환하고, 그렇지 않으면 false를 반환합니다.
	 *
	 * @note
	 * 이 메서드는 executeSelect() 호출 후에 사용해야 합니다. 
	 * <br>select가 업데이트한 내부 fd_set 복사본을 검사합니다.
	 */
	bool isSocketReady(SOCKET socket) const;

private:
	/// 재사용 가능한 원본 소켓 집합 (모니터링할 소켓들의 집합).
	fd_set _originSet;

	/// select()에 전달되는 소켓 집합의 복사본입니다 (select 호출 시 변경될 수 있음).
	fd_set _copySet;

	/// 현재 집합에 포함된 소켓 개수입니다.
	int  _socketCount;
};

