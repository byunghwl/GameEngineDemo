
/************************************************************************/
/*				// [2014.12.2 ]
					SH_Sprite.h
*/
/************************************************************************/
#pragma once



/************************************************************************/
/*					class CSprite
				스프라이트를 그려주는 클래스 
				// [2014.12.2 ]
*/
/************************************************************************/
class CSprite 
{
protected:

	LPDIRECT3DDEVICE9					m_pDevice; // Our rendering device
	LPDIRECT3DTEXTURE9					m_pTex;//로딩화면
	RECT								m_SrcRect; // 소스이미지에서 사용할 영역 
	LPD3DXSPRITE						m_pD3DSprite; //
	D3DSURFACE_DESC						m_TexInfo;// 텍스쳐 정보			

public: // 생성자 소멸자

	CSprite();
	virtual ~CSprite();

public:

	//기본 함수
	LPDIRECT3DTEXTURE9					LoadTexture(const WCHAR* szFileName, int nWidth, int nHeight, DWORD ColorKey, BOOL bHasAlpha );

	bool							    Init(IDirect3DDevice9 *Device,  LPCWSTR _szPath , RECT* _pSrcRect = nullptr); 
	void								Update(float _dTime);
	void								Draw();






};

//////////////////////////////////////////////////////////////////////////
//전역변수 선언
extern CSprite							G_SpriteTitle; 


/******************************************************** end *************************************************************************/