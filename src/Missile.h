/************************************************************************/
/*							[2014.7.24 �輺ȯ]                          */
//							    bullet Ŭ����
//						���ð��ӿ� ���̴� ���� ��. ȭ������ ����, �̻����� ��Ŭ������ 
//								��ӹ޴´�.
//					
/************************************************************************/

#pragma  once

//[2014.7.24 �輺ȯ]
//////////////////////////////////////////////////////////////////////////
// ���漱��
class ASEMesh;
class AirCraft;
class cBullet;
class cSmoke;

//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* 
						[2014.7.24 �輺ȯ]
						cBullet�� �ܺο��� ���� ���� 
						��������� ��� ���� ����ü
*/
/************************************************************************/
struct Missile_DATA
{ 	 
	//�ܺο��� ���� (��ũ���� ��������..) �����Ͱ� �������
	//�̰��� �߰��մϴ�.... 

	//�� ����ü�� ������
	int		Size;
	bool	is_right ; //������ ������ �����Ǿ��ִ°�?
	float	Dist_to_Smoke;// �̻��� �������� ���� ���� ���� ���������� �Ÿ�
	Missile_DATA();	

};

/************************************************************************/
/*						 [2014.7.24 �輺ȯ]                                    
					 class cBullet : �ֻ���Ŭ����					    
*/
/************************************************************************/

class cMissile : public cBullet 
{
	friend  class AirCraft;

protected:
	// �������(�ܺο��� ���������ʴ� ���� �����͵�)
	static int					m_MisslieCnt; //�̻��� ��ü�� ���ӳ����� ������ �� �����̱� ������ static ������ ���� 
	static ASEMesh				*m_pMesh;	  //�޽� ����Ÿ  ��� �̻����� ���� �޽� ����
	AirCraft					*m_pAirCraft; // �� �̻����� ������ ���� ������,,
											  //(�̶����ʹ� ���� �ٴ��ʿ䰡 ���� ������ ����Ⱑ ������ �̻����� �����ؾ���)
	
	cSmoke						m_Smoke;	  //���� ���� ��ƼŬ �ý��� 
	D3DXVECTOR3					m_BezierPoint[4];  //������ ������ �� 4��
	float						m_BezPercent;  // ������ � �����

	//�ܺ� ���� ������
	Missile_DATA				m_MData; 
	Missile_DATA				m_MDbk;			//�Ѿ� ���½� ����� [2014.7.24 �輺ȯ]

public:
	//������ �Ҹ��� 
	cMissile();
	cMissile( LPDIRECT3DDEVICE9 _Device, Missile_DATA* pMData = nullptr, Bullet_DATA* pBData = nullptr );					 
	virtual ~cMissile();


public:
	//�����Լ� ������
	virtual int					Init( Missile_DATA* pMD , Bullet_DATA* pBD, LPDIRECT3DDEVICE9 _Device);	
	virtual	void				Reset();
	virtual void				Release();
	virtual int					Update(float timeDelta);
	virtual int					Draw(float dTime, float alpha = 1.0f);
	virtual void			    SetVtxShader(LPDIRECT3DVERTEXSHADER9 _pShader){ m_pMesh->Set_VtxShader(_pShader);		}
	virtual void				SetPixShader(LPDIRECT3DPIXELSHADER9 _pShader){m_pMesh->Set_PixShader(_pShader);		}
public:
	//���� üũ �Լ�


public:
	// get.. set
	ASEMesh*					GetASEData() const{ return m_pMesh;} //�ܺο��� �����͸��̿��� ���θ� �����ؾ��ϹǷ�
																	 //const�� ��ȯ�ϸ� �ȵ�


};




/****************************************************** End *****************************************************/