#include "pch.h"
#include "Level_Loading.h"
#include "Loader.h"
#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_GamePlay.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	if (FAILED(Ready_GameObjects()))
		return E_FAIL;

	if (FAILED(Ready_LoadingThread()))
		return E_FAIL;
	m_pGameInstance->Subscribe<Event_NextLevel>([&](const Event_NextLevel& e) {m_bIsNextLevel = true; });

	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
	if (true == m_pLoader->isFinished())
		//&&	GetKeyState(VK_SPACE) & 0x8000)
	{
		CLevel* pNewLevel = { nullptr };

		switch (m_eNextLevelID)
		{
		case LEVEL::LOGO:
			pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
			if (FAILED(m_pGameInstance->Open_Level(static_cast<_uint>(m_eNextLevelID), pNewLevel)))
				return;
			break;
		case LEVEL::GAMEPLAY:
			if (!m_bisEvent)
			{
				m_pGameInstance->Publish(Event_EndLoding{});
				m_bisEvent = true;
			}

			if (m_bIsNextLevel)
			{
				pNewLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				if (FAILED(m_pGameInstance->Open_Level(static_cast<_uint>(m_eNextLevelID), pNewLevel)))
					return;
			}
			break;
		}
	}
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Show_LoadingText();

	return S_OK;
}

HRESULT CLevel_Loading::Ready_GameObjects()
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOADING), TEXT("Layer_UI"),
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Object_LodingUI"))))
		return E_FAIL;

	CUIObject::UIOBJECT_DESC Desc = {};
	Desc.iIndex = 0;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOADING), TEXT("Layer_UI"),
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Object_Loding_Side"), &Desc)))
		return E_FAIL;
	Desc.iIndex = 1;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOADING), TEXT("Layer_UI"),
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Object_Loding_Side"), &Desc)))
		return E_FAIL;
	
	Desc.fRotationPerSec = 1.f;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOADING), TEXT("Layer_UI"),
		ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Object_Loding_Icon"), &Desc)))
		return E_FAIL;
	
    return S_OK;
}

HRESULT CLevel_Loading::Ready_LoadingThread()
{
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
