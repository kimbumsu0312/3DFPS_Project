#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include "GameInstance.h"
#include "Engine_Defines.h"

//클라 관련
#include "Client_Enum.h"
#include "Client_Defines.h"
#include "Client_Event.h"

//매니저 관련
#include "Player_Manager.h"
#include "Inven_Manager.h"

//UI 관련
#include "UIObject.h"
#include "PoolUIObj.h"
#include "StateObject.h"
#include "WeaponObject.h"
#include "UI_Header.h"

//몬스터 관련
#include "Monster_Header.h"
#include "Alcina.h"
#include "Body_Alcina.h"

//플레이어 관련
#include "Player_Header.h"

//맵 관련
#include "BackGround.h"
#include "Terrain.h"
#include "Sky.h"
#include "BaseMapObj.h"

//아이템 관련
#include "InvenItem.h"
#include "PoolWorldItem.h"

//유틸 관련
#include "Camera_Free.h"
#include "MapNevi.h"
#include "MonSpawner.h"
#include "SpawnPoint.h"
#include "ItemSpawner.h"



#endif //PCH_H