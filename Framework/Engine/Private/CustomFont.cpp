#include "pch.h"
#include "CustomFont.h"

CCustomFont::CCustomFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : m_pDevice{ pDevice }, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCustomFont::Initialize(const _tchar* pFontFilePath)
{
	m_pBatch = new SpriteBatch(m_pContext);
	m_pFont = new SpriteFont(m_pDevice, pFontFilePath);

	D3D11_BLEND_DESC blendDesc = {};
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;

	// RenderTarget[0] 만 쓰는 경우 (UI, 텍스트 등)
	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
	rtBlendDesc.BlendEnable = TRUE;

	// 컬러 블렌딩
	rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;

	// 알파 블렌딩
	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;

	rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// 첫 번째 렌더타겟에 설정
	blendDesc.RenderTarget[0] = rtBlendDesc;

	m_pDevice->CreateBlendState(&blendDesc, &m_pBlendState);
	return S_OK;
}

void CCustomFont::DrawText(const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRadian, const _float2& vOrigin, const _float2& vScale)
{
	m_pBatch->Begin(SpriteSortMode_Deferred, m_pBlendState);
	_float2 vPos = vPosition;
	_vector textSize = m_pFont->MeasureString(pText);

	_float textWidth = (XMVectorGetX(textSize) * vScale.x) * vOrigin.x;
	_float textHight = (XMVectorGetY(textSize) * vScale.y) * vOrigin.y;
	vPos.x -= textWidth;
	vPos.y -= textHight;


	m_pFont->DrawString(m_pBatch, pText, vPos, vColor, fRadian, vOrigin, vScale);

	m_pBatch->End();
}

CCustomFont* CCustomFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath)
{
	CCustomFont* pInstance = new CCustomFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pFontFilePath)))
	{
		MSG_BOX(TEXT("Failed to Created : CCustomFont"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCustomFont::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pBlendState);

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pFont);
}
