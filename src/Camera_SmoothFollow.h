#pragma once



class CEntity;
class ASECameraData;
//////////////////////////////////////////////////////////////////////////
enum CAMERA_MODE
{	
	//���� CAMERA_MODE
	CM_SMOOTHFOLLOW_,
	CM_ANI_

};

/************************************************************************/
/*     Smooth Follow ī�޶� Ŭ����                                                    */
/************************************************************************/

class CCamera_SmoothFollow : CCamera

{

protected:

	CEntity*							m_pAirCraft;  //�i�ư� ����ⰴü�� ������
	
	
	//////////////////////////////////////////////////////////////////////////
	//���⼭���� ī�޶� �ִϸ��̼��� ���� ������
	bool							isRotAni_Pitch;//���� ȸ���ִϸ��̼� ���ΰ�?
	bool							isRotAni_Roll;//���� ȸ���ִϸ��̼� ���ΰ�?
	bool							isRotAni_Yaw;//���� ȸ���ִϸ��̼� ���ΰ�?

	float							m_CurAniProcess; // ���� ī�޶� ȸ���ִ��� ������ 0~1������ ��  0�̸� ����, 1�̸� ��

	D3DXVECTOR3						m_OldRight , m_NewRight;  //ī�޶� ���� �ִϸ��̼��� ���� Old�� New �� �־����
	D3DXVECTOR3						m_OldUp   , m_NewUp;
	D3DXVECTOR3						m_OldLook , m_NewLook;
	D3DXVECTOR3						m_pos;//ī�޶� ���� ��ġ

	D3DXMATRIX						m_MCurRot, m_MNewRot, m_MOldRot;  // ���� ȸ����� , ���ο� ȸ����� , ���� ȸ�����
	D3DXQUATERNION					m_Old_Q, m_New_Q; //���� �����, ���ο� �����
	
	CCamera_SmoothFollow();
	//////////////////////////////////////////////////////////////////////////

public:

	
	virtual ~CCamera_SmoothFollow();

	void							Init(LPDIRECT3DDEVICE9 _device, CEntity* pGameObject , ASECameraData* pCameraData = nullptr, int _CameraMode = 0);

public: // virtual �Լ� ������

	virtual	void					Update(float _dTime);

private: // ���� �Լ�

	bool							isPitchAngle_Larger(float _Angle); //PitchAngle�� _Angle ���� ������� ȸ������ ũ�ٸ� true
	bool							isRollAngle_Larger(float _Angle);  //RollAngle�� _Angle ���� ������� ȸ������ ũ�ٸ� true
	bool							isYawAngle_Larger(float _Angle);   //YawAngle�� _Angle ���� ������� ȸ������ ũ�ٸ� true

	void							Lerp(float dTime);
	void							Animation(float _dTime);
	
public://get,, set



};//End of class CCamera_SmoothFollow 




//************************************* �� *****************************************************//