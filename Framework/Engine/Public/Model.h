#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CModel final : public CComponent
{
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& Prototype);
	virtual ~CModel() = default;

public:
	virtual HRESULT			Initialize_Prototype(const _char* pModelFilePath);
	virtual HRESULT			Initialize(void* pArg);

	HRESULT					Render(_uint Index);
private:
	const aiScene*			m_pAIScene = { nullptr };
	Assimp::Importer		m_Importer = {};
	_uint					m_iNumMeshes = {};
	vector<class CMesh*>	m_Meshes;

private:
	HRESULT					Ready_Meshes();

public:
	static CModel*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath);
	virtual CComponent*		Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END