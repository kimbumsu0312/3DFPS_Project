#include "pch.h"
#include "Spawn_Bela.h"
#include "Bela.h"
#include "Player_Manager.h"

CSpawn_Bela::CSpawn_Bela()
{
}

HRESULT CSpawn_Bela::Initalize(void* pArg)
{
    return S_OK;
}

void CSpawn_Bela::Enter(CBela* pContainer)
{
    m_eAnimState = STATE_ANIM::START;
    *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CBela::ANIM_STATE::ATTACK);
    pContainer->Switch_Anim("Freeszes_Attack_CriticalHit", false);

    _matrix vPlayerMat = CPlayer_Manager::GetInstance()->Get_PlayerWorld();
    _vector vScale, vWorldRot, vWorldTrans;
    XMMatrixDecompose(&vScale, &vWorldRot, &vWorldTrans, vPlayerMat);

    _vector vLook = { XMVectorGetX(vPlayerMat.r[2]), XMVectorGetY(vPlayerMat.r[2]), XMVectorGetZ(vPlayerMat.r[2]), 0.f};

    _vector vOffset = { 0.f, 0.f , -4.f, 1.f };
    vOffset = XMVector3Rotate(vOffset, vWorldRot);
    _vector vPos = vWorldTrans + vOffset;

    pContainer->Get_Transform()->Set_State(STATE::POSITION, vPos);
    pContainer->Get_Navi()->Set_CellIndex(CPlayer_Manager::GetInstance()->Get_CellIndex());
    pContainer->Get_Transform()->LookAt(vWorldTrans);

    pContainer->Get_BlackBoard()->Set_Data().fNoies = 1.f;
    m_fAccTime = 0.f;

    pContainer->Get_BlackBoard()->Set_Data().bIsFly = false;
    m_pGameInstance->OnOff_Light(TEXT("Light_Bela"), true);
}

void CSpawn_Bela::Update(CBela* pContainer, _float fDeltatime)
{
    if (m_eAnimState == STATE_ANIM::START)
    {
        pContainer->Attack_Collision();

        m_eAnimState = STATE_ANIM::LOOP;
    }
    else if (m_eAnimState == STATE_ANIM::LOOP)
    {
        pContainer->Attack_Collision();

        m_fAccTime += fDeltatime * 0.02f;
        m_fEffectTime += fDeltatime;

        pContainer->Get_BlackBoard()->Set_Data().bIsFly = true;

        pContainer->Get_BlackBoard()->Set_Data().fNoies -= m_fAccTime;

        if (*pContainer->Get_BlackBoard()->Get_Data().bIsAnimFinsh == true)
        {
            m_eAnimState = STATE_ANIM::END;
            *pContainer->Get_BlackBoard()->Set_Data().iAnimState = ENUM_CLASS(CBela::ANIM_STATE::NORMAL);
            pContainer->Switch_Anim("Idle_Freezes", true);

            pContainer->Get_BlackBoard()->Set_Data().fNoies = 0.f;
            m_fAccTime = 0.f;

            CFly_Effect::FLY_EFFECT_INIT Desc;
            Desc.vPos = pContainer->Get_Transform()->Get_State(STATE::POSITION);
            Desc.vPos = XMVectorSetY(Desc.vPos, XMVectorGetY(Desc.vPos) + 1.5f);
        }
    }
    else if (m_eAnimState == STATE_ANIM::END)
    {
        m_fAccTime += fDeltatime * 0.02f;
        pContainer->Get_BlackBoard()->Set_Data().fNoies += m_fAccTime;
        if (pContainer->Get_BlackBoard()->Set_Data().fNoies >= 1.f)
        {
            pContainer->Get_BlackBoard()->Set_Data().IsSpawn = false;
            pContainer->Get_BlackBoard()->Set_Data().IsChase = true;
        }
    }
}

void CSpawn_Bela::Exit(CBela* pContainer)
{
    pContainer->Get_BlackBoard()->Set_Data().bIsFly = false;
}

CSpawn_Bela* CSpawn_Bela::Create(void* pArg)
{
    CSpawn_Bela* pInstance = new CSpawn_Bela();

    if (FAILED(pInstance->Initalize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CSpawn_Bela"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSpawn_Bela::Free()
{
    __super::Free();
}
