#pragma once
#include "Engine_Defines.h"
#include "ContainerObject.h"
#include "Client_Struct.h"
#include "BlackBoard.h"

NS_BEGIN(Engine)
class CCollider;
NS_END

NS_BEGIN(Client)
class CCrow final : public CContainerObject
{
public:
	typedef struct CrowData
	{
		//애니메이션 관련
		string* szAnimTag = { nullptr };
		_bool* bIsAnimLoop = { nullptr };
		_bool* bIsAnimFinsh = { nullptr };

		_wstring* szCulStateTag = { nullptr };

	}CROW_DATA;

private:
	CCrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCrow(const CCrow& Prototype);
	virtual ~CCrow() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Priority_Update(_float fTimeDelta);
	virtual void					Update(_float fTimeDelta);
	virtual void					Late_Update(_float fTimeDelta);
	virtual HRESULT					Render();

private:
	//애니메이션 관련
	string							m_szAnimTag = {};
	_bool							m_bIsAnimLoop = { true };
	_bool							m_bIsAnimFinsh = { false };

	//파츠 관련
	class CBody_Crow*				m_pBodyObject = { nullptr };
	
	//유틸 관련
	CBlackBoard<CROW_DATA>*			m_BlackBoard = { nullptr };

	_bool							m_bIsNextLevel = { false };
private:
	HRESULT							Ready_PartObjects();
	HRESULT							Ready_Utility();

	void							Root_Move();
public:
	static CCrow*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free();

};

NS_END