#pragma  once


//���ҽ� Ÿ��
enum eRscType
{
	eRT_TEXTURE,  /*=0*/
	eRT_MESH_NORMAL,
	eRT_SKIN_MESH,
	eRT_BONE_LIST,
	eRT_VTXBUFF,
	eRT_IDXBUFF,
	eRT_SOUND,
	eRT_VTXSHADER,
	eRT_PIXSHADER,/*=8*/
	eRT_FX_SHADER,
	eRT_MESH_LIST,
	eRT_MAX

};

class IResource;
class IRestoreable;
/************************************************************************/
/*							class CResourceMgr
*/
/************************************************************************/

class CResourceMgr
{
	friend class IResource; //��� ���ҽ��� �ֻ��� �θ� �������̽� 
	
protected:

	std::list<IResource*>					m_List_IResource;
	std::list<IRestoreable*>				m_List_Restoreable;
	static CResourceMgr*					m_pRscMgr;//�̱���								    ;
public:
	//�̱��� �ν��Ͻ� ��ȯ
	static CResourceMgr*					getInstance(); 

public:

	CResourceMgr();
	virtual ~CResourceMgr();

public:

	// �ش��ϴ� _Name�� ���ҽ��� ����Ʈ�� ��ϵǾ����� ������ Path�� �̿��� �ش�Ÿ�Կ� ���� �ε����� ����Ʈ�� �����, ���� IResource* ��ȯ
	// �̹� �ε�Ǿ� �ִٸ� �����޼��� �ڽ� ����� nullptr ��ȯ
	IResource*								LoadResource(const WCHAR* _Path, WCHAR* _Name, int _RscType);
	// ���� ���ҽ���(���� ��ĳ���Ϳ��� ����ϴ� ���ҽ����� ����)�� ���ǵǾ��ִ� ��Ÿ���Ϸ� ���� ���ҽ��� �о���δ�.
	int										LoadResources_from_Metafile(const CHAR* _pPath);
	// ����ڰ� ���� ���ҽ��� ����Ʈ�� ������ִ� �Լ�
	int										RegisterRsc(IResource* _pRsc);
	//�̸����� �ش縮�ҽ� ã�Ƽ� ��ȯ
	IResource*								Find_Resource(const WCHAR* _pName);
	// ��� ���ҽ��� ���¸� eRS_TOBE_DISCARD �� �����.
	void									ChangeStateDiscard();
	// eRS_TOBE_DISCARD������ ��� ���ҽ��� �����Ѵ�.
	void									Discard();
	// ��縮�ҽ��� �����Ѵ�.
	void									Release(); 
private:// �����Լ�
	// �ش��ϴ� _Name�� ���ҽ��� ����Ʈ�� ��ϵǾ��ִ��� Ȯ���ϰ�	������ _Path�� �̿��� �ش�Ÿ�Կ� ���� �ε����� ����Ʈ�� �����
	// ���� �ٸ������� �ٲ� ����Ѵ�.
	IResource*								_LoadResource(const WCHAR* _Path, WCHAR* _Name, int _RscType);


	friend class SHR_Mesh;
	friend class SHR_SkinMesh;
	friend class SHR_BoneList;
	friend class SHR_D3D9Texture;

};


//////////////////////////////////////////////////////////////////////////
// ���⼭���� ���ҽ� �������̽� 


/************************************************************************/
/*						class IResource
			��� ���ҽ����� �ݵ�� �� �������̽��� ��ӹ޾ƾ� ��
							[2014.8.27 ]
*/
/************************************************************************/
class IResource
{
	friend class CResourceMgr;
	friend class CRscPack_Character;

protected:

	static LPDIRECT3DDEVICE9			m_pd3dDevice ;		  // Our rendering device

	int									eResourceState;       // ���ҽ� ���� 
	int									eResourceType;		  // ���ҽ� Ÿ��
	DWORD								m_NameSize;           // sizeof(WCHAR)*wcslen(m_Name)
	DWORD								m_PathSize;			  // sizeof(WCHAR)*wcslen(m_Path)
	WCHAR								m_Name[Max_Name_Len]; // resource name; //���ҽ� ���� �ĺ��� ����
	WCHAR								m_Path[Max_Path_Len]; // resource Location 
	

public:

	IResource();
	virtual ~IResource();
	static void							SetDevice(LPDIRECT3DDEVICE9 _pd3dDevice){m_pd3dDevice = _pd3dDevice; }
	
public: 

	void								SetPathSize(int _val){m_PathSize = _val;}
	void								SetPath(const WCHAR* _path){wcscpy_s(m_Path, _path);}

};


/************************************************************************/
/*								class IRefCounter
					���ҽ��� ���۷��� ī������ ���� �������̽�
								[2014.9.3 ]
*/
/************************************************************************/
class IRefCounter
{
protected:
	//���۷��� ī��Ʈ
	int							m_RefCount;

public:
	//������
	IRefCounter();
	//~�Ҹ���
	virtual ~IRefCounter();
public:

	void						AddRef();
	void						Release();

};


/************************************************************************/
/*						class IRestorable						
			����̽��ҽǽ� ���ҽ��� �����ϱ����� �������̽�
						[2014.8.27 ]
*/
/************************************************************************/
class IRestoreable
{

private:

	
protected:

	virtual void					Restore() PURE; //device Lost�� ���

public:
	// ������
	IRestoreable(){ };
	virtual ~IRestoreable(){};


};
//////////////////////////////////////////////////////////////////////////

#include "Resource.h"



/****************************************************** End *****************************************************/