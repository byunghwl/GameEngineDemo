/************************************************************************/
/*						Resource.h

*/
/************************************************************************/

#pragma  once

//리소스 상태
enum eRscState
{
	eRS_TOBE_DISCARD, // 폐기될 예정이다.(다음씬에서 사용되지 않아 지워질예정)
	eRS_TOBE_USED,	  //사용될 예정이다. (다음씬에서 사용될 예정)
};

class CResourceMgr;
class SHR_SkinMesh;

/************************************************************************/
/*							class SHR_VtxBuff
*/
/************************************************************************/
class SHR_VtxBuff : public IResource,
					public IRestoreable,
					public IRefCounter
{
		
private:

	UINT										m_NumVtx;		// 버텍스 개수	
	UINT										m_StrideSize;   // 버텍스 구조체 사이즈
	UINT										m_BuffSize;		// 버퍼사이즈
	DWORD										m_FVF;			// 버텍스 포멧
	int											m_PoolType;		// 메모리 풀 타입

	LPDIRECT3DVERTEXBUFFER9						m_pVtxBuff;     // 버텍스버퍼
	VOID*										m_pVtxBuffPoint;// 버텍스버퍼를가리키는 포인터


public:

	SHR_VtxBuff();
	virtual ~SHR_VtxBuff();

public:
	// 버텍스 버퍼를 생성한다.
	int											Create(_D3DPOOL eD3Dpool);
	int											Create();
	// 버텍스 버퍼를 복사한다.
	int											WriteBuf( void*& _pVtxBuf);

public:

	virtual void								Restore(){}; //device Lost때 사용예정

public://접근자

	LPDIRECT3DVERTEXBUFFER9						GetD3DVtxBuf(){return m_pVtxBuff;}
	UINT										GetBuffSize(){return m_BuffSize;}
	DWORD										GetFVF(){return m_FVF;}
	UINT										GetStrideSize(){return m_StrideSize;}
	UINT										GetNumVtx(){return m_NumVtx;}
	int											GetPoolType(){return m_PoolType;}

	void										SetNumVtx(int _NumVtx){m_NumVtx = _NumVtx;}
	void										SetStrideSize(int _size){m_StrideSize = _size;}
	void										SetBuffSize(int _size){m_BuffSize = _size;}
	void										SetFvF(int _FVF){m_FVF = _FVF;}
	void										SetPoolType(int _pooltype){m_PoolType= _pooltype;}
	void										SetName(const WCHAR* _pName);
	void										SetNameSize(int _Val){m_NameSize = _Val;}
										
};

/************************************************************************/
/*							class SHR_IdxBuff
*/
/************************************************************************/
class SHR_IdxBuff : public IResource,
					public IRestoreable,
					public IRefCounter
{
private:

	UINT										m_NumIdx;       // 인덱스 개수
	UINT										m_NumFace;		// 폴리곤개수
	UINT										m_BuffSize;		// 인덱스버퍼 사이즈
	int											m_PoolType;		// 메모리 풀 타입
	LPDIRECT3DINDEXBUFFER9						m_pIdxBuff;		// 인덱스 버퍼
	void*										m_pIdxBuffPoint;// 인덱스버퍼를 가리키는 포인터
	
public:
	// 생성자
	SHR_IdxBuff();
	// 소멸자
	virtual ~SHR_IdxBuff();

public:
	// 인덱스 버퍼를 생성한다.
	int											Create(_D3DPOOL eD3Dpool);
	int											Create();
	// 인덱스 버퍼를 복사한다.
	int											WriteBuf( void*& _pIdxBuf);
	// 인덱스 설정
	void										SetIndices();

public:

	virtual void								Restore(){}; //device Lost때 사용예정

public://접근자

	LPDIRECT3DINDEXBUFFER9						GetD3DVtxBuf(){return m_pIdxBuff;}
	UINT										GetBuffSize(){return m_BuffSize;}
	UINT										GetNumIdx(){return  m_NumIdx;} 
	UINT										GetNumFace(){return  m_NumFace;} 

	void										SetNumIdx(int _NumIdx){m_NumIdx = _NumIdx;}
	void										SetNumFace(int _Numface){m_NumFace = _Numface;}
	void										SetBuffSize(int _size){m_BuffSize = _size;}										
	void										SetPoolType(int _pooltype){m_PoolType= _pooltype;}
	void										SetName(const WCHAR* _pName);
	void										SetNameSize(int _Val){m_NameSize = _Val;}
};






/************************************************************************/
/*							class SHR_D3DTexture
*/
/************************************************************************/
class SHR_D3D9Texture : public IResource,
						public IRefCounter
{
private:

	LPDIRECT3DTEXTURE9						m_pTex;      //다이렉트x용 텍스쳐 
	
public://생성자 소멸자

	SHR_D3D9Texture();
	virtual ~SHR_D3D9Texture();

public:

	int										Create(const WCHAR* _pName,const WCHAR* _pPath = nullptr);
	LPDIRECT3DTEXTURE9						GetD3DTex(){return m_pTex;}


};
/************************************************************************/
/*						  class SHR_VtxShader
						성환_리소스_D3D9버텍스셰이더
						   IResource를 상속받음
						    [2014.9.3 ]
*/
/************************************************************************/
class SHR_VtxShader : public IResource, //base class
					  public IRefCounter
{
private:
	
	// d3d9 버텍스셰이더 객체
	LPDIRECT3DVERTEXSHADER9				m_pVtxShader ; 

public:

	SHR_VtxShader();
	virtual ~SHR_VtxShader();
	int									Create(const WCHAR* _pName , const WCHAR* _pPath  );

public://접근자
	//버텍스셰이더 켜기
	void								TurnOn(); 
	//버텍스셰이더 끄기
	void								TurnOff();
};





/************************************************************************/
/*						  class SHR_PixShader
						성환_리소스_D3D9픽셀셰이더
						   IResource를 상속받음
						    [2014.9.3 ]
*/
/************************************************************************/
class SHR_PixShader : public IResource, //base class
						  public IRefCounter
{
private:
	
	// d3d9 픽셀셰이더 객체
	LPDIRECT3DPIXELSHADER9				m_pPixShader ; 

public:

	SHR_PixShader();
	virtual ~SHR_PixShader();
	int									Create(const WCHAR* _pName , const WCHAR* _pPath  );

public://접근자
	//버텍스셰이더 켜기
	void								TurnOn(); 
	//버텍스셰이더 끄기
	void								TurnOff();
};





#include "SHR_Mesh.h"



/****************************************************** End *****************************************************/