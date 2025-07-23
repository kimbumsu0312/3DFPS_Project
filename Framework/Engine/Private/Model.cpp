#include "pch.h"
#include "Model.h"
#include "Bone.h"
#include "Animation.h"
#include "Mesh.h"
#include "MeshMaterial.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent{ pDevice, pContext}
{
}

CModel::CModel(const CModel& Prototype) : CComponent(Prototype), m_Meshes(Prototype.m_Meshes), m_iNumMeshes(Prototype.m_iNumMeshes)
, m_Materials(Prototype.m_Materials), m_iNumMaterials(Prototype.m_iNumMaterials), m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }, m_eModelType(Prototype.m_eModelType)
, m_Bones{ Prototype.m_Bones },m_Animations(Prototype.m_Animations), m_iCurrentAnimIndex(Prototype.m_iCurrentAnimIndex), m_iNumAnimations(Prototype.m_iNumAnimations)
{
    
    for (auto& pBone : m_Bones)
        Safe_AddRef(pBone);

    for (auto& pMesh : m_Meshes)
        Safe_AddRef(pMesh);

    for (auto& pMaterial : m_Materials)
        Safe_AddRef(pMaterial);

    for (auto& pAnim : m_Animations)
        Safe_AddRef(pAnim);
}

HRESULT CModel::Initialize_Prototype(const SAVE_MODEL& pModelData)
{
    m_eModelType = pModelData.eModel;

    m_PreTransformMatrix = pModelData.PreTransformMatrix;

    if (FAILED(Ready_Bones(pModelData)))
        return E_FAIL;

    if (FAILED(Ready_Meshes(pModelData)))
        return E_FAIL;

    if (FAILED(Ready_Materials(pModelData)))
        return E_FAIL;

    if (FAILED(Ready_Animations(pModelData)))
        return E_FAIL;

    return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
    if (FAILED(m_Meshes[iMeshIndex]->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_Meshes[iMeshIndex]->Render()))
        return E_FAIL;

    return S_OK;
}

void CModel::Set_Animation(_uint iIndex)
{
    if (iIndex >= m_iNumAnimations)
        return;

    m_iCurrentAnimIndex = iIndex;
}

HRESULT CModel::Bind_Materials(CShader* pShader, const _char* pConstantName, _uint iMeshIndex, _int iTexIndex, _uint iIndex)
{
    if (iMeshIndex >= m_iNumMeshes)
        return E_FAIL;

    _uint       iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

    if (m_iNumMaterials <= iMaterialIndex)
        return E_FAIL;

    return m_Materials[iMaterialIndex]->Bind_Resources(pShader, pConstantName, iTexIndex, iIndex);
}

HRESULT CModel::Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
    if (iMeshIndex >= m_iNumMeshes)
        return E_FAIL;

    return m_Meshes[iMeshIndex]->Bind_BoneMatrices(pShader, pConstantName, m_Bones);
}

void CModel::Play_Animation(_float fTimeDelta)
{
    if (m_eModelType != MODELTYPE::ANIM)
        return;
    //현재 애니메이션에 뼈 트랜슾폼 매트릭스를 갱신
    m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(m_Bones, fTimeDelta);

    for (auto& pBone : m_Bones)
    {
        //뼈들의 행렬을 부모 뼈의 행렬에 맞게 맞춰준다.
        pBone->Update_CombinedTransformationMatrix(m_PreTransformMatrix, m_Bones);
    }
}

HRESULT CModel::Ready_Meshes(const SAVE_MODEL& pModelData)
{
    m_iNumMeshes = pModelData.iNumMeshes;

    for (size_t i = 0; i < m_iNumMeshes; i++)
    {
        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType, pModelData.Meshs[i], m_Bones, pModelData.Bones);
        if (nullptr == pMesh)
            return E_FAIL;

        m_Meshes.push_back(pMesh);
    }

    return S_OK;
}

HRESULT CModel::Ready_Materials(const SAVE_MODEL& pModelData)
{
    m_iNumMaterials = pModelData.iNumMaterials;

    for (size_t i = 0; i < m_iNumMaterials; i++)
    {

        CMeshMaterial* pMeshMaterial = CMeshMaterial::Create(m_pDevice, m_pContext, pModelData.MeshMaterials[i]);
        if (nullptr == pMeshMaterial)
            return E_FAIL;

        m_Materials.push_back(pMeshMaterial);
    }

    return S_OK;
}

HRESULT CModel::Ready_Bones(const SAVE_MODEL& pModelData)
{
    for (auto& BoneData : pModelData.Bones)
    {
        CBone* pBone = CBone::Create(BoneData);

        if (pBone == nullptr)
            return E_FAIL;

        m_Bones.push_back(pBone);
    }

    return S_OK;
}

HRESULT CModel::Ready_Animations(const SAVE_MODEL& pModelData)
{
    m_iNumAnimations = pModelData.iNumAnimations;;

    for (size_t i = 0; i < m_iNumAnimations; i++)
    {
        CAnimation* pAnimation = CAnimation::Create(pModelData.Animations[i], m_Bones);
        if (nullptr == pAnimation)
            return E_FAIL;

        m_Animations.push_back(pAnimation);
    }

    return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const SAVE_MODEL& pModelData)
{
    CModel* pInstance = new CModel(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pModelData)))
    {
        MSG_BOX(TEXT("Failed to Created : CModel"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
    CModel* pInstance = new CModel(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CModel"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CModel::Free()
{
    __super::Free();

    for (auto& pBone : m_Bones)
        Safe_Release(pBone);
    m_Bones.clear();

    for (auto& pMesh : m_Meshes)
        Safe_Release(pMesh);
    m_Meshes.clear();

    for (auto& pMaterial : m_Materials)
        Safe_Release(pMaterial);
    m_Materials.clear();

    for (auto& pAnim : m_Animations)
        Safe_Release(pAnim);
    m_Animations.clear();
}
