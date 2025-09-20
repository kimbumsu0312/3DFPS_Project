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
#include "Guide_Paper.h"
#include "LodingUI.h"
#include "Loding_Side.h"
#include "Loding_Icon.h"
#include "BulletCount.h"
#include "Screen_Blood.h"

//몬스터 관련
#include "Monster_Header.h"
#include "Alcina.h"
#include "Body_Alcina.h"
#include "Monster_WereWolf.h"
#include "Body_WereWolf.h"

//플레이어 관련
#include "ShotGun.h"
#include "Player.h"
#include "Body_Player.h"
#include "Camera_Player.h"
#include "Knife.h"
#include "HandGun.h"
#include "Sniper.h"

//맵 관련
#include "BackGround.h"
#include "Terrain.h"
#include "Sky.h"
#include "BaseMapObj.h"
#include "Model_Door.h"
#include "Event_Door.h"

//아이템 관련
#include "InvenItem.h"
#include "PoolWorldItem.h"
#include "WorldItem.h"
#include "MsgItem.h"

//유틸 관련
#include "Camera_Free.h"
#include "MapNevi.h"
#include "MonSpawner.h"
#include "SpawnPoint.h"
#include "ItemSpawner.h"
#include "Trigger.h"

//이펙트 관련
#include "Snow.h"
#include "Muzzle_Effect.h"
#include "Blood_Effect.h"
#include "Fly_Effect.h"
#include "Spark_Effect.h"
#include "Screen_Blood_Particle.h"
//카메라
#include "LogoCamera.h"

//자연
#include "Body_Crow.h"
#include "Crow.h"
#include "LogoMap.h"
#endif //PCH_H