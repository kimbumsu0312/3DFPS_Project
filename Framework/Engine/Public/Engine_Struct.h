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

	typedef struct tagKeyFrame
	{
		XMFLOAT3	vScale;
		XMFLOAT4	vRotation;
		XMFLOAT3	vTranslation;

		float		fTrackPosition;
	}KEYFRAME;

	typedef struct tagAnimFrame
	{
		unsigned int	iStartFrame;
		unsigned int	iEndFrame;
		float			fTickPerSecond;

	}ANIMEFRAME;

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

	typedef struct tagVertexAnimMesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT3		vTangent;
		XMFLOAT3		vBinormal;
		/* 정점에게 적용되야할 뼈들의 인덱스*/
		XMUINT4			vBlendIndex;
		XMFLOAT4		vBlendWeight;
		XMFLOAT2		vTexcoord;

		static const unsigned int	iNumElements = { 7 };
		static constexpr D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 80, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	}VTXANIMMESH;

	//저장, 불러오기 관련 구조체
	//터레인 저장
	typedef struct tagSaveTerrain
	{
		unsigned int			iNumverticesX;
		unsigned int			iNumverticesZ;
		vector<XMFLOAT3>		pVertexPositions;
		vector<XMFLOAT2>		pVertexTex;

	}SAVE_TERRAIN;

	//레벨에 배치되는 오브젝트 저장
	typedef struct tagObjcetData
	{
		wstring					szModel_Path = {};
		wstring					szObject_Path;
		XMMATRIX				objmat;

	}OBJCET_DATA;

	//메테리얼 저장
	typedef struct tagSaveMaterial
	{
		unsigned int			iTexCount;
		vector<string>			szFullPath;

	}SAVE_MATERIAL;

	//메테리얼들을 담고 있는 객체
	typedef struct tagSaveMeshMaterial
	{
		vector<SAVE_MATERIAL>	Materials;
	}SAVE_MESHMATERIAL;

	//매쉬 저장
	struct Face { unsigned int iIndices[3]; };
	
	typedef struct tagSaveMesh
	{
		wstring					szName;
		unsigned int			iMaterialIndex;
		unsigned int			iNumVertices;
		unsigned int			iVertexStride;
		unsigned int			iNumIndices;
		unsigned int			iNumFaces;
		vector<Face>			iFaces;

		vector<VTXMESH>			NonAnimVertex;
		vector<VTXANIMMESH>		AnimVertex;

		unsigned int			iNumBones;
		vector<int>				BoneIndices;

	}SAVE_MESH;

	typedef struct tagSaveBone
	{
		wstring					szName;
		XMFLOAT4X4				TransformationMatrix;
		int						iParentBoneIndex;
		XMFLOAT4X4				matOffset;				
	}SAVE_BONE;

	typedef struct tagChannels
	{
		unsigned int			iBoneIndex;
		unsigned int			iNumKeyFrames;
		vector<KEYFRAME>		KeyFrames;

	}SAVE_CHANNEL;

	typedef struct tagSaveAnimation
	{
		unsigned int			iNumChannels;
		float					fDuration;
		float					fTickPerSecond;

		vector<SAVE_CHANNEL>	Channels;
	}SAVE_ANIM;

	//모델 정보 저장
	typedef struct tagSaveModel
	{
		string						szName;
		MODELTYPE					eModel;
		XMFLOAT4X4					PreTransformMatrix;

		unsigned int				iNumMaterials;
		vector<SAVE_MESHMATERIAL>	MeshMaterials;

		unsigned int				iNumBone;
		vector<SAVE_BONE>			Bones;

		unsigned int				iNumMeshes;
		vector<SAVE_MESH>			Meshs;

		unsigned int				iNumAnimations;
		vector<SAVE_ANIM>			Animations;

	}SAVE_MODEL;

	//애니메이션 정보 저장
	typedef struct tagSaveAnimData
	{
		string			szAnimName;
		unsigned int	iStartFrame;
		unsigned int	iEndFrame;
		float			fTickPerSecond;

	}SAVE_ANIMDATA;
}

#endif // Engine_Struct_h__
