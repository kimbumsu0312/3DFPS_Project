#include "pch.h"
#include "Navigation.h"
#include "Cell.h"

#include "GameInstance.h"

_float4x4 CNavigation::m_WorldMatrix = {};

struct CompareNode
{
	bool operator()(const Node a, const Node b)
	{
		return a.f > b.f;
	}
};

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent { pDevice, pContext }
{
}

CNavigation::CNavigation(const CNavigation& Prototype) : CComponent ( Prototype ), m_Cells{ Prototype.m_Cells }
#ifdef _DEBUG
,m_pShader{ Prototype.m_pShader }
#endif
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);

#ifdef _DEBUG	
	Safe_AddRef(m_pShader);
#endif

}

HRESULT CNavigation::Initialize_Prototype()
{
#ifdef _DEBUG
	m_vColor = _float4(1.f, 0.f, 0.f, 1.f);
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Engine_Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CNavigation::Initialize_Prototype(const string& pNavigationFilePath)
{
	ifstream file(pNavigationFilePath, ios::binary);
	if (!file.is_open())
	{
		MSG_BOX(TEXT("네비게이션 로드 실패"));
		return E_FAIL;
	}

	vector<SAVE_CELLDATA> Cells;
	_uint iSize = 0;
	file.read(reinterpret_cast<char*>(&iSize), sizeof(_uint));
	Cells.resize(iSize);

	file.read(reinterpret_cast<char*>(Cells.data()), sizeof(SAVE_CELLDATA) * iSize);

	file.close();

	for (_uint i = 0; i < iSize; ++i)
	{

		_float3 CellData[3] = {};
		CellData[0] = Cells[i].Point_A;
		CellData[1] = Cells[i].Point_B;
		CellData[2] = Cells[i].Point_C;

		CCell* pCell = CCell::Create(m_pDevice, m_pContext, CellData, i, Cells[i].iCellType, true);
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
		
#ifdef _DEBUG
		SAVE_CELLDATA SaveCell{};
		SaveCell.iCellType = Cells[i].iCellType;
		SaveCell.Point_A = Cells[i].Point_A;
		SaveCell.Point_B = Cells[i].Point_B;
		SaveCell.Point_C = Cells[i].Point_C;
		m_pCellPos.push_back(SaveCell);
#endif
	}

	SetUp_Neighbors();

#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Engine_Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
#ifdef _DEBUG
	m_vColor = _float4(1.f, 0.f, 0.f, 1.f);
#endif
	if (nullptr == pArg)
		return S_OK;

	NAVIGATION_DESC* pDesc = static_cast<NAVIGATION_DESC*>(pArg);

	// 셀 생성 시, 현재 객체의 셀 인덱스를 가져온다.
	m_iCurrentCellIndex = pDesc->iCurrentCellIndex;

	return S_OK;
}

void CNavigation::Update(_fmatrix WorldMatrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

_bool CNavigation::isMove(_fvector vCulPosition)
{
	//현재 월드 매트릭스를 네비의 로컬로 변경
	_vector vLocalPos = XMVector3TransformCoord(vCulPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	_int iNeighborIndex = { -1 };

	//로컬 셀의 위치 내의 객체가 있으면 트루 
	if (true == m_Cells[m_iCurrentCellIndex]->isIn(vLocalPos, &iNeighborIndex))
		return true;
	
	if (-1 != iNeighborIndex)
	{
		for (_int j = 0; j < 5000; j++)
		{
			if (-1 == iNeighborIndex)
				return false;
			//인접한 셀에 범수의 객체가 있는지 판단 후, 없으면 다음 인접 셀로 넘김
			//있는 경우 셀 인덱스를 반환
			if (true == m_Cells[iNeighborIndex]->isIn(vLocalPos, &iNeighborIndex))
				break;
		}

		m_iCurrentCellIndex = iNeighborIndex;
		return true;
	}

	return false;
}

_bool CNavigation::isOutNormal(_fvector vPosition, _float3& pOut)
{
	return m_Cells[m_iCurrentCellIndex]->isIn(vPosition, pOut);
}

_vector CNavigation::Compute_OnCell(_fvector vPosition)
{
	//객체를 월드의 로컬 위치로 변경
	_vector vLocalPos = XMVector3TransformCoord(vPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	//현재 월드의 높이를 가져온다.
	_float		fHeight = m_Cells[m_iCurrentCellIndex]->Compute_Height(vLocalPos);

	//객체의 월드값을 변경해준다.
	vLocalPos = XMVectorSetY(vLocalPos, fHeight);

	//월드 값으로 변환해서 범수의 월드반환
	return XMVector3TransformCoord(vLocalPos, XMLoadFloat4x4(&m_WorldMatrix));

}
#ifdef _DEBUG

void CNavigation::SetUp_Node(_int TargetIndex)
{
	priority_queue<Node, vector<Node>, CompareNode> SearchList;
	
	Node StartNode{};
	StartNode.CellIndex = m_iCurrentCellIndex;
	StartNode.PreIndex = -1;
	StartNode.g = 0;
	StartNode.vCenter = m_Cells[m_iCurrentCellIndex]->Get_Center();
	StartNode.h = m_Cells[m_iCurrentCellIndex]->Distance(XMLoadFloat3(&m_Cells[TargetIndex]->Get_Center()));
	StartNode.f = StartNode.g + StartNode.h;

	//시작 노드를 리스트에 넣는다.
	SearchList.push(StartNode);

	Node FinalNode{};

	while (!SearchList.empty())
	{
		//우선 순위가 제일 높은 노드를 꺼낸다.
		Node Current = SearchList.top();
		SearchList.pop();

		//인덱스가 목표 인덱스면 반복 종료
		if (Current.CellIndex == TargetIndex)
		{
			FinalNode = Current;
			break;
		}

		Node Temp{};
		//검색한 인덱스인지 체크
		if (FindNodeInClosed(Current.CellIndex, Temp))
		{
			continue;
		}
		
		m_CompleteList.push_back(Current);

		//인접셀 검사
		for (_int i = 0; i < ENUM_CLASS(CELL_LINE::END); ++i)
		{
			_int NeighborIndex = m_Cells[Current.CellIndex]->Get_NeighborIndeices(i);
			if (NeighborIndex < 0)
				continue;

			//인접범수셀 중 검사한 곳이 있으면 넘긴다.
			if (FindNodeInClosed(NeighborIndex, Temp))
				continue;

			Node NeighborNode{};
			NeighborNode.CellIndex = NeighborIndex;
			NeighborNode.PreIndex = Current.CellIndex;
			NeighborNode.vCenter = m_Cells[NeighborIndex]->Get_Center();
			NeighborNode.g = Current.g + m_Cells[NeighborIndex]->Distance(XMLoadFloat3(&Current.vCenter));
			NeighborNode.h = m_Cells[NeighborIndex]->Distance(XMLoadFloat3(&m_Cells[TargetIndex]->Get_Center()));
			NeighborNode.f = NeighborNode.g + NeighborNode.h;

			SearchList.push(NeighborNode);
		}
	}

	while (true)
	{
		//현재의 마지막 노드 셀 인덱스 저장
		m_NodePath.push_back(FinalNode.CellIndex);
		Node parentNode{};

		//이전 노드 인덱스가 -1이 아니면 이전 노드를 찾아서 저장
		if (FinalNode.PreIndex != -1)
		{
			if (!FindNodeInClosed(FinalNode.PreIndex, parentNode))
				continue;

			FinalNode = parentNode;
		}
		else
			break;
	}

	//벡터 뒤집기
	reverse(m_NodePath.begin(), m_NodePath.end());

	while (!SearchList.empty())
	{
		SearchList.pop();
	}
	
	m_CompleteList.clear();
}

_bool CNavigation::SetUp_Portal(vector<Portal>& PortalPath)
{
	Portal vPortal{};
	for (_int i = 1; i < m_NodePath.size(); ++i)
	{
		if (!m_Cells[i]->Compute_Portal(i, vPortal))
			return false;

		PortalPath.push_back(vPortal);
	}
	
	for (auto& portal : PortalPath)
	{
		_float3 vCenter{};

		XMStoreFloat3(&vCenter, (XMLoadFloat3(&portal.vRight) + XMLoadFloat3(&portal.vLeft)) * 0.5f);
		m_Portal.push_back(vCenter);
		_vector vDir = XMVector3Normalize(XMLoadFloat3(&portal.vRight) - XMLoadFloat3(&portal.vLeft));

		_vector vNormal = XMVectorSet(-XMVectorGetZ(vDir), 0.f, XMVectorGetX(vDir), 0.f);


		_vector vOffset = vNormal * 5.f;

		XMStoreFloat3(&portal.vLeft, XMLoadFloat3(&portal.vLeft) + vOffset);
		XMStoreFloat3(&portal.vRight, XMLoadFloat3(&portal.vRight) + vOffset);

	}
	return true;
}

_vector CNavigation::IsNaviMove(_vector vPos)
{
	if (XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_NaviPath[m_iNaviMoveIndex]) - vPos)) <= 5.5f)
		++m_iNaviMoveIndex;

	_vector vDir = XMLoadFloat3(&m_NaviPath[m_iNaviMoveIndex]) - vPos;

	return XMLoadFloat3(&m_NaviPath[m_iNaviMoveIndex]);
	//return XMVector3Normalize(vDir);
}

_vector CNavigation::IsNaviNode()
{
	if (m_iCurrentCellIndex == m_NodePath[m_iNaviMoveIndex])
		m_iNaviMoveIndex++;


	return XMLoadFloat3(&m_Cells[m_NodePath[m_iNaviMoveIndex]]->Get_Center());
}

_vector CNavigation::IsNaviPortal(_vector vPos)
{
	if (XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_Portal[m_iNaviMoveIndex]) - vPos)) <= 0.1f)
		++m_iNaviMoveIndex;


	return XMLoadFloat3(&m_Portal[m_iNaviMoveIndex]);
}

void CNavigation::Search_MovePos(_int TargetIndex)
{
	auto Cross2D = [](_vector a, _vector b) {
		return XMVectorGetX(a) * XMVectorGetZ(b) - XMVectorGetZ(a) * XMVectorGetX(b);};

	m_iNaviMoveIndex = 0;
	SetUp_Node(TargetIndex);
	
	vector<Portal> PortalPath;
	if (!SetUp_Portal(PortalPath))
		return;

	vector<_float3> Path;

	_vector apex = XMLoadFloat3(&m_Cells[m_NodePath.front()]->Get_Center());
	_vector vLeft = XMLoadFloat3(&PortalPath[0].vLeft);
	_vector vRight = XMLoadFloat3(&PortalPath[0].vRight);

	Path.push_back(m_Cells[m_NodePath.front()]->Get_Center());

	_float3 vPos{};
	for (int i = 1; i < PortalPath.size(); i++) {
		_vector newvLeft = XMLoadFloat3(&PortalPath[i].vLeft);
		_vector newvRight = XMLoadFloat3(&PortalPath[i].vRight);

		// 왼쪽 벽 체크
		if (Cross2D(newvLeft - apex, vLeft - apex) >= 0.f)
		{
			if (Cross2D(newvLeft - apex, vRight - apex) > 0.f)
			{
				apex = vRight;
				XMStoreFloat3(&vPos, apex);
				Path.push_back(vPos);
				vLeft = apex;
				vRight = newvRight;
				continue;
			}
			vLeft = newvLeft;
		}

		// 오른쪽 벽 체크
		if (Cross2D(newvRight - apex, vRight - apex) <= 0.f) {
			if (Cross2D(newvRight - apex, vLeft - apex) < 0.f) {
				apex = vLeft;
				XMStoreFloat3(&vPos, apex);
				Path.push_back(vPos);
				vRight = apex;
				vLeft = newvLeft;
				continue;
			}
			vRight = newvRight;
		}
	}

	Path.push_back(m_Cells[m_NodePath.back()]->Get_Center());
	m_NaviPath = Path;
}

HRESULT CNavigation::Add_Cell(const _float3* pPos, _uint iCellType)
{	
	_float3	vPoints[ENUM_CLASS(CELL_POINT::END)] = {};
	memcpy(vPoints, pPos, sizeof(_float3) * ENUM_CLASS(CELL_POINT::END));


	_vector vCamPos = XMLoadFloat4(m_pGameInstance->Get_CamPosition());
	_vector vUp = vCamPos - XMLoadFloat3(&vPoints[ENUM_CLASS(CELL_POINT::A)]);
	_vector vAB = XMLoadFloat3(&vPoints[ENUM_CLASS(CELL_POINT::B)]) - XMLoadFloat3(&vPoints[ENUM_CLASS(CELL_POINT::A)]);
	_vector vAC = XMLoadFloat3(&vPoints[ENUM_CLASS(CELL_POINT::C)]) - XMLoadFloat3(&vPoints[ENUM_CLASS(CELL_POINT::A)]);
	_vector vCross = XMVector3Cross(vUp, vAB);

	_float fDot = XMVectorGetX(XMVector3Dot(vCross, vAC));

	if (fDot < 0)
	{
		_float3 vB = vPoints[ENUM_CLASS(CELL_POINT::B)];
		vPoints[ENUM_CLASS(CELL_POINT::B)] = vPoints[ENUM_CLASS(CELL_POINT::C)];
		vPoints[ENUM_CLASS(CELL_POINT::C)] = vB;
	}

	CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, (_uint)m_Cells.size(), iCellType, false);

	if (pCell == nullptr)
	{
		MSG_BOX(TEXT("Failed to Add Cell"));
		Safe_Release(pCell);
	}
	m_Cells.push_back(pCell);

	SAVE_CELLDATA CellPos{};
	CellPos.Point_A = vPoints[0];
	CellPos.Point_B = vPoints[1];
	CellPos.Point_C = vPoints[2];
	CellPos.iCellType = iCellType;

	m_pCellPos.push_back(CellPos);
	return S_OK;
}

HRESULT CNavigation::Render()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	_float4x4	WorldMatrix = m_WorldMatrix;

	if (-1 == m_iCurrentCellIndex)
	{
		if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
			return E_FAIL;

		m_pShader->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4));

		m_pShader->Begin(0);

		for (auto& pCell : m_Cells)
			pCell->Render();
	}

	else
	{
		_float4 vColor = _float4(1.f, 0.f, 0.f, 1.f);

		WorldMatrix._42 += 0.1f;

		if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
			return E_FAIL;

		m_pShader->Bind_RawValue("g_vColor", &vColor, sizeof(_float4));

		m_pShader->Begin(0);

		m_Cells[m_iCurrentCellIndex]->Render();
	}
	 
	return S_OK;
}
HRESULT CNavigation::Save_Cell(string szFilename)
{
	string FilePath = "../Bin/Data/NavigationData/" + szFilename + ".dat";

	ofstream out(FilePath, ios::binary);

	if (!out)
	{
		MSG_BOX(TEXT("Save 실패"));
		return E_FAIL;
	}

	_uint	iSize = (_uint)m_pCellPos.size();
	out.write(reinterpret_cast<const _char*>(&iSize), sizeof(_uint));
	out.write(reinterpret_cast<const _char*>(m_pCellPos.data()), sizeof(SAVE_CELLDATA) * iSize);

	out.close();
	MSG_BOX(TEXT("Save"));

	return S_OK;
}
_bool CNavigation::IsSnap(_float3& vPos, _float Radius)
{
	for (auto Cell : m_Cells)
	{
		if (Cell->IsSnap(vPos, Radius))
		{

		}
	}
	return false;
}
void CNavigation::Chage_Color(_float4 vColor)
{
	m_vColor = vColor;
}
_int CNavigation::Selete_CellIndex(CTransform& pTransformCom)
{
	_float fDis = D3D11_FLOAT32_MAX;
	_int iIndex = {-1};
	m_pGameInstance->TransformToLocalSpace(pTransformCom);
	for (auto Cell : m_Cells)
	{
		Cell->IsPick(fDis, iIndex);
	}

	return iIndex;
}
void CNavigation::Erase_LastCell()
{
	if (m_Cells.size() == 0)
		return;

	m_Cells.erase(m_Cells.end() - 1);
	m_pCellPos.erase(m_pCellPos.end() - 1);
}
#endif

void CNavigation::SetUp_Neighbors()
{
	//현재 셀의 이웃을 셋팅해준다.
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CELL_POINT::A), pSourCell->Get_Point(CELL_POINT::B)))
			{
				pSourCell->Set_Neighbor(CELL_LINE::AB, pDestCell);
			}
			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CELL_POINT::B), pSourCell->Get_Point(CELL_POINT::C)))
			{
				pSourCell->Set_Neighbor(CELL_LINE::BC, pDestCell);
			}
			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CELL_POINT::C), pSourCell->Get_Point(CELL_POINT::A)))
			{
				pSourCell->Set_Neighbor(CELL_LINE::CA, pDestCell);
			}
		}
	}
}

_bool CNavigation::FindNodeInClosed(_int cellIndex, Node& pOut)
{
	for (auto Node : m_CompleteList)
	{
		if (Node.CellIndex == cellIndex)
		{
			pOut = Node;
			return true;
		}
	}
	return false;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& pNavigationFilePath)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pNavigationFilePath)))
	{
		MSG_BOX(TEXT("Failed to Created : CNavigation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CNavigation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CNavigation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();

#ifdef _DEBUG	
	Safe_Release(m_pShader);
#endif
}


  