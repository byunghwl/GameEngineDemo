/************************************************************************/
/*							2014.07.19                            */
//							  빌보드 클래스
//
/************************************************************************/


#pragma once

class CGameObject;



class cSHBillBoard : virtual  public CGameObject,
					 virtual  public IAlphaBlendable
{


	LPDIRECT3DTEXTURE9				m_pTex;// 빌보드로 사용할 텍스처

	D3DMATERIAL9					m_Mtrl; // [2014.12.3 ] 빌보드에 적용될 메테리얼
	D3DXHANDLE						m_hTech; // [2014.12.3 ]
	D3DXMATRIX						m_ViewTM_Inv;

	float							m_Width, m_Height;
	D3DXVECTOR3						m_Pos;   //월드상의 위치
	VOID*							pVertices;

	LPDIRECT3DVERTEXBUFFER9  		pVtxbuffer;//버텍스 버퍼
	float							UVal_interval; // uv 애니메이션 할 간격  [2014.12.3 ]
	//////////////////////////////////////////////////////////////////////////
	// 실시간 계산될 변수 // [2014.12.3 ]
	D3DXVECTOR3						m_Look, m_Up, m_Right;
	D3DXMATRIXA16					m_WorldTM, m_RotTM, m_ScaleTM, m_PosTM;
	float							m_UVAni_X; // uv의 u에 x만큼 더해줄 값 [2014.12.3 ]
	int								FrameNum; // 매프래임마다 증가하는 숫자

	bool							m_bVisible;


public:

	//생성자 소멸자
	cSHBillBoard();

	virtual~ cSHBillBoard();

public:
	void							Init(LPDIRECT3DDEVICE9 _pDevice, TCHAR* _TexPath, float Width , float FrameCnt , D3DXVECTOR3 _pos  = D3DXVECTOR3(0.0f,0.0f,0.0f));
	// 애니메이션 끝나면 1을 반환,,
	int								Update(float _dTime, D3DXVECTOR3 _Pos);
	// from IAlphaBlendable [2014.12.8 ]
	virtual	void					DrawAlpha();	
	// from IAlphaBlendable [2014.12.8 ] 
	// _CamPos : 카메라 위치
	virtual float				    CalDistToCam(D3DXVECTOR3 _CamPos);	
	
public:
	//get..set
	void							Set_hTech(const char* _szStr); // [2014.12.3 ]
	void							Set_m_UVAni_X(float _Val){m_UVAni_X = _Val;} // [2014.12.3 ]
	void							Set_m_bVisible(bool _Val){m_bVisible = _Val;} // [2014.12.8 ]
};