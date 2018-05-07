/************************************************************************/
/*   2014. 07. 09														*/
/*	 ASEData class = 최상위 클래스 							            */
/************************************************************************/

#pragma once
#define  MAX_LEN_NAME  256



//-----------------------------------------------------------------------------
// 읽어들인 버텍스인덱스,  노말과 , 메테리얼 ID를 잠시 저장하기 위한 구조체
//-----------------------------------------------------------------------------
struct Temp_idx_Norm_mtID
{
	int idx;
	D3DXVECTOR3 Norm;
	int mtrl_ID;
	bool check; // 서브메테리얼 옮길때 체크할값

	Temp_idx_Norm_mtID() : check(false)
	{}

};

//---------------------------------------------------------------------------------------------
// Scene 구조체  
// 애니메이션 정보를 가지고 있다.
//---------------------------------------------------------------------------------------------
struct SCENE
{	
	int				LastFrame;
	int 			FirstFrame;
	int				FrameSpeed;
	int				Ticks_Per_Frame;

};


//////////////////////////////////////////////////////////////////////////
//
struct UV_COORD
{
	float _u,_v;
};

//////////////////////////////////////////////////////////////////////////
// geomObject 안에있는 같은 메테리얼 ID를 가지고 있는 삼각형들의 집합
// 같은 메테리얼 ID 를 가지고 있는 버텍스버퍼와 인덱스 버퍼를 묶어서 락을 걸어버린다.
// d3d출력에 맞게 ASE 정보를 가공한 데이터 ,,geomObject 안에 여러개가 있을 수 있다.

class FACE_Set {

public:
	//임시 버텍스 저장소
	MY_VERTEX				*ptemp_Vtx;
	MY_VERTEX_SKINED		*ptemp_Vtx_Skin; 
	WORD					*ptemp_Idx;
	Temp_idx_Norm_mtID		*ptemp_INMID;
	int						TempINMIDCnt; //이게곧 인덱스 개수


	//여기서부터는 임시 아님
	int							 Mtrl_ID;	// 메테리얼 아이디
	LPDIRECT3DTEXTURE9			 m_pTex;      //다이렉트x용 텍스쳐 
	D3DMATERIAL9				 m_Material;  // 적용할 재질
	void*						 m_pVtxBufPointer;
	void*						 m_pIdxBufPointer;
	LPDIRECT3DVERTEXBUFFER9  	 vertexbuffer;
	LPDIRECT3DINDEXBUFFER9		 indexbuffer;
	int							 numfaces;    /*같은 ID를 만날때마다 증가시켜준다.*/
	int						     numvertex;
	WCHAR						 pTexPath[Max_Path_Len];// 텍스쳐 패쓰
	WCHAR						 pTexName[Max_Name_Len];// 텍스쳐 패쓰

	FACE_Set() : Mtrl_ID(-1),  numvertex(0), numfaces(0),TempINMIDCnt(0),
				vertexbuffer(NULL), indexbuffer(NULL), m_pTex(NULL), ptemp_Vtx(nullptr), ptemp_Vtx_Skin(nullptr), ptemp_Idx(nullptr)
		      
	{}

	~FACE_Set();

public: 

	void     Draw(LPDIRECT3DDEVICE9 _device);
	void     Draw(LPDIRECT3DDEVICE9 _device, LPDIRECT3DVERTEXSHADER9  _pVShader,  LPDIRECT3DPIXELSHADER9  _pPShader);
	void     DrawSkinning(LPDIRECT3DDEVICE9 _device);
	void     DrawSkinning(LPDIRECT3DDEVICE9 _device , LPDIRECT3DVERTEXSHADER9  _pVShader,  LPDIRECT3DPIXELSHADER9  _pPShader);
};


//////////////////////////////////////////////////////////////////////////
//	파서에서 읽어들인 MATERIAL 정보

struct Mtrl_MAP
{
	TCHAR		*pClass;		//	*MAP_CLASS "Bitmap"	
	TCHAR		*pBitmapName;		// *BITMAP 
	float		u_Offset, v_Offset, u_Tiling, v_Tiling;
	Mtrl_MAP() : pClass(NULL), pBitmapName(NULL), u_Offset(0), v_Offset(0), u_Tiling(0), v_Tiling(0)
	{	}

	~Mtrl_MAP()
	{
		SAFE_DELETE_ARR(pClass )
		SAFE_DELETE_ARR(pBitmapName)
	}
};


//-----------------------------------------------------------------------------
// 메테리얼
//-----------------------------------------------------------------------------
struct MATERIAL
{	
	int					_mtrl_ID;
	int					_SubMtrlCnt;	 //,서브메테리얼 카운트
	TCHAR				*pClass;			// *MATERIAL_CLASS    ex) "Standard", "Multi/Sub-Object"
	D3DXCOLOR			_Ambient,  _Diffuse, _Specular;   
	float				_Shine, _Shine_Strength; //스페큘러에 쓰인다
	float				_Transparency;		//*MATERIAL_TRANSPARENCY   ,d3d에서 alpha
	float				_Self_illum;	// emissive 
	Mtrl_MAP			_Diffuse_Map;	//디퓨즈맵  , *MAP_DIFFUSE          

	CGrowableArray<MATERIAL*>  _GA_SubMtrl;// 서브메테리얼

	 MATERIAL() : _SubMtrlCnt(0), pClass(NULL), _Shine(0),_Shine_Strength(0),_Transparency(0),_Self_illum(0)
	 { }

	~MATERIAL()
	{ 
		SAFE_DELETE_ARR(pClass)
		for(int i=0 ; i<_GA_SubMtrl.GetSize() ; i++){	MATERIAL* m= _GA_SubMtrl.GetAt(i);  SAFE_DELETE_(m)    }
	}
};

//---------------------------------------------------------------------------------------------
// 버텍스 인텍스 3개와 그에 해당하는 *MESH_MTLID 를 저장하기 위한 구조체
//---------------------------------------------------------------------------------------------
struct FACE_LIST
{
	int idx_1, idx_2, idx_3;
	int mtrl_ID;          
};

//---------------------------------------------------------------------------------------------
// *BONE_LIST 를 잠시 저장하기 위한 구조체
//---------------------------------------------------------------------------------------------
struct  BONE_LIST
{
	int		idx;
	CHAR   Name[512];
};
//---------------------------------------------------------------------------------------------
//  NODE TM 구조체
//---------------------------------------------------------------------------------------------

struct NODE_TM
{
	CHAR			Name[MAX_LEN_NAME];
	CHAR			ParentName[MAX_LEN_NAME];
	D3DXMATRIX		NodeTM,  NodeTM_Pos,  NodeTM_Scale,  NodeTM_Rot;
	D3DXVECTOR3		RotAxis;
	float			RotAngle;
	D3DXVECTOR3		Pos;
	D3DXVECTOR3		Scale, ScaleAxis;
	float			ScaleAngle;
	
	NODE_TM&  operator=(const NODE_TM& rhs)
	{
		strcpy_s(Name, rhs.Name);	
		strcpy_s(ParentName, rhs.ParentName);	
		NodeTM = rhs.NodeTM; NodeTM_Pos = rhs.NodeTM_Pos ; NodeTM_Scale = rhs.NodeTM_Scale; NodeTM_Rot = NodeTM_Rot;
		RotAxis = rhs.RotAxis;
		RotAngle = RotAngle; Pos = rhs.Pos; Scale = rhs.Scale; ScaleAxis = rhs.ScaleAxis;
		ScaleAngle = rhs.ScaleAngle;

		return *this;
	}


	NODE_TM()
	{ 
		memset(Name,NULL,sizeof(Name));	
		memset(ParentName,NULL,sizeof(ParentName));	
	}
	~NODE_TM()
	{

	}
};

//---------------------------------------------------------------------------------------------
// 애니메이션을 위한 구조체 선언
//---------------------------------------------------------------------------------------------

struct CONTROL_POS 
{
	float Curtick; D3DXVECTOR3 pos; 
	CONTROL_POS(){Curtick = 0.0f; }

};
struct CONTROL_ROT 
{
	float Curtick; D3DXVECTOR4 rot; D3DXQUATERNION quat; 
	CONTROL_ROT(){Curtick = 0.0f; }
};
struct CONTROL_SCALE
{
	float Curtick; D3DXVECTOR3 scale; D3DXVECTOR3 scale_Axis; float angle; D3DXQUATERNION quat; 
	CONTROL_SCALE(){Curtick = 0.0f; }
};

struct TMAnimation {
	 
	SCENE*		m_pScene;  //이게 널이면 TMAnimation 정보가 없는것임!!

	int			FrameCnt; // 총 프레임개수가 몇개인가
	int			FirstTick;  //SCENE_TICKSPERFRAME * SCENE_FIRSTFRAME 
	int			LastTick;  //SCENE_TICKSPERFRAME * SCENE_LASTFRAME 
	int			TickPerFrame; //프레임당 틱
	float		LastTime;	
	int			PosSize, RotSize, ScaleSize;
	CONTROL_POS*		pPos;
	CONTROL_ROT*		pRot;
	CONTROL_SCALE*		pScale;

	bool	isFirstPosKeyNZero;
	bool	isFirstRotKeyNZero;
	bool	isFirstScaleKeyNZero;

	TMAnimation() :FrameCnt(0), pPos(NULL), pRot(NULL), pScale(NULL), m_pScene(NULL), isFirstPosKeyNZero(false),
				   isFirstRotKeyNZero(false), isFirstScaleKeyNZero(false)
	{ pPos = NULL; pRot = NULL;  pScale = NULL;
	}

	~TMAnimation()
	{
		SAFE_DELETE_ARR(pPos)
		SAFE_DELETE_ARR(pRot)
		SAFE_DELETE_ARR(pScale)
	}

};
//-----------------------------------------------------------------------------
// 버텍스 웨이트값을 저장하기 위한 구조체
//-----------------------------------------------------------------------------
class Vertex_Weight
{
public:

	int			Boneidx;
	float		Weight;

	bool operator<(Vertex_Weight& rhv)
	{ 
		return (Weight < rhv.Weight); 
	}
};




//-----------------------------------------------------------------------------
// VWeight_List ,, 정점 하나와 대응한다. 
//-----------------------------------------------------------------------------
#define MAX_VERTEX_WEIGHT 20

class VWeight_List
{
public:

	int				m_size;
	Vertex_Weight   m_VertexW[MAX_VERTEX_WEIGHT]; 

public:

	VWeight_List() : m_size(0) 
	{
		memset(m_VertexW, 0 , sizeof(Vertex_Weight)*MAX_VERTEX_WEIGHT);
	}
	~VWeight_List(){};

	VWeight_List& operator=(const VWeight_List& rhv)  //복사를위한 연산자오버로딩
	{
		memcpy(m_VertexW,  rhv.m_VertexW,  sizeof(Vertex_Weight)*MAX_VERTEX_WEIGHT);
		m_size = rhv.m_size;
		return *this;
	}

public:
	
	void		Sort_By_Weight();
	bool		insert(const Vertex_Weight& _Val );

public:
	int			GetSize() const { return m_size;}

};





//*************************************************************************
//
//	파서에서 읽어들인 GEOMOBJECT 정보
//
//*************************************************************************
class ASEMesh;

class GEOMOBJECT
{	
public:
	int					MeshType;
	ASEMesh*			pAseData;
	//////////////////////////////////////////////////////////////////////////
	// 읽어들인후 사라지는 정보
	
	int					Mtrl_Ref;    // 메테리얼 레퍼런스 ,, *MATERIAL_REF,  초기값은 -1
	int					num_vertex, num_faces, num_tvertex, num_tvfaces, num_cvertex, num_cvfaces, num_pIndice, num_pNormalCnt;
	int					num_BoneCnt;
	WORD*				p_indices;	//인덱스저장할 공간	

	Temp_idx_Norm_mtID* p_I_N_MtID;//버텍스인덱스, 노말 메테리얼아이디를 가지고있는 구조체
	D3DXVECTOR3			*pVertex, *ptVertex, *pcVertex, *pNormal;
	VWeight_List		*pVertWeightList;
	int*				ptFace;
	UV_COORD*			pUV;
	FACE_LIST*			pFace_List; 
 

	//////////////////////////////////////////////////////////////////////////
	// 캐릭터 그릴때  사용되기위해 남이있는 정보
	NODE_TM							NodeTM;  
	bool							m_isSkinned;		// 스키닝되는 메쉬인가????
	bool							m_HaveMesh;			//메쉬정보를 가지고 있나???

	D3DXMATRIX						m_BoneIniMat; // 본 이니셜 메트릭스 =  bone_NodeTM * skin_NodeTM 의 역행렬
	D3DXMATRIX*						m_pMatPalette;  //메트릭스 팔레트  //스킨드메쉬가 가지고있어야함

	float							m_CurAniTime;// 
	int								m_FaceSetCnt; //서브메테리얼 개수
	FACE_Set*						m_pFaceSet;   //서브메테리얼 개수만큼 만들어 준다.
	D3DXMATRIX						m_MLocalTM, m_MWorldTM;	 // 부모로 얼만큼  이동했는지.. ,부모 행렬까지 다곱한 자신의 최종 월드 행렬
	D3DXMATRIX						m_LTM_scale,   m_LTM_Rot,	 m_LTM_Pos;  //로컬 ScaleTM, 로컬 RotTM, 로컬 PosTM;
	D3DXMATRIX						m_AniTM_Scale, m_AniTm_Rot,  m_AniTm_Pos;  //키프레임 애니메이션 할때 로컬TM 대신 사용하기위한 행렬
	D3DXMATRIX						m_ArcBallTM;
	TMAnimation						m_TMAni; // 애니메이션 정보

	GEOMOBJECT*						m_pParent;    //부모 포인터

	BoundingSphere					m_BoundSphere;  //바운딩스피어

	
public:
	// 생성자
	GEOMOBJECT();
	//소멸자
	~GEOMOBJECT();

public:
	//faceset정보 만들어 주는 함수
	virtual void		      Create_FaceSet(MATERIAL* m, LPDIRECT3DDEVICE9 _pDevice);
	

	virtual void			  Update(float _dTime, D3DXMATRIX MoveTM);
	virtual void			  Draw( LPDIRECT3DDEVICE9 _device);   
	virtual void			  Draw( LPDIRECT3DDEVICE9 _device,  LPDIRECT3DVERTEXSHADER9  _pVShader,  LPDIRECT3DPIXELSHADER9  _pPShader);   
	//깊은복사해주는 함수
	virtual GEOMOBJECT&	      DeepCopy(const GEOMOBJECT& rhs);

public://get.,, set

	D3DMATRIX				 getLocalTM(){return m_MLocalTM;}
	D3DXMATRIX				 getWorldTM() const {return m_MWorldTM;}					 

public:

	void					 Animation(float _dTime);
	D3DMATERIAL9			 Make_Material(MATERIAL* m, int id = -1);

	D3DXMATRIX				 GetCurr_PosTM(float _CurTick);
	D3DXMATRIX				 GetCurr_RotTM(float _CurTick);
	D3DXMATRIX				 GetCurr_ScaleTM(float _CurTick);


	const BoundingSphere*    GetBoundingSphere() const {return &m_BoundSphere;}
	
};


/************************************************************************/
/*  class SKINNED_OBJECT                                                */
/* GeoObject를 상속받음		                                            */
/* GeoObject가 스키닝되는 메쉬일때 사용                                 */
/************************************************************************/


class SKINNED_OBJECT : public GEOMOBJECT
{	
public:
	// 생성자
	SKINNED_OBJECT(){MeshType = eMeshType_Skin;}
	//소멸자
	virtual ~SKINNED_OBJECT(){};

public:
	//faceset정보 만들어 주는 함수
	virtual void				 Create_FaceSet(MATERIAL* m, LPDIRECT3DDEVICE9 _pDevice);
	
	virtual void				 Update(float _dTime, D3DXMATRIX MoveTM);
	virtual void				 Draw( LPDIRECT3DDEVICE9 _device);  
	virtual void				 Draw( LPDIRECT3DDEVICE9 _device, LPDIRECT3DVERTEXSHADER9  _pVShader,  LPDIRECT3DPIXELSHADER9  _pPShader);   
	virtual GEOMOBJECT&			 DeepCopy(const GEOMOBJECT& rhs);

public:


public://get.,, set

};



/************************************************************************/
/*     HerperObject  클래스 선언			                             */
/************************************************************************/

class HELPEROBJECT : public GEOMOBJECT //부모 상속
{

public:

	// 생성자
	HELPEROBJECT(){MeshType = eMeshType_Helper;};
	//소멸자
	~HELPEROBJECT(){};

public:
	//faceset정보 만들어 주는 함수
	virtual void				 Create_FaceSet(MATERIAL* m, LPDIRECT3DDEVICE9 _pDevice){}
	virtual D3DMATERIAL9		 Make_Material(MATERIAL* m, int id = -1){return DEFAULT_MATERIAL; }

	virtual void				 Update(float _dTime, D3DXMATRIX MoveTM);
	virtual void				 Draw( LPDIRECT3DDEVICE9 _device){} 

public://get.,, set


};

//-----------------------------------------------------------------------------
// SHAPE 오브젝트에 쓰일 LINE
//-----------------------------------------------------------------------------
struct SHAPE_LINE
{
	D3DXVECTOR3			*pVert_List;// 라인을 구성하는 정점들
	int					 Vert_Cnt;		//정점 총 개수
						
	SHAPE_LINE(): pVert_List(NULL), Vert_Cnt(0)
	{}
	~SHAPE_LINE()
	{
		SAFE_DELETE_ARR(pVert_List)
	}
};

//---------------------------------------------------------------------------------------------
// shapeObject 클래스
//---------------------------------------------------------------------------------------------

class SHAPEOBJECT : public GEOMOBJECT//상속GEOMOBJECT
{
	friend  class	ASEParser;
	
	ID3DXLine			*m_pShape;	   //라인을 그려줄 객체
	int					m_LineCnt;	// 라인총 개수
	SHAPE_LINE			*pSLine;		//라인구조체 , 라인개수만큼 동적생성	
	D3DXMATRIX			m_ViewTM;		//카메라 변환행렬
	D3DXMATRIXA16		m_ProjTM;		//투영변환 행렬
	D3DXMATRIX			m_ShapeWorldTM; //shape Object에만 적용될 자기자신의 WorldTM (부모에게 ProjectionTM과 ViewTM이 포함되어있는값)


public:

	// 생성자
	SHAPEOBJECT() : m_pShape(NULL), pSLine(NULL), m_LineCnt(0)
	{MeshType = eMeshType_Shape;}
	//소멸자
	~SHAPEOBJECT()
	{
		SAFE_RELEASE(m_pShape)
		SAFE_DELETE_ARR(pSLine)	
	};

public:
	//faceset정보 만들어 주는 함수
	virtual void				 Create_FaceSet(MATERIAL* m, LPDIRECT3DDEVICE9 _pDevice);
	virtual D3DMATERIAL9		 Make_Material(MATERIAL* m, int id = -1){return DEFAULT_MATERIAL; }

	virtual void				 Update(float _dTime, D3DXMATRIX MoveTM);
	virtual void				 Draw( LPDIRECT3DDEVICE9 _device);

private:

	void						 Create_Line(LPDIRECT3DDEVICE9 _pDevice);

public:
	//get.,, set
	void						SetLineCnt(int _Val){ m_LineCnt = _Val;}			
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  ASEData   클래스 선언
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class ASEData 
{
	friend class ASEParser;
	friend class GEOMOBJECT;
	friend class SKINNED_OBJECT;

private:
	
	SCENE							 m_Scene;
	
public:

	ASEData(){};
	virtual ~ASEData(){};

	
	virtual 	void    Update(float _dTime,  D3DXMATRIX MoveTM) = 0;


public://get,, set
	
public:


};

#include "ASECameraData.h"
#include "ASEMesh.h"

//*******************************************  끝  **********************************************//