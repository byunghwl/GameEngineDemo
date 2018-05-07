#pragma  once


//리소스 타입
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
	friend class IResource; //모든 리소스의 최상위 부모 인터페이스 
	
protected:

	std::list<IResource*>					m_List_IResource;
	std::list<IRestoreable*>				m_List_Restoreable;
	static CResourceMgr*					m_pRscMgr;//싱글톤								    ;
public:
	//싱글톤 인스턴스 반환
	static CResourceMgr*					getInstance(); 

public:

	CResourceMgr();
	virtual ~CResourceMgr();

public:

	// 해당하는 _Name의 리소스가 리스트에 등록되어있지 않으면 Path를 이용해 해당타입에 따라 로드한후 리스트에 등록함, 그후 IResource* 반환
	// 이미 로드되어 있다면 에러메세지 박스 출력후 nullptr 반환
	IResource*								LoadResource(const WCHAR* _Path, WCHAR* _Name, int _RscType);
	// 사용될 리소스들(보통 한캐릭터에서 사용하는 리소스들의 집합)이 정의되어있는 메타파일로 부터 리소스를 읽어들인다.
	int										LoadResources_from_Metafile(const CHAR* _pPath);
	// 사용자가 만든 리소스를 리스트에 등록해주는 함수
	int										RegisterRsc(IResource* _pRsc);
	//이름으로 해당리소스 찾아서 반환
	IResource*								Find_Resource(const WCHAR* _pName);
	// 모든 리소스의 상태를 eRS_TOBE_DISCARD 로 만든다.
	void									ChangeStateDiscard();
	// eRS_TOBE_DISCARD상태인 모든 리소스를 제거한다.
	void									Discard();
	// 모든리소스를 제거한다.
	void									Release(); 
private:// 내부함수
	// 해당하는 _Name의 리소스가 리스트에 등록되어있는지 확인하고	없으면 _Path를 이용해 해당타입에 따라 로드한후 리스트에 등록함
	// 씬이 다른씬으로 바뀔때 사용한다.
	IResource*								_LoadResource(const WCHAR* _Path, WCHAR* _Name, int _RscType);


	friend class SHR_Mesh;
	friend class SHR_SkinMesh;
	friend class SHR_BoneList;
	friend class SHR_D3D9Texture;

};


//////////////////////////////////////////////////////////////////////////
// 여기서부터 리소스 인터페이스 


/************************************************************************/
/*						class IResource
			모든 리소스들은 반드시 이 인터페이스를 상속받아야 함
							[2014.8.27 ]
*/
/************************************************************************/
class IResource
{
	friend class CResourceMgr;
	friend class CRscPack_Character;

protected:

	static LPDIRECT3DDEVICE9			m_pd3dDevice ;		  // Our rendering device

	int									eResourceState;       // 리소스 상태 
	int									eResourceType;		  // 리소스 타입
	DWORD								m_NameSize;           // sizeof(WCHAR)*wcslen(m_Name)
	DWORD								m_PathSize;			  // sizeof(WCHAR)*wcslen(m_Path)
	WCHAR								m_Name[Max_Name_Len]; // resource name; //리소스 고유 식별자 역할
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
					리소스의 레퍼런스 카운팅을 위한 인터페이스
								[2014.9.3 ]
*/
/************************************************************************/
class IRefCounter
{
protected:
	//레퍼런스 카운트
	int							m_RefCount;

public:
	//생성자
	IRefCounter();
	//~소멸자
	virtual ~IRefCounter();
public:

	void						AddRef();
	void						Release();

};


/************************************************************************/
/*						class IRestorable						
			디바이스소실시 리소스를 리셋하기위한 인터페이스
						[2014.8.27 ]
*/
/************************************************************************/
class IRestoreable
{

private:

	
protected:

	virtual void					Restore() PURE; //device Lost때 사용

public:
	// 생성자
	IRestoreable(){ };
	virtual ~IRestoreable(){};


};
//////////////////////////////////////////////////////////////////////////

#include "Resource.h"



/****************************************************** End *****************************************************/