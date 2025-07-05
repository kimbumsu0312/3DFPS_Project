#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Line final : public CVIBuffer
{
private:
	CVIBuffer_Rect_Line(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Rect_Line(const CVIBuffer_Rect_Line& Prototype);
	virtual ~CVIBuffer_Rect_Line() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;

public:
	static CVIBuffer_Rect_Line* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*			Clone(void* pArg) override;
	virtual void				Free() override;
};

NS_END