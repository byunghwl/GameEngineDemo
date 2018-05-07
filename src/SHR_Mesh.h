/***********************************************************************
                           SH_MESH.h                                 
************************************************************************/

#pragma once
//////////////////////////////////////////////////////////////////////////
// ���漱��
class SHR_MeshList;  // [2014.11.27 ]

/************************************************************************
                     �������̽� IRenderer                                      
************************************************************************/
class IRenderer
{
	
public:

	virtual void						Draw( LPDIRECT3DDEVICE9 _device) PURE;

};


/************************************************************************
                        �������̽�   IUpdater                                     
************************************************************************/
class IUpdater
{
public:

	virtual void						Update(	CAnimator& m_Animator , D3DXMATRIX* pMoveTM = nullptr) PURE;

};







/***********************************************************************
							Struct Mesh_Info  
						�θ��� TM �̳�, �޽��� ���TM , �� 

************************************************************************/

struct Mesh_Info
{
	////////////////////////////////////////////////////////////////////////// NODE_TM
	int								MeshType;
	CHAR							Name[MAX_LEN_NAME];
	CHAR							ParentName[MAX_LEN_NAME];
	bool							HaveMesh;//���ؽ��� ������ �ֳ�?
	bool							isSkinned;		// ��Ű�׵Ǵ� �޽��ΰ�????
	bool							isBone;			// �����?

	D3DXMATRIX						NodeTM,  NodeTM_Pos,  NodeTM_Scale,  NodeTM_Rot;
	D3DXVECTOR3						RotAxis;
	float							RotAngle;
	D3DXVECTOR3						Pos;
	D3DXVECTOR3						Scale, ScaleAxis;
	float							ScaleAngle;

	D3DXMATRIX						BoneIniMat; // �� �̴ϼ� ��Ʈ���� =  bone_NodeTM * skin_NodeTM �� �����

	D3DXMATRIX						LTM_scale,   LTM_Rot,	 LTM_Pos;  //���� ScaleTM, ���� RotTM, ���� PosTM;

	D3DXVECTOR3						BoundingBox_Min, BoundingBox_Max; // [2014.11.11 ]
	float							BoundShpere_Radius; //  ���ۿ�����Ʈ�� ��豸��� ������ �־���� ������  Scene���� ����Ѵ�.  [2014.11.11 ]
	
	Mesh_Info&  operator=(const Mesh_Info& rhs);

	Mesh_Info()
	{ 
		memset(Name,NULL,sizeof(Name));	
		memset(ParentName,NULL,sizeof(ParentName));	
	}
	~Mesh_Info()
	{

	}
};


/************************************************************************/
/*			������׸��� ������� ���׸��� ���� ���ؽ����۸� 
				������������ ���� Ŭ����
*/
/************************************************************************/
class FaceSet
{
public:// {�ӽ�}
	
	D3DMATERIAL9				m_Material;  // ������ ����

	// �ܺο��� ��ϵ� ������

	SHR_D3D9Texture*			m_pTexDiff;//��ǻ�� ��   [2014.10.29 ]
	SHR_D3D9Texture*			m_pTexSpecularMap;//����ŧ���� [2014.10.29 ]
	SHR_D3D9Texture*			m_pTexBumpMap;//����ŧ���� [2014.10.29 ]
	SHR_D3D9Texture*			m_pTexEmissiveMap;//����ŧ���� [2014.10.29 ]

	SHR_VtxBuff*				m_pVtxBuff;// ���ؽ� ����
	SHR_IdxBuff*				m_pIdxBuf;// �ε��� ����

public:

	FaceSet();
	~FaceSet(){};

public:

	void						Draw(LPDIRECT3DDEVICE9 _device);

};


//---------------------------------------------------------------------------------------------
// �ִϸ��̼��� ���� ����ü ����
//---------------------------------------------------------------------------------------------
struct CONTROL_Pos 
{
	float Curtick; D3DXVECTOR3 pos; 
	CONTROL_Pos(){Curtick = 0.0f; }

};
struct CONTROL_Rot 
{
	float Curtick; D3DXVECTOR4 rot; D3DXQUATERNION quat; 
	CONTROL_Rot(){Curtick = 0.0f; }
};
struct CONTROL_Scale
{
	float Curtick; D3DXVECTOR3 scale; D3DXVECTOR3 scale_Axis; float angle; D3DXQUATERNION quat; 
	CONTROL_Scale(){Curtick = 0.0f; }
};



/***********************************************************************
                   struct ANI_INFO
				  �� SHR_Mesh�� ������ �ִ� 
				  �ִϸ��̼� ����
************************************************************************/
struct ANI_INFO
{
	bool						Animateable;
	int							LastFrame;
	int 						FirstFrame;
	int							FrameSpeed;
	int							Ticks_Per_Frame;
	int							FrameCnt; // �� �����Ӱ����� ��ΰ�
	int							FirstTick;  //SCENE_TICKSPERFRAME * SCENE_FIRSTFRAME 
	int							LastTick;  //SCENE_TICKSPERFRAME * SCENE_LASTFRAME 
	int							TickPerFrame; //�����Ӵ� ƽ
	float						LastTime;	
	int							PosSize, RotSize, ScaleSize;
	CONTROL_Pos*				pPos;
	CONTROL_Rot*				pRot;
	CONTROL_Scale*				pScale;


	ANI_INFO() : pPos(nullptr), pRot(nullptr), pScale(nullptr)
	{}


	~ANI_INFO()
	{
//		int a=0;
// 		for(int i = 0 ; i< PosSize ; i++)
// 		{
	//		delete[] pPos;
// 
// 		}

// 		for(int i = 0 ; i< RotSize ; i++)
// 		{
// 			delete (pPos+i);
// 
// 		}
// 
// 		for(int i = 0 ; i< ScaleSize ; i++)
// 		{
// 			delete (pPos+i);
// 
// 		}
// 		SAFE_DELETE_ARR(pPos)
	//	SAFE_DELETE_ARR(pRot)
	//		SAFE_DELETE_ARR(pScale)
	}

};




/***********************************************************************

                         class SHR_Mesh         

************************************************************************/				
class SHR_Mesh : virtual public IResource, 
				 virtual public IRestoreable,
				 virtual public IRefCounter,
				 virtual public IRenderer,
				 virtual public IUpdater
{

public:
	//////////////////////////////////////////////////////////////////////////
	// �ܺο��� �ҷ��� ����
	//  �޽��� �⺻���� ����, ���TM ���� ���
	Mesh_Info							m_MeshInfo;
	//	���ؽ� ����
	int									m_FaceSetCnt; //������׸��� ����
	FaceSet*							m_pFaceSet;   // ������׸��� ������ŭ ����� �ش�.
	// Ű������ ����
	ANI_INFO							m_AniInfo;   // �ִϸ��̼� Ű������ ����
	SHR_VtxShader*						m_pVtxShader;   // ���ؽ� ���̴�
	SHR_PixShader*						m_pPixShader;   // �ȼ� ���̴�
	
	D3DXHANDLE							m_hTech; //[2014.11.21 ]
	//////////////////////////////////////////////////////////////////////////
	// (�ǽð����Ǵ� ������)
	D3DXMATRIX							m_AniTM_Scale, m_AniTm_Rot,  m_AniTm_Pos;  //Ű������ �ִϸ��̼� �Ҷ� ����TM ��� ����ϱ����� ���
	float								m_CurAniTime; // ���� �ִϸ��̼� ��� �ð�
	SHR_Mesh*							m_pParent;    //�θ� ������
	D3DXMATRIX							m_LocalTM, m_WorldTM;	 // �θ�� ��ŭ  �̵��ߴ���.. ,�θ� ��ı��� �ٰ��� �ڽ��� ���� ���� ���
	// �ִϸ��̼� ���� �Ҷ� ����� ������
	D3DXVECTOR3							m_Blend_PosKey;  //���� ���� Ŭ���� ������ Position Key �� �����س��� ����
	D3DXQUATERNION						m_Blend_RotKey;  //���� ���� Ŭ���� ������ Rotation Key �� �����س��� ����
	D3DXVECTOR3							m_Blend_ScalePosKey;  //���� ���� Ŭ���� ������ Scale  Key �� �����س��� ����
	D3DXQUATERNION						m_Blend_ScaleRotKey;  //���� ���� Ŭ���� ������ Scale Key �� �����س��� ����

public:// ������ �Ҹ���

	SHR_Mesh();
	SHR_Mesh(const WCHAR* _Name , const WCHAR* _Path = nullptr);
	virtual ~SHR_Mesh();

public://�����Լ� 
	// from IUpdater
	virtual void						Update(CAnimator& _Animator ,  D3DXMATRIX* pMoveTM = nullptr);
	// from IRenderer
	virtual void						Draw( LPDIRECT3DDEVICE9 _device); 
	// from IRestoreable
	virtual void						Restore(){}; //device Lost�� ���

public:
	// �޽��� ��Ÿ���϶� ����� ������Ʈ �Լ�, ���ڷ� 
	void								UpdateBone(CAnimator &_Animator , D3DXMATRIX* pMoveTM = nullptr);

public:// ������

	D3DMATRIX							GetLocalTM(){return m_LocalTM;}
	D3DXMATRIX							GetWorldTM() const {return m_WorldTM;}		
	const ANI_INFO&						GetAniInfo() const {return m_AniInfo;}
	SHR_VtxShader*						GetVtxShader(){return m_pVtxShader;}
	SHR_PixShader*						GetPixShader(){return m_pPixShader;}
	
	
	int									SetFXShader(IResource* _pRsc); // [2014.10.25 ]
	int									SetVtxShader(IResource* _pRsc);
	int									SetPixShader(IResource* _pRsc);
	
	void								SetName(const WCHAR* _pName);
	void								Set_m_hTech(const char* _str);

private:// �����Լ�

	void								_Animation(CAnimator &_Animator);
	D3DXMATRIX							_GetCurr_PosTM(CAnimator &_Animator);
	D3DXMATRIX							_GetCurr_RotTM(CAnimator &_Animator);
	D3DXMATRIX							_GetCurr_ScaleTM(CAnimator &_Animator);
	
							

};



/***********************************************************************

                         class SHR_SkinMesh      
						��ȯ���ҽ� ��Ų��޽�

************************************************************************/				
class SHR_SkinMesh : virtual public SHR_Mesh

{

public:

	CHAR								m_pBoneName[Max_Bone][Max_Name_Len];
	D3DXMATRIX*							m_pMatPalette;  //������ ��Ʈ���� �ȷ�Ʈ  
	int									m_BoneCnt;      // ��Ű�׿� ������ ��ġ�� ���� �� ����
	std::vector<SHR_Mesh*>				m_Vec_BoneList; //��Ű�׿� ������ ��ġ�� �޽� ����Ʈ
	

public:// ������ �Ҹ���

	SHR_SkinMesh();
	SHR_SkinMesh(const WCHAR* _Name , const WCHAR* _Path = nullptr);
	virtual ~SHR_SkinMesh();

public://�����Լ� ������
	// from IUpdater
	virtual void						Update( CAnimator& _Animator , D3DXMATRIX* pMoveTM = nullptr);
	// IRenderer
	virtual void						Draw( LPDIRECT3DDEVICE9 _device); 

public:

	virtual void						Restore(){}; //device Lost�� ���

public:// ������

	int									GetBoneCnt(){return m_BoneCnt;}
	std::vector<SHR_Mesh*>*				GetVecBoneList(){return &m_Vec_BoneList;}
	const D3DXMATRIX*					GetMatrixPalette(){return m_pMatPalette;}

public:
	// _Path == nullptr �̸� �̹� m_Path�� ����Ǿ��ִٰ� �����Ѵ�.!!
	int									LoadResource(const WCHAR* _Path = nullptr);
};


/************************************************************************/
/*                       class SHR_BoneList
	��Ų��޽����� ������ ��� �޽�(Bone, Helper, ����޽� ��)��  ��Ƴ���
	���ҽ�,,	 ���������� ���ԵǾ��ִ� �޽��� �̰��� �ѹ��� �� ��Ƶд�.
						[2014.9.20 ]
*/
/************************************************************************/
class SHR_BoneList : virtual public IResource, 
					 virtual public IRefCounter					 
					
{
protected:

	friend class CRscPack_Character;

	std::vector<SHR_Mesh*>				m_Vec_MeshList; // ������ �ִ� ������Ʈ �� ��� �޽� ����Ʈ
	bool								m_bShow;       
	SHR_VtxShader*						m_pVtxShader;   // ���ؽ� ���̴�
	SHR_PixShader*						m_pPixShader;   // �ȼ� ���̴�
	ANI_INFO							m_BL_AniInfo;   // BoneList_Ani_Info
	CAnimator							m_Animator;// ���뿡�� ����� Ÿ�̸�

public:

	SHR_BoneList();
	SHR_BoneList(const WCHAR* _Name, const WCHAR* _Path = nullptr);
	virtual ~SHR_BoneList();

public:// ������ �����ε�

	//SHR_BoneList&						operator=(SHR_BoneList &_rhs);

public:
	
	void								Update(CAnimator& _Animator, D3DXMATRIX* pMoveTM = nullptr);
	
	void								Draw( LPDIRECT3DDEVICE9 _device);
	// _pNewMesh �� m_Vec_MeshList�� �߰������ְ� pParentName �̸����� �θ� ã�� ��������ش�. ����  pParentName�� nullptr �̸� �׳� �߰����Ѵ�. 
	int									AddMesh(SHR_Mesh* _pNewMesh, const WCHAR* pParentName = nullptr);	

	// SHR_MeshList�� �ȿ� �ִ� �޽����� ���鼭 �θ� �ִٸ� ���� �θ�  �߰��Ѵ�. // [2014.11.27 ]
	void								ConnectParent_From_MeshList(SHR_MeshList* _pMeshList);

public:// get.. set

	void								SetParent(SHR_Mesh* _pMesh);// �θ� ������ ã���ֱ�

	std::vector<SHR_Mesh*>*				GetMeshList(){return &m_Vec_MeshList;}	
	CAnimator&							GetAniTimer(){return m_Animator;}
	SHR_Mesh*						FindMeshByName(const CHAR* pszName); // [2014.11.11 ]				

public:
	
	int									LoadResource(const WCHAR* _Path = nullptr);

	//m_Vec_MeshList �ȿ��ִ� ��� �޽��鿡�� _hTech �� �����Ѵ�. [2014.11.21 ]
	void								Set_hTech_AllMesh(const char* _str);

	//�޽� �̸����� m_Vec_MeshList �ȿ��ִ� �޽��� ã�� _hTech�� �������ش�.  [2014.11.21 ]
	HRESULT								Set_hTech_byMeshName(const char* _str, const CHAR* pszName);
};


/************************************************************************/
/*                            class SHR_MeshList
	��Ų��޽����� ������ �޽�(Bone, Helper ��)�� ������ �޽����� ��Ƴ���
	���ҽ�,,	�� ĳ���Ϳ� �ϳ����� ���ȴ�. [2014.11.12 ]

*/
/************************************************************************/
class SHR_MeshList : public SHR_BoneList
{
private:
	


public:

	SHR_MeshList();
	SHR_MeshList(const WCHAR* _Name, const WCHAR* _Path = nullptr);
	virtual ~SHR_MeshList();

public:

	// Desc   : _pBoneList�� ���� �θ� ã�Ƽ� ����,, [2014.11.21 ]
	void								ConnectParent_with_BoneList( SHR_BoneList* _pBoneList);

public:// operator
	SHR_MeshList&						operator=(const SHR_MeshList& _rhs);

};

/*************************************  end  ****************************************/