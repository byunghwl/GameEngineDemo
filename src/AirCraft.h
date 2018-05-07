


#pragma  once

//////////////////////////////////////////////////////////////////////////
// ���漱��
class ASEMesh;
class cMissile;
class CEntity;
//////////////////////////////////////////////////////////////////////////
enum AIRCRAFT_TEXTURE_TYPE
{	//AIRCRAFT_TEXTURE_TYPE
	ATT_UP,
	ATT_DOWN,
	ATT_RIGHT,
	ATT_LEFT,
	ATT_FROTN,
	ATT_BACK,
//	ATT_SNIPER,
	ATT_MAX
};

/************************************************************************/
/*						[2014.7.26 �輺ȯ]
				struct A_DATA :  AirCraft�� ���� �ܺ� ����(��ũ��Ʈ ��)
				�� �����Ѵ�.
*/
/************************************************************************/
struct	A_DATA
{
	int			Size; // �� ����ü�� ������
	int			MissileCnt; //������ �ִ� �̻��� ����
};


/************************************************************************/
/*					[2014.7.26 �輺ȯ]
				class AirCraft  ����� Ŭ����
				CEntity Ŭ������ ��ӹ޴´�.
*/
/************************************************************************/
class AirCraft 
	: public CEntity, //Parent Class
	  public IControlable,
	  public IRenderable,
	  public IProgressable
{
	friend  class cMissile;
protected:

	ASEMesh*					m_pAseMesh; //ase������ �о�鿩 ���� �޽�
	

	const HELPEROBJECT			*m_pHFirePos_R, *m_pHFirePos_L; // �̻��� �߻���ġ ������, ������ �˷��� ���ۿ�����Ʈ.. �������� �ű⶧���� �޸� ������ �ʿ� ����.
	
	//////////////////////////////////////////////////////////////////////////
	// transform�� ���� �������
	//D3DXVECTOR3				m_VecRight, m_VecUp, m_VecSee; //$ ����Ŭ������ �̵� [2014.7.26 �輺ȯ]
	//D3DXVECTOR3				m_Pos;         //���� ���� ����  ��ġ //$ ����Ŭ������ �̵� [2014.7.26 �輺ȯ]


	//////////////////////////////////////////////////////////////////////////
	// �ܺκ���
	A_DATA						m_AData;
	A_DATA						m_ADatabk;// �����

public:
	AirCraft();
	virtual ~AirCraft();

public: //�����Լ�
	
	virtual void					Draw();
	virtual void					Update(float _dTime);	
	virtual bool					MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void					SetVtxShader(LPDIRECT3DVERTEXSHADER9 _pShader){m_pAseMesh->Set_VtxShader(_pShader);		}
	virtual void					SetPixShader(LPDIRECT3DPIXELSHADER9 _pShader){m_pAseMesh->Set_PixShader(_pShader);		}
	//�ʱ�ȭ�Ҷ� �ʿ��� �Լ�
	HRESULT							Init(LPDIRECT3DDEVICE9 _pDevice , A_DATA* pAData, UNIT_DATA* pUData );
	int								InitGameObject();
	void							InitMaterial(D3DXCOLOR _Ambi, D3DXCOLOR _Diffuse, D3DXCOLOR _Specu, D3DXCOLOR _Emissive, float _Specu_Power);

public:

	//�̵������� �Լ�
	void							MoveLeft(float& _dTime);
	void							MoveRight(float& _dTime);
	void							MoveForward(float& _dTime);
	void							MoveBack(float& _dTime);
	void							MoveDown(float& _dTime);
	void							MoveUp(float& _dTime);
	//ȸ���� ���� �Լ�
	void							Rot_Pitch(float _Angle);// ���⺤�͸� �������� ȸ��
	void							Rot_Roll(float _Angle);//���溤�� �������� ȸ��
	void							Rot_Yaw(float _Angle);//���⺤�ͱ������� ȸ��


public: //Get,, set �Լ�


	ASEMesh*						GetASEData() const{ return m_pAseMesh;}
	





};


//////////////////////////////////////////////////////////////////////////
// �������� �����




/****************************************************** End *****************************************************/