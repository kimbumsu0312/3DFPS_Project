#include "pch.h"
#include "NeviMesh.h"
#include "MapObject.h"
#include "Imgui_Manager.h"
#include "PointCube.h"
CNeviMesh::CNeviMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CContainerObject{pDevice, pContext}
{
}

CNeviMesh::CNeviMesh(const CNeviMesh& Prototype) : CContainerObject( Prototype )
{
}

HRESULT CNeviMesh::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNeviMesh::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	Ready_Components();
	Ready_PartObjects();
	CImgui_Manger::GetInstance()->Set_NeviMesh(this);
	return S_OK;
}

void CNeviMesh::Priority_Update(_float fTimeDelta)
{
	dynamic_cast<CPointCube*>(m_PartObjects.at(TEXT("Part_PointA")))->IsSelete(false);
	dynamic_cast<CPointCube*>(m_PartObjects.at(TEXT("Part_PointB")))->IsSelete(false);
	dynamic_cast<CPointCube*>(m_PartObjects.at(TEXT("Part_PointC")))->IsSelete(false);
	switch (m_iSeletePoint)
	{
	case 0:
		dynamic_cast<CPointCube*>(m_PartObjects.at(TEXT("Part_PointA")))->IsSelete(true);
		break;
	case 1:
		dynamic_cast<CPointCube*>(m_PartObjects.at(TEXT("Part_PointB")))->IsSelete(true);
		break;
	case 2:
		dynamic_cast<CPointCube*>(m_PartObjects.at(TEXT("Part_PointC")))->IsSelete(true);
		break;
	}
}

void CNeviMesh::Update(_float fTimeDelta)
{
	if (g_SeletePos)
	{
		if (m_pGameInstance->IsMouseDown(MOUSEKEYSTATE::LB))
		{
			_bool IsPick = false;
			_float fSourcelength = {-1.f};

			for (auto Object : *m_fObjects)
			{
				_float fDestlength = { -1.f };
				if (true == static_cast<CMapObject*>(Object)->IsPick_Objcet(fDestlength))
				{
					IsPick = true;
	
					if (fSourcelength < 0.f || fSourcelength > fDestlength)
					{
						fSourcelength = fDestlength;
					}
				}
			}

			if (IsPick)
			{
				_vector vLocalPos = {};

				_vector vPickLocalPos = m_pGameInstance->Get_LocalRayPos();
				_vector vPickLocalRay = m_pGameInstance->Get_LocalRayDir();

				vLocalPos = vPickLocalPos + vPickLocalRay * fSourcelength;				
				
				XMStoreFloat3(&m_fCellPoint[m_iSeletePoint] ,XMVector3TransformCoord(vLocalPos, m_pTransformCom->Get_WorldMatrix()));

				if (m_pNevigationCom->IsSnap(m_fCellPoint[m_iSeletePoint], 0.3f))
					int a = 10;

				switch (m_iSeletePoint)
				{
				case 0:
					dynamic_cast<CPointCube*>(m_PartObjects.at(TEXT("Part_PointA")))->Set_Transform(m_fCellPoint[m_iSeletePoint]);
					break;
				case 1:
					dynamic_cast<CPointCube*>(m_PartObjects.at(TEXT("Part_PointB")))->Set_Transform(m_fCellPoint[m_iSeletePoint]);
					break;
				case 2:
					dynamic_cast<CPointCube*>(m_PartObjects.at(TEXT("Part_PointC")))->Set_Transform(m_fCellPoint[m_iSeletePoint]);
					break;
				}
			}
		}
	}
	if (m_pGameInstance->IsKeyDown(DIK_1))
	{
		m_iSeletePoint = 0;
	}
	if (m_pGameInstance->IsKeyDown(DIK_2))
	{
		m_iSeletePoint = 1;
	}
	if (m_pGameInstance->IsKeyDown(DIK_3))
	{
		m_iSeletePoint = 2;
	}
	
	if (m_pGameInstance->IsKeyDown(DIK_U))
	{
		m_pNevigationCom->Add_Cell(m_fCellPoint);
	}
	m_pNevigationCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CNeviMesh::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;
}

HRESULT CNeviMesh::Render()
{
	m_pNevigationCom->Render();

	return S_OK;
}

void CNeviMesh::Set_Objcets()
{
	m_fObjects = m_pGameInstance->Get_Objects();
}

void CNeviMesh::Save_Nevi(string szFileName)
{
	m_pNevigationCom->Save_Cell(szFileName);
}

HRESULT CNeviMesh::Ready_Components()
{
	m_pNevigationCom = CNavigation::Create(m_pDevice, m_pContext);

	return S_OK;
}

HRESULT CNeviMesh::Ready_PartObjects()
{
	CPartObject::PARTOBJECT_DESC Desc;
	Desc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();

	if (FAILED(__super::Add_PartObject(TEXT("Part_PointA"), ENUM_CLASS(LEVEL::MAP), TEXT("Prototype_GameObjcet_PointCube"), &Desc)))
		return E_FAIL;
	if (FAILED(__super::Add_PartObject(TEXT("Part_PointB"), ENUM_CLASS(LEVEL::MAP), TEXT("Prototype_GameObjcet_PointCube"), &Desc)))
		return E_FAIL;
	if (FAILED(__super::Add_PartObject(TEXT("Part_PointC"), ENUM_CLASS(LEVEL::MAP), TEXT("Prototype_GameObjcet_PointCube"), &Desc)))
		return E_FAIL;

	return E_NOTIMPL;
}

CNeviMesh* CNeviMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNeviMesh* pInstance = new CNeviMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CNeviMesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNeviMesh::Clone(void* pArg)
{
	CNeviMesh* pInstance = new CNeviMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CNeviMesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNeviMesh::Free()
{
	__super::Free();
	Safe_Release(m_pNevigationCom);
}
