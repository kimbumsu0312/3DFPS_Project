#ifndef Engine_Struct_h__
#define Engine_Struct_h__



namespace Engine
{
	typedef struct tagEngineDesc
	{
		HINSTANCE		hInst;
		HWND			hWnd;
		WINMODE			eWinMode;
		unsigned int	iWinSizeX, iWinSizeY;
		unsigned int	iNumLevels;
	}ENGINE_DESC;	

	typedef struct tagLightDesc
	{
		enum TYPE {DIRECTIONAL, POINT, END};

		TYPE		eType;
		XMFLOAT4	vDiffuse;
		XMFLOAT4	vAmbient;
		XMFLOAT4	vSpecular;

		XMFLOAT4	vDirection;
		XMFLOAT4	vPosition;
		float		fRange;

	}LIGHT_DESC;

	//쉐이더 관련 구조체
	typedef struct tagVertexPositionTexcoord
	{
		XMFLOAT3	vPosition;
		XMFLOAT2	vTexcoord;

		//상수값으로 사용 및 전역으로 생성해서 추가 메모리 할당을 받지 않게 처리
		static const unsigned int iNumElements = { 2 };
		//컴파일 타임에 상수 값을 계산하는 함수
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	}VTXPOSTEX;

	typedef struct tagVertexPositionNormalTexcoord
	{
		XMFLOAT3	vPosition;
		XMFLOAT3	vNormal;
		XMFLOAT2	vTexcoord;

		static const unsigned int iNumElements = { 3 };
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	}VTXNORTEX;

	typedef struct tagVertexMesh
	{
		XMFLOAT3	vPosition;
		XMFLOAT3	vNormal;
		XMFLOAT3	vTangent;
		XMFLOAT3	vBinormal;
		XMFLOAT2	vTexcoord;

		static const unsigned int iNumElements = { 5 };
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	}VTXMESH;

	//저장, 불러오기 관련 구조체
	typedef struct tagSaveTerrain
	{
		unsigned int			iNumverticesX;
		unsigned int			iNumverticesZ;
		vector<XMFLOAT3>		pVertexData;
		vector<XMFLOAT2>		pTexcoordData;
	}SAVE_TERRAIN;

	typedef struct tagSaveMaterial
	{
		unsigned int			iTexCount;
		vector<string>			szFullPath;

	}SAVE_MATERIAL;

	typedef struct tagSaveMeshMaterial
	{
		vector<SAVE_MATERIAL>	Materials;
	}SAVE_MESHMATERIAL;

	struct Face { unsigned int iIndices[3]; };
	
	typedef struct tagSaveMesh
	{
		unsigned int			iMaterialIndex;
		unsigned int			iNumVertices;
		unsigned int			iVertexStride;
		unsigned int			iNumIndices;
		unsigned int			iNumFaces;
		vector<Face>			iFaces;

		vector<XMFLOAT3>		vPosition;
		vector<XMFLOAT3>		vNormal;
		vector<XMFLOAT3>		vTangent;
		vector<XMFLOAT3>		vBinormal;
		vector<XMFLOAT2>		vTexcoord;
	}SAVE_MESH;

	typedef struct tagSaveModel
	{
		MODELTYPE					eModel;
		unsigned int				iNumMeshes;
		vector<SAVE_MESH>			Meshs;

		unsigned int				iNumMaterials;
		vector<SAVE_MESHMATERIAL>	MeshMaterials;
	}SAVE_MODEL;

}


#endif // Engine_Struct_h__
