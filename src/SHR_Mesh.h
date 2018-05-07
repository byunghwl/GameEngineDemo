/***********************************************************************
                           SH_MESH.h                                 
************************************************************************/

#pragma once
//////////////////////////////////////////////////////////////////////////
// 전방선언
class SHR_MeshList;  // [2014.11.27 ]

/************************************************************************
                     인터페이스 IRenderer                                      
************************************************************************/
class IRenderer
{
	
public:

	virtual void						Draw( LPDIRECT3DDEVICE9 _device) PURE;

};


/************************************************************************
                        인터페이스   IUpdater                                     
************************************************************************/
class IUpdater
{
public:

	virtual void						Update(	CAnimator& m_Animator , D3DXMATRIX* pMoveTM = nullptr) PURE;

};







/***********************************************************************
							Struct Mesh_Info  
						부모의 TM 이나, 메쉬의 노드TM , 등 

************************************************************************/

struct Mesh_Info
{
	////////////////////////////////////////////////////////////////////////// NODE_TM
	int								MeshType;
	CHAR							Name[MAX_LEN_NAME];
	CHAR							ParentName[MAX_LEN_NAME];
	bool							HaveMesh;//버텍스를 가지고 있나?
	bool							isSkinned;		// 스키닝되는 메쉬인가????
	bool							isBone;			// 뼈대용?

	D3DXMATRIX						NodeTM,  NodeTM_Pos,  NodeTM_Scale,  NodeTM_Rot;
	D3DXVECTOR3						RotAxis;
	float							RotAngle;
	D3DXVECTOR3						Pos;
	D3DXVECTOR3						Scale, ScaleAxis;
	float							ScaleAngle;

	D3DXMATRIX						BoneIniMat; // 본 이니셜 메트릭스 =  bone_NodeTM * skin_NodeTM 의 역행렬

	D3DXMATRIX						LTM_scale,   LTM_Rot,	 LTM_Pos;  //로컬 ScaleTM, 로컬 RotTM, 로컬 PosTM;

	D3DXVECTOR3						BoundingBox_Min, BoundingBox_Max; // [2014.11.11 ]
	float							BoundShpere_Radius; //  헬퍼오브젝트가 경계구라면 가지고 있어야할 반지름  Scene에서 계산한다.  [2014.11.11 ]
	
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
/*			서브메테리얼 있을경우 메테리얼 마다 버텍스버퍼를 
				나눠놓기위해 만든 클래스
*/
/************************************************************************/
class FaceSet
{
public:// {임시}
	
	D3DMATERIAL9				m_Material;  // 적용할 재질

	// 외부에서 등록된 포인터

	SHR_D3D9Texture*			m_pTexDiff;//디퓨즈 맵   [2014.10.29 ]
	SHR_D3D9Texture*			m_pTexSpecularMap;//스페큘러맵 [2014.10.29 ]
	SHR_D3D9Texture*			m_pTexBumpMap;//스페큘러맵 [2014.10.29 ]
	SHR_D3D9Texture*			m_pTexEmissiveMap;//스페큘러맵 [2014.10.29 ]

	SHR_VtxBuff*				m_pVtxBuff;// 버텍스 정보
	SHR_IdxBuff*				m_pIdxBuf;// 인덱스 정보

public:

	FaceSet();
	~FaceSet(){};

public:

	void						Draw(LPDIRECT3DDEVICE9 _device);

};


//---------------------------------------------------------------------------------------------
// 애니메이션을 위한 구조체 선언
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
				  각 SHR_Mesh가 가지고 있는 
				  애니메이션 정보
************************************************************************/
struct ANI_INFO
{
	bool						Animateable;
	int							LastFrame;
	int 						FirstFrame;
	int							FrameSpeed;
	int							Ticks_Per_Frame;
	int							FrameCnt; // 총 프레임개수가 몇개인가
	int							FirstTick;  //SCENE_TICKSPERFRAME * SCENE_FIRSTFRAME 
	int							LastTick;  //SCENE_TICKSPERFRAME * SCENE_LASTFRAME 
	int							TickPerFrame; //프레임당 틱
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
	// 외부에서 불러올 정보
	//  메쉬의 기본적인 정보, 노드TM 정보 등등
	Mesh_Info							m_MeshInfo;
	//	버텍스 정보
	int									m_FaceSetCnt; //서브메테리얼 개수
	FaceSet*							m_pFaceSet;   // 서브메테리얼 개수만큼 만들어 준다.
	// 키프레임 정보
	ANI_INFO							m_AniInfo;   // 애니메이션 키프레임 정보
	SHR_VtxShader*						m_pVtxShader;   // 버텍스 셰이더
	SHR_PixShader*						m_pPixShader;   // 픽셀 셰이더
	
	D3DXHANDLE							m_hTech; //[2014.11.21 ]
	//////////////////////////////////////////////////////////////////////////
	// (실시간계산되는 변수들)
	D3DXMATRIX							m_AniTM_Scale, m_AniTm_Rot,  m_AniTm_Pos;  //키프레임 애니메이션 할때 로컬TM 대신 사용하기위한 행렬
	float								m_CurAniTime; // 현재 애니메이션 재생 시간
	SHR_Mesh*							m_pParent;    //부모 포인터
	D3DXMATRIX							m_LocalTM, m_WorldTM;	 // 부모로 얼만큼  이동했는지.. ,부모 행렬까지 다곱한 자신의 최종 월드 행렬
	// 애니메이션 블랜딩 할때 사용할 변수들
	D3DXVECTOR3							m_Blend_PosKey;  //이전 상태 클립의 마지막 Position Key 를 저장해놓을 변수
	D3DXQUATERNION						m_Blend_RotKey;  //이전 상태 클립의 마지막 Rotation Key 를 저장해놓을 변수
	D3DXVECTOR3							m_Blend_ScalePosKey;  //이전 상태 클립의 마지막 Scale  Key 를 저장해놓을 변수
	D3DXQUATERNION						m_Blend_ScaleRotKey;  //이전 상태 클립의 마지막 Scale Key 를 저장해놓을 변수

public:// 생성자 소멸자

	SHR_Mesh();
	SHR_Mesh(const WCHAR* _Name , const WCHAR* _Path = nullptr);
	virtual ~SHR_Mesh();

public://가상함수 
	// from IUpdater
	virtual void						Update(CAnimator& _Animator ,  D3DXMATRIX* pMoveTM = nullptr);
	// from IRenderer
	virtual void						Draw( LPDIRECT3DDEVICE9 _device); 
	// from IRestoreable
	virtual void						Restore(){}; //device Lost때 사용

public:
	// 메쉬가 본타입일때 사용할 업데이트 함수, 인자로 
	void								UpdateBone(CAnimator &_Animator , D3DXMATRIX* pMoveTM = nullptr);

public:// 접근자

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

private:// 내부함수

	void								_Animation(CAnimator &_Animator);
	D3DXMATRIX							_GetCurr_PosTM(CAnimator &_Animator);
	D3DXMATRIX							_GetCurr_RotTM(CAnimator &_Animator);
	D3DXMATRIX							_GetCurr_ScaleTM(CAnimator &_Animator);
	
							

};



/***********************************************************************

                         class SHR_SkinMesh      
						성환리소스 스킨드메쉬

************************************************************************/				
class SHR_SkinMesh : virtual public SHR_Mesh

{

public:

	CHAR								m_pBoneName[Max_Bone][Max_Name_Len];
	D3DXMATRIX*							m_pMatPalette;  //본들의 메트릭스 팔레트  
	int									m_BoneCnt;      // 스키닝에 영향을 미치는 본의 총 개수
	std::vector<SHR_Mesh*>				m_Vec_BoneList; //스키닝에 영향을 미치는 메쉬 리스트
	

public:// 생성자 소멸자

	SHR_SkinMesh();
	SHR_SkinMesh(const WCHAR* _Name , const WCHAR* _Path = nullptr);
	virtual ~SHR_SkinMesh();

public://가상함수 재정의
	// from IUpdater
	virtual void						Update( CAnimator& _Animator , D3DXMATRIX* pMoveTM = nullptr);
	// IRenderer
	virtual void						Draw( LPDIRECT3DDEVICE9 _device); 

public:

	virtual void						Restore(){}; //device Lost때 사용

public:// 접근자

	int									GetBoneCnt(){return m_BoneCnt;}
	std::vector<SHR_Mesh*>*				GetVecBoneList(){return &m_Vec_BoneList;}
	const D3DXMATRIX*					GetMatrixPalette(){return m_pMatPalette;}

public:
	// _Path == nullptr 이면 이미 m_Path가 저장되어있다고 가정한다.!!
	int									LoadResource(const WCHAR* _Path = nullptr);
};


/************************************************************************/
/*                       class SHR_BoneList
	스킨드메쉬들을 제외한 모든 메쉬(Bone, Helper, 보통메쉬 등)를  모아놓은
	리소스,,	 계층구조가 포함되어있는 메쉬를 이곳에 한번에 다 모아둔다.
						[2014.9.20 ]
*/
/************************************************************************/
class SHR_BoneList : virtual public IResource, 
					 virtual public IRefCounter					 
					
{
protected:

	friend class CRscPack_Character;

	std::vector<SHR_Mesh*>				m_Vec_MeshList; // 가지고 있는 업데이트 할 모든 메쉬 리스트
	bool								m_bShow;       
	SHR_VtxShader*						m_pVtxShader;   // 버텍스 셰이더
	SHR_PixShader*						m_pPixShader;   // 픽셀 셰이더
	ANI_INFO							m_BL_AniInfo;   // BoneList_Ani_Info
	CAnimator							m_Animator;// 뼈대에만 적용될 타이머

public:

	SHR_BoneList();
	SHR_BoneList(const WCHAR* _Name, const WCHAR* _Path = nullptr);
	virtual ~SHR_BoneList();

public:// 연산자 오버로딩

	//SHR_BoneList&						operator=(SHR_BoneList &_rhs);

public:
	
	void								Update(CAnimator& _Animator, D3DXMATRIX* pMoveTM = nullptr);
	
	void								Draw( LPDIRECT3DDEVICE9 _device);
	// _pNewMesh 를 m_Vec_MeshList에 추가시켜주고 pParentName 이름으로 부모를 찾아 연결시켜준다. 만약  pParentName가 nullptr 이면 그냥 추가만한다. 
	int									AddMesh(SHR_Mesh* _pNewMesh, const WCHAR* pParentName = nullptr);	

	// SHR_MeshList의 안에 있는 메쉬들을 돌면서 부모가 있다면 본의 부모를  추가한다. // [2014.11.27 ]
	void								ConnectParent_From_MeshList(SHR_MeshList* _pMeshList);

public:// get.. set

	void								SetParent(SHR_Mesh* _pMesh);// 부모 포인터 찾아주기

	std::vector<SHR_Mesh*>*				GetMeshList(){return &m_Vec_MeshList;}	
	CAnimator&							GetAniTimer(){return m_Animator;}
	SHR_Mesh*						FindMeshByName(const CHAR* pszName); // [2014.11.11 ]				

public:
	
	int									LoadResource(const WCHAR* _Path = nullptr);

	//m_Vec_MeshList 안에있는 모든 메쉬들에게 _hTech 를 장착한다. [2014.11.21 ]
	void								Set_hTech_AllMesh(const char* _str);

	//메쉬 이름으로 m_Vec_MeshList 안에있는 메쉬를 찾아 _hTech를 설정해준다.  [2014.11.21 ]
	HRESULT								Set_hTech_byMeshName(const char* _str, const CHAR* pszName);
};


/************************************************************************/
/*                            class SHR_MeshList
	스킨드메쉬들을 제외한 메쉬(Bone, Helper 등)을 포함한 메쉬들을 모아놓은
	리소스,,	한 캐릭터에 하나씩만 사용된다. [2014.11.12 ]

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

	// Desc   : _pBoneList로 부터 부모 찾아서 연결,, [2014.11.21 ]
	void								ConnectParent_with_BoneList( SHR_BoneList* _pBoneList);

public:// operator
	SHR_MeshList&						operator=(const SHR_MeshList& _rhs);

};

/*************************************  end  ****************************************/