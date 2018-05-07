/************************************************************************/
/*				// [2014.12.2 ]
					SH_Sprite.cpp
*/
/************************************************************************/
#include "DXUT.h"
#include "SH_Sprite.h"

//////////////////////////////////////////////////////////////////////////
//전역변수 선언
CSprite						G_SpriteTitle; // [2014.12.2 ]
UINT						G_CurBuffWidth;
UINT						G_CurBuffHight; // [2014.12.2 ]
//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : 
//-----------------------------------------------------------------------------
CSprite::CSprite()
{
	m_pTex = nullptr;
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : 
//-----------------------------------------------------------------------------
CSprite::~CSprite()
{
	SAFE_RELEASE(m_pDevice) // Our rendering device
	SAFE_RELEASE(m_pTex)//로딩화면
	
	SAFE_RELEASE(m_pD3DSprite) //
}


//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : 
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9	CSprite::LoadTexture(const WCHAR* szFileName, int nWidth, int nHeight, DWORD ColorKey, BOOL bHasAlpha )
{
	

	D3DXIMAGE_INFO SrcInfo;
	//wcscpy(filePath,szFileName);
	HRESULT hr;

	D3DFORMAT fmt = ( bHasAlpha || ( ColorKey != 0 ) ) ? D3DFMT_A8R8G8B8 : D3DFMT_X8R8G8B8;

	hr = D3DXCreateTextureFromFile(m_pDevice,szFileName, &m_pTex);
		

	if( hr != D3D_OK )
	{	SHUtil::MsgBoxW(L"텍스쳐로딩실패");
		return NULL;
	}
	
	return m_pTex;
}

//-----------------------------------------------------------------------------
// Arg    : IDirect3DDevice9 *Device, LPCWSTR _szPath, RECT* _pSrcRect /*= nullptr*/  [2014.12.2 ]
// Return :  true  ,, false
// Desc   : 
//-----------------------------------------------------------------------------
bool	CSprite::Init(IDirect3DDevice9 *Device, LPCWSTR _szPath, RECT* _pSrcRect /*= nullptr*/ )
{
	// RECT* _pSrcRect 나중에 이거 nullptr 일때랑 아닐때랑 처리 하기 ★★★ // [2014.12.2 ]
	m_pDevice =Device;
	m_pTex = LoadTexture(_szPath, 1021, 764, 0xFFFF00FF, FALSE );

	if(FAILED(D3DXCreateSprite(m_pDevice, &m_pD3DSprite)))
	{
		::MessageBox(0, L"CSprite::Init", L"PSystem", 0);
		return false;
	}

	G_CurBuffWidth=SHGlobal_GetScreenWidth;//현재화면의 가로
	G_CurBuffHight=SHGlobal_GetScreenHeight;//현재화면의 세로
	
	
	m_pTex->GetLevelDesc(0,&m_TexInfo);
	m_SrcRect.left = 0;
	m_SrcRect.top = 0;
	m_SrcRect.bottom = m_TexInfo.Height;
	m_SrcRect.right = m_TexInfo.Width;
	return true;
}


//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : 
//-----------------------------------------------------------------------------
void		CSprite::Update(float _dTime)
{

}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : 
//-----------------------------------------------------------------------------
void	CSprite::Draw()
{

	HRESULT F =m_pD3DSprite->Begin(  D3DXSPRITE_ALPHABLEND); //| D3DXSPRITE_OBJECTSPACE  );

	D3DXMATRIX TempTM, sMat_Title;
	D3DXMatrixIdentity(&TempTM);
	D3DXVECTOR3 tempPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vCenter( 0.0f, 0.0f, 0.0f );

	D3DXMatrixIdentity(&sMat_Title);
	D3DXMatrixScaling(&sMat_Title, ((float)G_CurBuffWidth/(float)m_TexInfo.Width), ((float)G_CurBuffHight/(float)m_TexInfo.Height) , 1.0f);//로딩이미지 해상도에따라 스케일하기

	m_pD3DSprite->SetTransform(&sMat_Title);
	m_pD3DSprite->Draw(m_pTex, NULL,  NULL, NULL, 0xFFFFFFFF );

	
	m_pD3DSprite->End();
}




/************************************************** end *****************************************************/