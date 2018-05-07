
/************************************************************************/
/*				// [2014.12.2 ]
					SH_Sprite.h
*/
/************************************************************************/
#pragma once



/************************************************************************/
/*					class CSprite
				��������Ʈ�� �׷��ִ� Ŭ���� 
				// [2014.12.2 ]
*/
/************************************************************************/
class CSprite 
{
protected:

	LPDIRECT3DDEVICE9					m_pDevice; // Our rendering device
	LPDIRECT3DTEXTURE9					m_pTex;//�ε�ȭ��
	RECT								m_SrcRect; // �ҽ��̹������� ����� ���� 
	LPD3DXSPRITE						m_pD3DSprite; //
	D3DSURFACE_DESC						m_TexInfo;// �ؽ��� ����			

public: // ������ �Ҹ���

	CSprite();
	virtual ~CSprite();

public:

	//�⺻ �Լ�
	LPDIRECT3DTEXTURE9					LoadTexture(const WCHAR* szFileName, int nWidth, int nHeight, DWORD ColorKey, BOOL bHasAlpha );

	bool							    Init(IDirect3DDevice9 *Device,  LPCWSTR _szPath , RECT* _pSrcRect = nullptr); 
	void								Update(float _dTime);
	void								Draw();






};

//////////////////////////////////////////////////////////////////////////
//�������� ����
extern CSprite							G_SpriteTitle; 


/******************************************************** end *************************************************************************/