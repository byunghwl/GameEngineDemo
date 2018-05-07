/************************************************************************/
/*							2014. 07. 09								*/
/*				 ASECameraData = AseData�� ��ӹ���				        */
/*			 Camera Data�� Ase���Ͽ��� �о�� �����س��� Ŭ����		    */
/************************************************************************/

#pragma once



class ASEData;
/************************************************************************/
/* ASECameraData  class                                                 */
/************************************************************************/

class ASECameraData : public ASEData//���
{
	friend class ASEParser;

public:
	float			m_FOV;	//field of view
	NODE_TM			NodeTM;   
	TMAnimation		m_TMAni; // �ִϸ��̼� ����
	float			m_CurAniTime;// 
	D3DXMATRIX		m_MLocalTM, m_MWorldTM;	 // �θ�� ��ŭ  �̵��ߴ���.. ,�θ� ��ı��� �ٰ��� �ڽ��� ���� ���� ���
	D3DXMATRIX		m_LTM_scale,   m_LTM_Rot,	 m_LTM_Pos;  //���� ScaleTM, ���� RotTM, ���� PosTM;
	D3DXMATRIX		m_AniTM_Scale, m_AniTm_Rot,  m_AniTm_Pos;  //Ű������ �ִϸ��̼� �Ҷ� ����TM ��� ����ϱ����� ���

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

