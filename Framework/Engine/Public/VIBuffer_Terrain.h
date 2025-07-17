#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
public:

	typedef struct tagVIBuffer_Terrain
	{
		_uint	iNumverticesX = {};
		_uint	iNumverticesZ = {};
	}VIBUFFER_TERRAIN_DESC;


private:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize_Prototype(const _tchar* pHeightMapFilePath);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual HRESULT				Initialize();
	
private:
	_uint						m_iNumverticesX = {};
	_uint						m_iNumverticesZ = {};
	
public:
	static CVIBuffer_Terrain*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath);
	static CVIBuffer_Terrain*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*			Clone(void* pArg);
	virtual void				Free() override;
};

NS_END