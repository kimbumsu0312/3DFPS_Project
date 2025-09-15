#include "pch.h"
#include "Bounding_OBB.h"
#include "Bounding_AABB.h"
#include "Bounding_Sphere.h"

CBounding_OBB::CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CBounding{ pDevice, pContext }
{
}

HRESULT CBounding_OBB::Initialize(const CBounding::BOUNDING_DESC* pDesc)
{
    const BOUNDING_OBB_DESC* pOBBDesc = static_cast<const BOUNDING_OBB_DESC*>(pDesc);

    _float4		vQuaternion = {};
    XMStoreFloat4(&vQuaternion, XMQuaternionRotationRollPitchYaw(pOBBDesc->vAngles.x, pOBBDesc->vAngles.y, pOBBDesc->vAngles.z));

    m_pOriginalDesc = new BoundingOrientedBox(pOBBDesc->vCenter, pOBBDesc->vExtents, vQuaternion);
    m_pDesc = new BoundingOrientedBox(*m_pOriginalDesc);
    
    return S_OK;
}

void CBounding_OBB::Update(_fmatrix WorldMatrix)
{
    _vector vScale, vRot, vTrans;

    XMMatrixDecompose(&vScale, &vRot, &vTrans, WorldMatrix);

 //   _vector vOrigRot = XMLoadFloat4(&m_pOriginalDesc->Orientation);
 //   _vector finalRot = vOrigRot * vRot;
    _matrix worldMat = XMMatrixRotationQuaternion(vRot) * XMMatrixTranslationFromVector(vTrans);


    m_pOriginalDesc->Transform(*m_pDesc, worldMat);
}

_bool CBounding_OBB::Intersect(COLLIDER eType, CBounding* pTarget, _float3& pOutNroaml, _float3& pTargetNroaml)
{
    _bool isColl = { false };

    switch (eType)
    {
    case Engine::COLLIDER::AABB:
        isColl = m_pDesc->Intersects(*static_cast<CBounding_AABB*>(pTarget)->Get_Desc());
        break;
    case Engine::COLLIDER::OBB:
        isColl = Intersect_OBB(static_cast<CBounding_OBB*>(pTarget), pOutNroaml, pTargetNroaml);
        break;
    case Engine::COLLIDER::SPHERE:
        isColl = m_pDesc->Intersects(*static_cast<CBounding_Sphere*>(pTarget)->Get_Desc());
        break;
    }
    return isColl;
}
_bool CBounding_OBB::Intersect(RAY_DESC& RayDesc)
{
    _bool isColl = { false };
	  
    isColl = m_pDesc->Intersects(RayDesc.RayPos, RayDesc.RayDIr, RayDesc.OnColiderDis);

	if (isColl)
	{
		RayDesc.OnCloiderPos = RayDesc.RayPos + (RayDesc.RayDIr * RayDesc.OnColiderDis);
	}

    return isColl;
}

_bool CBounding_OBB::Intersect_OBB(CBounding_OBB* pTarget, _float3& pOut, _float3& pTargetNoraml)
{
	OBBDESC			OBBDesc[2];

	OBBDesc[0] = Compute_OBBDesc();
	OBBDesc[1] = pTarget->Compute_OBBDesc();

	_float		fDistance[3];
	_float		fGap = D3D11_FLOAT32_MAX;

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			_vector vAxis = XMLoadFloat3(&OBBDesc[i].vAlignDir[j]);

			fDistance[0] = fabsf(XMVectorGetX(XMVector3Dot(
				XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter),
				XMLoadFloat3(&OBBDesc[i].vAlignDir[j]))));

			fDistance[1] =
				fabsf(XMVectorGetX(XMVector3Dot(
					XMLoadFloat3(&OBBDesc[0].vCenterDir[0]),
					XMLoadFloat3(&OBBDesc[i].vAlignDir[j])))) +
				fabsf(XMVectorGetX(XMVector3Dot(
					XMLoadFloat3(&OBBDesc[0].vCenterDir[1]),
					XMLoadFloat3(&OBBDesc[i].vAlignDir[j])))) +
				fabsf(XMVectorGetX(XMVector3Dot(
					XMLoadFloat3(&OBBDesc[0].vCenterDir[2]),
					XMLoadFloat3(&OBBDesc[i].vAlignDir[j]))));

			fDistance[2] =
				fabsf(XMVectorGetX(XMVector3Dot(
					XMLoadFloat3(&OBBDesc[1].vCenterDir[0]),
					XMLoadFloat3(&OBBDesc[i].vAlignDir[j])))) +
				fabsf(XMVectorGetX(XMVector3Dot(
					XMLoadFloat3(&OBBDesc[1].vCenterDir[1]),
					XMLoadFloat3(&OBBDesc[i].vAlignDir[j])))) +
				fabsf(XMVectorGetX(XMVector3Dot(
					XMLoadFloat3(&OBBDesc[1].vCenterDir[2]),
					XMLoadFloat3(&OBBDesc[i].vAlignDir[j]))));

			if (fDistance[0] >= fDistance[1] + fDistance[2])
				return false;

			if (fGap > fDistance[1] + fDistance[2] - fDistance[0])
			{
				fGap = fDistance[1] + fDistance[2] - fDistance[0];
				_float fDot = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])));
				if (fDot < 0)
				{
					XMStoreFloat3(&pOut, XMLoadFloat3(&OBBDesc[i].vCenterDir[j]) * -1.f);
					pTargetNoraml = OBBDesc[i].vCenterDir[j];
				}
				else
				{
					pOut = OBBDesc[i].vCenterDir[j];
					XMStoreFloat3(&pTargetNoraml, XMLoadFloat3(&OBBDesc[i].vCenterDir[j]) * -1.f);
				}
			}
		}
	}
	return true;
}

#ifdef _DEBUG
HRESULT CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
    DX::Draw(pBatch, *m_pDesc, vColor);
    return S_OK;
}
#endif

CBounding_OBB::OBBDESC CBounding_OBB::Compute_OBBDesc() const
{
	OBBDESC			OBBDesc{};

	_float3			 vPoints[8];
	m_pDesc->GetCorners(vPoints);


	OBBDesc.vCenter = m_pDesc->Center;
	XMStoreFloat3(&OBBDesc.vCenterDir[0], (XMLoadFloat3(&vPoints[5]) - XMLoadFloat3(&vPoints[4])) * 0.5f);
	XMStoreFloat3(&OBBDesc.vCenterDir[1], (XMLoadFloat3(&vPoints[7]) - XMLoadFloat3(&vPoints[4])) * 0.5f);
	XMStoreFloat3(&OBBDesc.vCenterDir[2], (XMLoadFloat3(&vPoints[0]) - XMLoadFloat3(&vPoints[4])) * 0.5f);

	for (size_t i = 0; i < 3; i++)
		XMStoreFloat3(&OBBDesc.vAlignDir[i], XMVector3Normalize(XMLoadFloat3(&OBBDesc.vCenterDir[i])));

	return OBBDesc;
}

CBounding_OBB* CBounding_OBB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::BOUNDING_DESC* pDesc)
{
    CBounding_OBB* pInstance = new CBounding_OBB(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pDesc)))
    {
        MSG_BOX(TEXT("Failed to Created : CBounding_OBB"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBounding_OBB::Free()
{
    __super::Free();

    Safe_Delete(m_pDesc);
    Safe_Delete(m_pOriginalDesc);
}
