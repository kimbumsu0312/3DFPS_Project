#include "pch.h"
#include "VIBuffer_Rect_Line.h"

CVIBuffer_Rect_Line::CVIBuffer_Rect_Line(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Rect_Line::CVIBuffer_Rect_Line(const CVIBuffer_Rect_Line& Prototype) : CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Rect_Line::Initialize_Prototype()
{
	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VTXPOSCOLOR);
	m_iNumIndices = 6;
	m_iIndexStride = 2;
	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;



	return S_OK;
}

HRESULT CVIBuffer_Rect_Line::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Rect_Line* CVIBuffer_Rect_Line::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Rect_Line* pInstance = new CVIBuffer_Rect_Line(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_Rect_Line"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Rect_Line::Clone(void* pArg)
{
	CVIBuffer_Rect_Line* pInstance = new CVIBuffer_Rect_Line(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_Rect_Line"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Rect_Line::Free()
{
	__super::Free();
}
