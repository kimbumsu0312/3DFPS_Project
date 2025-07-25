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

	//���̴� ���� ����ü
	typedef struct tagVertexPositionTexcoord
	{
		XMFLOAT3	vPosition;
		XMFLOAT2	vTexcoord;

		//��������� ��� �� �������� �����ؼ� �߰� �޸� �Ҵ��� ���� �ʰ� ó��
		static const unsigned int iNumElements = { 2 };
		//������ Ÿ�ӿ� ��� ���� ����ϴ� �Լ�
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
		/* �������� ����Ǿ��� ������ �ε���*/
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

	//����, �ҷ����� ���� ����ü
	//�ͷ��� ����
	typedef struct tagSaveTerrain
	{
		unsigned int			iNumverticesX;
		unsigned int			iNumverticesZ;
		vector<XMFLOAT3>		pVertexPositions;
		vector<XMFLOAT2>		pVertexTex;

	}SAVE_TERRAIN;

	//������ ��ġ�Ǵ� ������Ʈ ����
	typedef struct tagObjcetData
	{
		wstring					szModel_Path = {};
		wstring					szObject_Path;
		XMMATRIX				objmat;

	}OBJCET_DATA;

	//���׸��� ����
	typedef struct tagSaveMaterial
	{
		unsigned int			iTexCount;
		vector<string>			szFullPath;

	}SAVE_MATERIAL;

	//���׸������ ��� �ִ� ��ü
	typedef struct tagSaveMeshMaterial
	{
		vector<SAVE_MATERIAL>	Materials;
	}SAVE_MESHMATERIAL;

	//�Ž� ����
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

	//�� ���� ����
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

	//�ִϸ��̼� ���� ����
	typedef struct tagSaveAnimData
	{
		string			szAnimName;
		unsigned int	iStartFrame;
		unsigned int	iEndFrame;
		float			fTickPerSecond;

	}SAVE_ANIMDATA;
}

#endif // Engine_Struct_h__
