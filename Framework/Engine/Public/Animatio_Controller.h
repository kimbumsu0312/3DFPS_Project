#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CAnimatio_Controller final : public CComponent
{
public:
	typedef struct tagAnimtioDesc
	{
		_int				iAnimIndex = {};
		string				szFile_Path = {};
		string				szCulAnimName = {};
		_bool				IsLoop = { false };
		class CModel*		pModel;
	}ANIMTION_DESC;
private:
	CAnimatio_Controller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CAnimatio_Controller() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);

	_bool							Player_Animation(_int iAnimIndex, string szAnimName, _bool IsLoop, class CModel* pModel, _float fTimeDelta, _int RootNodeIndex);
	void							Set_AnimStatus(ANIM_STATUS eStatus);

private:
	_int							m_iCulAnimIndex = {};
	_bool							m_bIsLoop = { false };

	string							m_szPreAnimName = {};
	string							m_szCulAnimName = {};
	ANIMEFRAME						m_CulAnimFrame = {};

	ANIM_STATUS						m_eAnim_Status = { ANIM_STATUS::PLAY };
	vector<unordered_map<string, 
		ANIMEFRAME>>				m_AnimDatas;

private:
	HRESULT							Load_AnimData(const string pAnimFilePath);
	ANIMEFRAME						Find_Anim(const string pAnimName);
public:
	static CAnimatio_Controller*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END