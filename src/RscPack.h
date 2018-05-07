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

					// �Ϲ� ��Ų���� ����� �޽� [2014.11.11 ]
************************************************************************/
 class CRscPack_NormalObj
 {
 public:

	SHR_MeshList*						   m_pMeshList;	// [2014.11.12 ]
	CAnimator							   m_Animator; // [2014.11.14 ]

 public://������ �Ҹ���

	 CRscPack_NormalObj(){};
	 virtual ~CRscPack_NormalObj(){};


 public:// �����Լ� ������ [2014.11.11 ]


	 virtual int							Build(const WCHAR* _pPath); 

 public:// �Ϲ��Լ�
	 void									Draw(IDirect3DDevice9* device);  //[2014.11.12 ]
	 //
	 virtual void							Update(float _dTime,  D3DXMATRIX* TM=nullptr); //[2014.11.14 ]

 };

/************************************************************************/
/*				   class CRscPack_Character
					�����Ҷ� �����Ǵ� ����
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


	D3DXHANDLE							m_hTech_Skin;// ���̴�, � ��ũ���� ���ų�? [2014.10.30 ]
	D3DXHANDLE							m_hTech_noSkin;// ��Ű�װ�� ���ϴ� ���̴� [2014.11.20 ]	

	IResource*							m_pAllRsc[eRPT_Max];
	IRenderer*							m_pRenderer[eRPT_Max]; // �׷������� ��� ���ҽ�	
	IUpdater*							m_pUpdater[eRPT_Max];  // ������Ʈ �Ǿ���� ��� ���ҽ�
	
	SHR_SkinMesh*						m_pSkinMesh; //[2014.11.21 ]
	SHR_MeshList						m_MeshList; // ���� �پ��ְų� ���������� ������ �ִ� �޽�����Ʈ // [2014.11.20 ]
	SHR_BoneList*						m_pBoneList;// ���� �ٸ� �������ͺ��� ����������Ʈ�Ǿ���Ѵ�.	// ������Ʈ�� ���ۿ�����Ʈ�� �����Ѵ�. (�ӽù��� [2014.11.11 ])
	CAnimator							m_Animator; // [2014.10.12 ]	
	
public:

	CRscPack_Character();
	virtual ~CRscPack_Character();

	//������ �����ε� [2014.11.17 ]
	CRscPack_Character&					operator=(const CRscPack_Character& _rhs );
public:

	virtual int							Build(const WCHAR* _pPath); 
	// 
	virtual void						Draw(IDirect3DDevice9* device);
	//
	virtual void						Update(float _dTime,  D3DXMATRIX* TM=nullptr);

public:
	// ��Ű�׿� �ʿ��� ��Ʈ���� �ȷ�Ʈ�� �޸𸮸� �Ҵ��ϱ�
	void								Make_BoneMatPalette(int eRPT);
	//  ������Ʈ�� ������ ��Ų��޽��� �����Ѵ�.
	void								Make_BoneList(int eRPT);  
	
public: // get.. set

	int									SetRsc_to_Renderer(IResource* pRsc , int eRPTNum);

	int									SetRsc_to_Updater(IResource* pRsc , int eRPTNum);
	// _enumVal�� ����ؼ� ��Ų�޽��� ���ҽ��� Ÿ��ĳ�����ؼ� ��ȯ�մϴ�. ���н� �޼����ڽ��� Nullptr�� ����
	SHR_SkinMesh*						GetSkinMesh(int _enumVal);

	// ������Ʈ�� ���ҽ��� ��ȯ�մϴ�. ���н� �޼����ڽ��� Nullptr�� ����
	SHR_BoneList*						GetBoneList(){return  m_pBoneList;} // [2014.10.16 ]

	// �޽� ����Ʈ�� ���ҽ��� ��ȯ�մϴ�, ���н� �޼����ڽ��� Nullptr�� ����  [2014.11.25 ]
	SHR_MeshList&						GetMeshList(){return  m_MeshList;} //  [2014.11.25 ]

	// _enumVal�� ����ؼ� �޽��� ���ҽ��� Ÿ��ĳ�����ؼ� ��ȯ�մϴ�. ���н� �޼����ڽ��� Nullptr�� ����
	SHR_Mesh*							GetMesh(int _enumVal);

	CAnimator&							GetAnimator(){return m_Animator;}

public:

	int									SaveMeta(const WCHAR* _Path);
};