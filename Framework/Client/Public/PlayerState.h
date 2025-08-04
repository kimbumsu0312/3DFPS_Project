#pragma once
#include "StateObject.h"
#include "Client_Struct.h"

NS_BEGIN(Client)
class CPlayerState abstract : public CStateObject
{
public:
    typedef struct tagPlayerStateDesc {
        _uint* pWeaponState = { nullptr };
        string* pAnimTag = { nullptr };
        _wstring* pStateTag = { nullptr };
        _bool* pIsAnimLoop = { nullptr };
        _bool* pIsAnimFinsh = { nullptr };
    }Player_STATE_DESC;
protected:
    CPlayerState();
    virtual ~CPlayerState() = default;

public:
    virtual HRESULT         Initalize(void* pArg);
    virtual void            Enter(const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState) = 0;
    virtual void            Update(_float fDeltatime, const PLAYER_ATTACK_STATE& pAttackState, const PLAYER_MOVE_STATE& pMoveState) = 0;
    virtual void            Exit() = 0;

protected:
    _uint* m_pWeaponState = { nullptr };
    string* m_pAnimTag = { nullptr };

    _wstring* m_pStateTag = { nullptr };
    _bool* m_pIsAnimLoop = { nullptr };
    _bool* m_pIsAnimFinsh = { nullptr };

protected:
    virtual void            Free() override;
};
NS_END