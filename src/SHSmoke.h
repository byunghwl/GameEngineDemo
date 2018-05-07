/************************************************************************/
/*							[2014.7.23 ]                          */
//							  ���� ȿ�� 
//					
/************************************************************************/

#pragma  once
class PSystem;



/************************************************************************/
/*						 [2014.7.23 ]                                    
						    class cSmoke
					     PSystem Ŭ������ ���
*/
/************************************************************************/

class cSmoke : public PSystem
{
protected:
	//�������
	D3DXVECTOR3				m_EmissDir;  //���� ���� ����
	float					m_SprMaxSize; // sprite �ƽ� ������
	float					m_EmitPower;//���� �Ŀ�
public:
	cSmoke(){};
	//������ ó��������ǥ��, ��ƼŬ ����
	cSmoke(D3DXVECTOR3* origin, int numParticles, float Maxsize);
	//�Ҹ���
	virtual ~cSmoke();
	// �ʱ�ȭ �Լ�  _EmitPower : ���� ���� ���� , _texFileName: ����� �ؽ��� �̸�
	int						Init(  LPDIRECT3DDEVICE9 _pDevice , LPCWSTR _texFileName, float _EmitPower = 10.0f,
								   int numParticles = 256, float sprMaxsize = 100.0f);	
	
public:
	//�����Լ� ������
	virtual void			Reset_Attribute(Attribute* attribute,int i=0);
	virtual void			update(float timeDelta);
	virtual void			preRender();
	virtual void			postRender();

public:
	// get.. set
	void					Set_EmissDir(D3DXVECTOR3 _Val){ m_EmissDir = _Val;}


};