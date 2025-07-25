#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CAnimatio_Controller final : public CComponent
{
private:
	CAnimatio_Controller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CAnimatio_Controller() = default;

public:
	virtual HRESULT					Initialize_Prototype(const string* pAnimFilePath);
	virtual HRESULT					Initialize(void* pArg);

	_bool							Player_Animation(string szAnimName, class CModel* pModel, _float fTimeDelta);
	void							Set_AnimStatus(ANIM_STATUS eStatus);
private:
	string							m_szPreAnimName = {};
	string							m_szCulAnimName = {};
	ANIMEFRAME						m_CulAnim = {};

	ANIM_STATUS						m_eAnim_Status = { ANIM_STATUS::PLAY };
	unordered_map<string, 
		ANIMEFRAME>					m_AnimDatas;

private:
	HRESULT							Load_AnimData(const string* pAnimFilePath);
	ANIMEFRAME						Find_Anim(const string pAnimName);
public:
	static CAnimatio_Controller*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string* pAnimFilePath);
	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END