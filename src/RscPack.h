/************************************************************************/
/*								RscPack.h
							[2014.9.15 ]
*/	
/************************************************************************/
#pragma once

class IRscPack
{
protected:


public:

	IRscPack(){};
	virtual ~IRscPack(){};

public:

	virtual int				Build(const WCHAR* _pPath) PURE;

};

/***********************************************************************
                    class CRscPack_NormalObj           

					// 일반 스킨없는 평범한 메쉬 [2014.11.11 ]
************************************************************************/
 class CRscPack_NormalObj
 {
 public:

	SHR_MeshList*						   m_pMeshList;	// [2014.11.12 ]
	CAnimator							   m_Animator; // [2014.11.14 ]

 public://생성자 소멸자

	 CRscPack_NormalObj(){};
	 virtual ~CRscPack_NormalObj(){};


 public:// 가상함수 재정의 [2014.11.11 ]


	 virtual int							Build(const WCHAR* _pPath); 

 public:// 일반함수
	 void									Draw(IDirect3DDevice9* device);  //[2014.11.12 ]
	 //
	 virtual void							Update(float _dTime,  D3DXMATRIX* TM=nullptr); //[2014.11.14 ]

 };

/************************************************************************/
/*				   class CRscPack_Character
					빌드할때 조립되는 정보
					[2014.11.11 ]
*/
/************************************************************************/

class CRscPack_Character : public IRscPack
{

public:
	enum RscPackType
	{
		eRPT_SkinBody,
		//eRPT_Bones,
		//eRPT_Sword,
		eRPT_Max
	};


	D3DXHANDLE							m_hTech_Skin;// 셰이더, 어떤 테크닉을 쓸거냐? [2014.10.30 ]
	D3DXHANDLE							m_hTech_noSkin;// 스키닝계산 안하는 셰이더 [2014.11.20 ]	

	IResource*							m_pAllRsc[eRPT_Max];
	IRenderer*							m_pRenderer[eRPT_Max]; // 그려져야할 모든 리소스	
	IUpdater*							m_pUpdater[eRPT_Max];  // 업데이트 되어야할 모든 리소스
	
	SHR_SkinMesh*						m_pSkinMesh; //[2014.11.21 ]
	SHR_MeshList						m_MeshList; // 본에 붙어있거나 계층구조를 가지고 있는 메쉬리스트 // [2014.11.20 ]
	SHR_BoneList*						m_pBoneList;// 본이 다른 업데이터보다 먼저업데이트되어야한다.	// 본리스트에 헬퍼오브젝트도 포함한다. (임시방편 [2014.11.11 ])
	CAnimator							m_Animator; // [2014.10.12 ]	
	
public:

	CRscPack_Character();
	virtual ~CRscPack_Character();

	//연산자 오버로딩 [2014.11.17 ]
	CRscPack_Character&					operator=(const CRscPack_Character& _rhs );
public:

	virtual int							Build(const WCHAR* _pPath); 
	// 
	virtual void						Draw(IDirect3DDevice9* device);
	//
	virtual void						Update(float _dTime,  D3DXMATRIX* TM=nullptr);

public:
	// 스키닝에 필요한 메트릭스 팔레트에 메모리만 할당하기
	void								Make_BoneMatPalette(int eRPT);
	//  본리스트를 만든후 스킨드메쉬에 장착한다.
	void								Make_BoneList(int eRPT);  
	
public: // get.. set

	int									SetRsc_to_Renderer(IResource* pRsc , int eRPTNum);

	int									SetRsc_to_Updater(IResource* pRsc , int eRPTNum);
	// _enumVal을 사용해서 스킨메쉬용 리소스를 타입캐스팅해서 반환합니다. 실패시 메세지박스와 Nullptr을 리턴
	SHR_SkinMesh*						GetSkinMesh(int _enumVal);

	// 본리스트용 리소스를 반환합니다. 실패시 메세지박스와 Nullptr을 리턴
	SHR_BoneList*						GetBoneList(){return  m_pBoneList;} // [2014.10.16 ]

	// 메쉬 리스트용 리소스를 반환합니다, 실패시 메세지박스와 Nullptr을 리턴  [2014.11.25 ]
	SHR_MeshList&						GetMeshList(){return  m_MeshList;} //  [2014.11.25 ]

	// _enumVal을 사용해서 메쉬용 리소스를 타입캐스팅해서 반환합니다. 실패시 메세지박스와 Nullptr을 리턴
	SHR_Mesh*							GetMesh(int _enumVal);

	CAnimator&							GetAnimator(){return m_Animator;}

public:

	int									SaveMeta(const WCHAR* _Path);
};