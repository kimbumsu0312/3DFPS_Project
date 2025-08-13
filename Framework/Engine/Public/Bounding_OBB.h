#pragma once
#include "Bounding.h"

NS_BEGIN(Engine)
class ENGINE_DLL CBounding_OBB final : public CBounding
{
public:
	typedef struct tagBoundingAOBDesc : public CBounding::BOUNDING_DESC
	{
		_float3		vExtents;
		_float3		vAngles;
	}BOUNDING_OBB_DESC;
private:
	CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_OBB() = default;

public:
	const BoundingOrientedBox* Get_Desc() const { return m_pDesc; }

public:
	HRESULT					Initialize(const CBounding::BOUNDING_DESC* pDesc);
	virtual void			Update(_fmatrix WorldMatrix) override;
	virtual _bool			Intersect(COLLIDER eType, CBounding* pTarget) override;
#ifdef _DEBUG
public:
	virtual HRESULT			Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
#endif

public:
	BoundingOrientedBox*	m_pOriginalDesc{};
	BoundingOrientedBox*	m_pDesc{};

public:
	static CBounding_OBB*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::BOUNDING_DESC* pDesc);
	virtual void			Free() override;

};
NS_END
