/************************************************************************/
/*							2014.07.19                            */
//							  ������ Ŭ����
//
/************************************************************************/


#pragma once

class CGameObject;



class cSHBillBoard : virtual  public CGameObject,
					 virtual  public IAlphaBlendable
{


	LPDIRECT3DTEXTURE9				m_pTex;// ������� ����� �ؽ�ó

	D3DMATERIAL9					m_Mtrl; // [2014.12.3 ] �����忡 ����� ���׸���
	D3DXHANDLE						m_hTech; // [2014.12.3 ]
	D3DXMATRIX						m_ViewTM_Inv;

	float							m_Width, m_Height;
	D3DXVECTOR3						m_Pos;   //������� ��ġ
	VOID*							pVertices;

	LPDIRECT3DVERTEXBUFFER9  		pVtxbuffer;//���ؽ� ����
	float							UVal_interval; // uv �ִϸ��̼� �� ����  [2014.12.3 ]
	//////////////////////////////////////////////////////////////////////////
	// �ǽð� ���� ���� // [2014.12.3 ]
	D3DXVECTOR3						m_Look, m_Up, m_Right;
	D3DXMATRIXA16					m_WorldTM, m_RotTM, m_ScaleTM, m_PosTM;
	float							m_UVAni_X; // uv�� u�� x��ŭ ������ �� [2014.12.3 ]
	int								FrameNum; // �������Ӹ��� �����ϴ� ����

	bool							m_bVisible;


public:

	//������ �Ҹ���
	cSHBillBoard();

	virtual~ cSHBillBoard();

public:
	void							Init(LPDIRECT3DDEVICE9 _pDevice, TCHAR* _TexPath, float Width , float FrameCnt , D3DXVECTOR3 _pos  = D3DXVECTOR3(0.0f,0.0f,0.0f));
	// �ִϸ��̼� ������ 1�� ��ȯ,,
	int								Update(float _dTime, D3DXVECTOR3 _Pos);
	// from IAlphaBlendable [2014.12.8 ]
	virtual	void					DrawAlpha();	
	// from IAlphaBlendable [2014.12.8 ] 
	// _CamPos : ī�޶� ��ġ
	virtual float				    CalDistToCam(D3DXVECTOR3 _CamPos);	
	
public:
	//get..set
	void							Set_hTech(const char* _szStr); // [2014.12.3 ]
	void							Set_m_UVAni_X(float _Val){m_UVAni_X = _Val;} // [2014.12.3 ]
	void							Set_m_bVisible(bool _Val){m_bVisible = _Val;} // [2014.12.8 ]
};