/************************************************************************/
/*							2014. 07. 09								*/
/*				 ASECameraData = AseData를 상속받음				        */
/*			 Camera Data를 Ase파일에서 읽어와 저장해놓을 클래스		    */
/************************************************************************/

#pragma once



class ASEData;
/************************************************************************/
/* ASECameraData  class                                                 */
/************************************************************************/

class ASECameraData : public ASEData//상속
{
	friend class ASEParser;

public:
	float			m_FOV;	//field of view
	NODE_TM			NodeTM;   
	TMAnimation		m_TMAni; // 애니메이션 정보
	float			m_CurAniTime;// 
	D3DXMATRIX		m_MLocalTM, m_MWorldTM;	 // 부모로 얼만큼  이동했는지.. ,부모 행렬까지 다곱한 자신의 최종 월드 행렬
	D3DXMATRIX		m_LTM_scale,   m_LTM_Rot,	 m_LTM_Pos;  //로컬 ScaleTM, 로컬 RotTM, 로컬 PosTM;
	D3DXMATRIX		m_AniTM_Scale, m_AniTm_Rot,  m_AniTm_Pos;  //키프레임 애니메이션 할때 로컬TM 대신 사용하기위한 행렬

public:

	ASECameraData();
	virtual		~ASECameraData();
	virtual 	void    Update(float _dTime,  D3DXMATRIX MoveTM){};

public:

	void		  Animation(float _dTime);

	D3DXMATRIX    GetCurr_PosTM(float _CurTick);
	D3DXMATRIX    GetCurr_RotTM(float _CurTick);
	D3DXMATRIX    GetCurr_ScaleTM(float _CurTick);
	float		  GetFov(){return m_FOV;}

	void		  MakeViewMatrix(D3DXMATRIX* _pViewTM);
	

};

