/************************************************************************/
/*						Resource.h

*/
/************************************************************************/

#pragma  once

//���ҽ� ����
enum eRscState
{
	eRS_TOBE_DISCARD, // ���� �����̴�.(���������� ������ �ʾ� ����������)
	eRS_TOBE_USED,	  //���� �����̴�. (���������� ���� ����)
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

	UINT										m_NumVtx;		// ���ؽ� ����	
	UINT										m_StrideSize;   // ���ؽ� ����ü ������
	UINT										m_BuffSize;		// ���ۻ�����
	DWORD										m_FVF;			// ���ؽ� ����
	int											m_PoolType;		// �޸� Ǯ Ÿ��

	LPDIRECT3DVERTEXBUFFER9						m_pVtxBuff;     // ���ؽ�����
	VOID*										m_pVtxBuffPoint;// ���ؽ����۸�����Ű�� ������


public:

	SHR_VtxBuff();
	virtual ~SHR_VtxBuff();

public:
	// ���ؽ� ���۸� �����Ѵ�.
	int											Create(_D3DPOOL eD3Dpool);
	int											Create();
	// ���ؽ� ���۸� �����Ѵ�.
	int											WriteBuf( void*& _pVtxBuf);

public:

	virtual void								Restore(){}; //device Lost�� ��뿹��

public://������

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

	UINT										m_NumIdx;       // �ε��� ����
	UINT										m_NumFace;		// �����ﰳ��
	UINT										m_BuffSize;		// �ε������� ������
	int											m_PoolType;		// �޸� Ǯ Ÿ��
	LPDIRECT3DINDEXBUFFER9						m_pIdxBuff;		// �ε��� ����
	void*										m_pIdxBuffPoint;// �ε������۸� ����Ű�� ������
	
public:
	// ������
	SHR_IdxBuff();
	// �Ҹ���
	virtual ~SHR_IdxBuff();

public:
	// �ε��� ���۸� �����Ѵ�.
	int											Create(_D3DPOOL eD3Dpool);
	int											Create();
	// �ε��� ���۸� �����Ѵ�.
	int											WriteBuf( void*& _pIdxBuf);
	// �ε��� ����
	void										SetIndices();

public:

	virtual void								Restore(){}; //device Lost�� ��뿹��

public://������

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

	LPDIRECT3DTEXTURE9						m_pTex;      //���̷�Ʈx�� �ؽ��� 
	
public://������ �Ҹ���

	SHR_D3D9Texture();
	virtual ~SHR_D3D9Texture();

public:

	int										Create(const WCHAR* _pName,const WCHAR* _pPath = nullptr);
	LPDIRECT3DTEXTURE9						GetD3DTex(){return m_pTex;}


};
/************************************************************************/
/*						  class SHR_VtxShader
						��ȯ_���ҽ�_D3D9���ؽ����̴�
						   IResource�� ��ӹ���
						    [2014.9.3 ]
*/
/************************************************************************/
class SHR_VtxShader : public IResource, //base class
					  public IRefCounter
{
private:
	
	// d3d9 ���ؽ����̴� ��ü
	LPDIRECT3DVERTEXSHADER9				m_pVtxShader ; 

public:

	SHR_VtxShader();
	virtual ~SHR_VtxShader();
	int									Create(const WCHAR* _pName , const WCHAR* _pPath  );

public://������
	//���ؽ����̴� �ѱ�
	void								TurnOn(); 
	//���ؽ����̴� ����
	void								TurnOff();
};





/************************************************************************/
/*						  class SHR_PixShader
						��ȯ_���ҽ�_D3D9�ȼ����̴�
						   IResource�� ��ӹ���
						    [2014.9.3 ]
*/
/************************************************************************/
class SHR_PixShader : public IResource, //base class
						  public IRefCounter
{
private:
	
	// d3d9 �ȼ����̴� ��ü
	LPDIRECT3DPIXELSHADER9				m_pPixShader ; 

public:

	SHR_PixShader();
	virtual ~SHR_PixShader();
	int									Create(const WCHAR* _pName , const WCHAR* _pPath  );

public://������
	//���ؽ����̴� �ѱ�
	void								TurnOn(); 
	//���ؽ����̴� ����
	void								TurnOff();
};





#include "SHR_Mesh.h"



/****************************************************** End *****************************************************/