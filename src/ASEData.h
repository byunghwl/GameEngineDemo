/************************************************************************/
/*   2014. 07. 09														*/
/*	 ASEData class = �ֻ��� Ŭ���� 							            */
/************************************************************************/

#pragma once
#define  MAX_LEN_NAME  256



//-----------------------------------------------------------------------------
// �о���� ���ؽ��ε���,  �븻�� , ���׸��� ID�� ��� �����ϱ� ���� ����ü
//-----------------------------------------------------------------------------
struct Temp_idx_Norm_mtID
{
	int idx;
	D3DXVECTOR3 Norm;
	int mtrl_ID;
	bool check; // ������׸��� �ű涧 üũ�Ұ�

	Temp_idx_Norm_mtID() : check(false)
	{}

};

//---------------------------------------------------------------------------------------------
// Scene ����ü  
// �ִϸ��̼� ������ ������ �ִ�.
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
// geomObject �ȿ��ִ� ���� ���׸��� ID�� ������ �ִ� �ﰢ������ ����
// ���� ���׸��� ID �� ������ �ִ� ���ؽ����ۿ� �ε��� ���۸� ��� ���� �ɾ������.
// d3d��¿� �°� ASE ������ ������ ������ ,,geomObject �ȿ� �������� ���� �� �ִ�.

class FACE_Set {

public:
	//�ӽ� ���ؽ� �����
	MY_VERTEX				*ptemp_Vtx;
	MY_VERTEX_SKINED		*ptemp_Vtx_Skin; 
	WORD					*ptemp_Idx;
	Temp_idx_Norm_mtID		*ptemp_INMID;
	int						TempINMIDCnt; //�̰԰� �ε��� ����


	//���⼭���ʹ� �ӽ� �ƴ�
	int							 Mtrl_ID;	// ���׸��� ���̵�
	LPDIRECT3DTEXTURE9			 m_pTex;      //���̷�Ʈx�� �ؽ��� 
	D3DMATERIAL9				 m_Material;  // ������ ����
	void*						 m_pVtxBufPointer;
	void*						 m_pIdxBufPointer;
	LPDIRECT3DVERTEXBUFFER9  	 vertexbuffer;
	LPDIRECT3DINDEXBUFFER9		 indexbuffer;
	int							 numfaces;    /*���� ID�� ���������� ���������ش�.*/
	int						     numvertex;
	WCHAR						 pTexPath[Max_Path_Len];// �ؽ��� �о�
	WCHAR						 pTexName[Max_Name_Len];// �ؽ��� �о�

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
//	�ļ����� �о���� MATERIAL ����

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
// ���׸���
//-----------------------------------------------------------------------------
struct MATERIAL
{	
	int					_mtrl_ID;
	int					_SubMtrlCnt;	 //,������׸��� ī��Ʈ
	TCHAR				*pClass;			// *MATERIAL_CLASS    ex) "Standard", "Multi/Sub-Object"
	D3DXCOLOR			_Ambient,  _Diffuse, _Specular;   
	float				_Shine, _Shine_Strength; //����ŧ���� ���δ�
	float				_Transparency;		//*MATERIAL_TRANSPARENCY   ,d3d���� alpha
	float				_Self_illum;	// emissive 
	Mtrl_MAP			_Diffuse_Map;	//��ǻ���  , *MAP_DIFFUSE          

	CGrowableArray<MATERIAL*>  _GA_SubMtrl;// ������׸���

	 MATERIAL() : _SubMtrlCnt(0), pClass(NULL), _Shine(0),_Shine_Strength(0),_Transparency(0),_Self_illum(0)
	 { }

	~MATERIAL()
	{ 
		SAFE_DELETE_ARR(pClass)
		for(int i=0 ; i<_GA_SubMtrl.GetSize() ; i++){	MATERIAL* m= _GA_SubMtrl.GetAt(i);  SAFE_DELETE_(m)    }
	}
};

//---------------------------------------------------------------------------------------------
// ���ؽ� ���ؽ� 3���� �׿� �ش��ϴ� *MESH_MTLID �� �����ϱ� ���� ����ü
//---------------------------------------------------------------------------------------------
struct FACE_LIST
{
	int idx_1, idx_2, idx_3;
	int mtrl_ID;          
};

//---------------------------------------------------------------------------------------------
// *BONE_LIST �� ��� �����ϱ� ���� ����ü
//---------------------------------------------------------------------------------------------
struct  BONE_LIST
{
	int		idx;
	CHAR   Name[512];
};
//---------------------------------------------------------------------------------------------
//  NODE TM ����ü
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
// �ִϸ��̼��� ���� ����ü ����
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
	 
	SCENE*		m_pScene;  //�̰� ���̸� TMAnimation ������ ���°���!!

	int			FrameCnt; // �� �����Ӱ����� ��ΰ�
	int			FirstTick;  //SCENE_TICKSPERFRAME * SCENE_FIRSTFRAME 
	int			LastTick;  //SCENE_TICKSPERFRAME * SCENE_LASTFRAME 
	int			TickPerFrame; //�����Ӵ� ƽ
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
// ���ؽ� ����Ʈ���� �����ϱ� ���� ����ü
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
// VWeight_List ,, ���� �ϳ��� �����Ѵ�. 
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

	VWeight_List& operator=(const VWeight_List& rhv)  //���縦���� �����ڿ����ε�
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
//	�ļ����� �о���� GEOMOBJECT ����
//
//*************************************************************************
class ASEMesh;

class GEOMOBJECT
{	
public:
	int					MeshType;
	ASEMesh*			pAseData;
	//////////////////////////////////////////////////////////////////////////
	// �о������ ������� ����
	
	int					Mtrl_Ref;    // ���׸��� ���۷��� ,, *MATERIAL_REF,  �ʱⰪ�� -1
	int					num_vertex, num_faces, num_tvertex, num_tvfaces, num_cvertex, num_cvfaces, num_pIndice, num_pNormalCnt;
	int					num_BoneCnt;
	WORD*				p_indices;	//�ε��������� ����	

	Temp_idx_Norm_mtID* p_I_N_MtID;//���ؽ��ε���, �븻 ���׸�����̵� �������ִ� ����ü
	D3DXVECTOR3			*pVertex, *ptVertex, *pcVertex, *pNormal;
	VWeight_List		*pVertWeightList;
	int*				ptFace;
	UV_COORD*			pUV;
	FACE_LIST*			pFace_List; 
 

	//////////////////////////////////////////////////////////////////////////
	// ĳ���� �׸���  ���Ǳ����� �����ִ� ����
	NODE_TM							NodeTM;  
	bool							m_isSkinned;		// ��Ű�׵Ǵ� �޽��ΰ�????
	bool							m_HaveMesh;			//�޽������� ������ �ֳ�???

	D3DXMATRIX						m_BoneIniMat; // �� �̴ϼ� ��Ʈ���� =  bone_NodeTM * skin_NodeTM �� �����
	D3DXMATRIX*						m_pMatPalette;  //��Ʈ���� �ȷ�Ʈ  //��Ų��޽��� �������־����

	float							m_CurAniTime;// 
	int								m_FaceSetCnt; //������׸��� ����
	FACE_Set*						m_pFaceSet;   //������׸��� ������ŭ ����� �ش�.
	D3DXMATRIX						m_MLocalTM, m_MWorldTM;	 // �θ�� ��ŭ  �̵��ߴ���.. ,�θ� ��ı��� �ٰ��� �ڽ��� ���� ���� ���
	D3DXMATRIX						m_LTM_scale,   m_LTM_Rot,	 m_LTM_Pos;  //���� ScaleTM, ���� RotTM, ���� PosTM;
	D3DXMATRIX						m_AniTM_Scale, m_AniTm_Rot,  m_AniTm_Pos;  //Ű������ �ִϸ��̼� �Ҷ� ����TM ��� ����ϱ����� ���
	D3DXMATRIX						m_ArcBallTM;
	TMAnimation						m_TMAni; // �ִϸ��̼� ����

	GEOMOBJECT*						m_pParent;    //�θ� ������

	BoundingSphere					m_BoundSphere;  //�ٿ�����Ǿ�

	
public:
	// ������
	GEOMOBJECT();
	//�Ҹ���
	~GEOMOBJECT();

public:
	//faceset���� ����� �ִ� �Լ�
	virtual void		      Create_FaceSet(MATERIAL* m, LPDIRECT3DDEVICE9 _pDevice);
	

	virtual void			  Update(float _dTime, D3DXMATRIX MoveTM);
	virtual void			  Draw( LPDIRECT3DDEVICE9 _device);   
	virtual void			  Draw( LPDIRECT3DDEVICE9 _device,  LPDIRECT3DVERTEXSHADER9  _pVShader,  LPDIRECT3DPIXELSHADER9  _pPShader);   
	//�����������ִ� �Լ�
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
/* GeoObject�� ��ӹ���		                                            */
/* GeoObject�� ��Ű�׵Ǵ� �޽��϶� ���                                 */
/************************************************************************/


class SKINNED_OBJECT : public GEOMOBJECT
{	
public:
	// ������
	SKINNED_OBJECT(){MeshType = eMeshType_Skin;}
	//�Ҹ���
	virtual ~SKINNED_OBJECT(){};

public:
	//faceset���� ����� �ִ� �Լ�
	virtual void				 Create_FaceSet(MATERIAL* m, LPDIRECT3DDEVICE9 _pDevice);
	
	virtual void				 Update(float _dTime, D3DXMATRIX MoveTM);
	virtual void				 Draw( LPDIRECT3DDEVICE9 _device);  
	virtual void				 Draw( LPDIRECT3DDEVICE9 _device, LPDIRECT3DVERTEXSHADER9  _pVShader,  LPDIRECT3DPIXELSHADER9  _pPShader);   
	virtual GEOMOBJECT&			 DeepCopy(const GEOMOBJECT& rhs);

public:


public://get.,, set

};



/************************************************************************/
/*     HerperObject  Ŭ���� ����			                             */
/************************************************************************/

class HELPEROBJECT : public GEOMOBJECT //�θ� ���
{

public:

	// ������
	HELPEROBJECT(){MeshType = eMeshType_Helper;};
	//�Ҹ���
	~HELPEROBJECT(){};

public:
	//faceset���� ����� �ִ� �Լ�
	virtual void				 Create_FaceSet(MATERIAL* m, LPDIRECT3DDEVICE9 _pDevice){}
	virtual D3DMATERIAL9		 Make_Material(MATERIAL* m, int id = -1){return DEFAULT_MATERIAL; }

	virtual void				 Update(float _dTime, D3DXMATRIX MoveTM);
	virtual void				 Draw( LPDIRECT3DDEVICE9 _device){} 

public://get.,, set


};

//-----------------------------------------------------------------------------
// SHAPE ������Ʈ�� ���� LINE
//-----------------------------------------------------------------------------
struct SHAPE_LINE
{
	D3DXVECTOR3			*pVert_List;// ������ �����ϴ� ������
	int					 Vert_Cnt;		//���� �� ����
						
	SHAPE_LINE(): pVert_List(NULL), Vert_Cnt(0)
	{}
	~SHAPE_LINE()
	{
		SAFE_DELETE_ARR(pVert_List)
	}
};

//---------------------------------------------------------------------------------------------
// shapeObject Ŭ����
//---------------------------------------------------------------------------------------------

class SHAPEOBJECT : public GEOMOBJECT//���GEOMOBJECT
{
	friend  class	ASEParser;
	
	ID3DXLine			*m_pShape;	   //������ �׷��� ��ü
	int					m_LineCnt;	// ������ ����
	SHAPE_LINE			*pSLine;		//���α���ü , ���ΰ�����ŭ ��������	
	D3DXMATRIX			m_ViewTM;		//ī�޶� ��ȯ���
	D3DXMATRIXA16		m_ProjTM;		//������ȯ ���
	D3DXMATRIX			m_ShapeWorldTM; //shape Object���� ����� �ڱ��ڽ��� WorldTM (�θ𿡰� ProjectionTM�� ViewTM�� ���ԵǾ��ִ°�)


public:

	// ������
	SHAPEOBJECT() : m_pShape(NULL), pSLine(NULL), m_LineCnt(0)
	{MeshType = eMeshType_Shape;}
	//�Ҹ���
	~SHAPEOBJECT()
	{
		SAFE_RELEASE(m_pShape)
		SAFE_DELETE_ARR(pSLine)	
	};

public:
	//faceset���� ����� �ִ� �Լ�
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
//  ASEData   Ŭ���� ����
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

//*******************************************  ��  **********************************************//