#pragma once
#include "Base.h"
#include "Player_Manager.h"

NS_BEGIN(Client)
template<typename T>
class CStateObject abstract : public CBase
{
protected:
	CStateObject() : m_pGameInstance(CGameInstance::GetInstance()) {
        Safe_AddRef(m_pGameInstance); }
	virtual ~CStateObject() = default;

public:
    virtual HRESULT         Initalize(void *pArg = nullptr) = 0;
    virtual void            Enter(T* pContainer) = 0;    
    virtual void            Update(T* pContainer, _float fDeltatime) = 0;
    virtual void            Exit(T* pContainer) = 0;

protected:
    CGameInstance*          m_pGameInstance = { nullptr };
    STATE_ANIM				m_eAnimState = STATE_ANIM::END;

protected:
    DIRECTION			    PlayerDIR(_vector vMyPos, _vector vMyLook);

protected:
    virtual void            Free() override {
        Safe_Release(m_pGameInstance); };
};
NS_END

template<typename T>
inline DIRECTION CStateObject<T>::PlayerDIR(_vector vMyPos, _vector vMyLook)
{
    _vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();

    _vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPos - vMyPos, 0.f));
    _vector vLook = XMVector3Normalize(XMVectorSetY(vMyLook, 0.f));

    _float fDot = XMVectorGetX(XMVector3Dot(vLook, vDir));
    _float fAngle = XMConvertToDegrees(acosf(fDot));

    _float fRight = XMVectorGetX(XMVector3Dot(XMVector3Cross(vLook, vDir), XMVectorSet(0, 1, 0, 0)));

    if (fAngle <= 22.5f)
        return DIRECTION::F;
    else if (fAngle <= 67.5f)
        return (fRight > 0) ? DIRECTION::FR : DIRECTION::FL;
    else if (fAngle <= 112.5f)
        return (fRight > 0) ? DIRECTION::R : DIRECTION::L;
    else if (fAngle <= 157.5f)
        return (fRight > 0) ? DIRECTION::BR : DIRECTION::BL;
    else
        return DIRECTION::B;
}
