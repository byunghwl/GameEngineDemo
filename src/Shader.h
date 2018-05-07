/************************************************************************/
/*						Resource.h

*/
/************************************************************************/

#pragma  once


/************************************************************************/
/*					class CFX_SHADER  
				���̷�Ʈ ���� ����Ʈ ������ ��ũ [2014.10.22 ]
*/
/************************************************************************/
class CFX_SHADER  : public IResource
{
public:

	LPD3DXEFFECT						m_pShader; // ���̴� ��ü
	int									m_MaxPass;

public:

	CFX_SHADER();
	virtual ~CFX_SHADER();
	int									Create(const WCHAR* _pName , const WCHAR* _pPath  );

public:// get.. set
	void								SetMaxPass(int val){m_MaxPass = val;}

	LPD3DXEFFECT						GetPointer(){return m_pShader;}
	int							        GetMaxPass(){return m_MaxPass;}
};




/***********************************************************************
           // �� ������ ���������� ����� �� �ִ� ����Ʈ ������ 
		                 ��ũ ��� ���̺�         // [2014.10.30 ]                                               
************************************************************************/

class C_ConstantTable
{

public:
	//////////////////////////////////////////////////////////////////////////
	//C_ConstantTable�� �ý��� �������� ������ �� �ֵ��� �ۺ����� ...
	D3DXHANDLE			h_LightDiff;  // ���� ��
	D3DXHANDLE			h_LightDir;   // ���� ����
	D3DXHANDLE 			h_LightAmbi;  // ���� �ֺ���
	D3DXHANDLE 			h_ViewPos;   // ī�޶� ��ġ
	D3DXHANDLE 			h_ViewProjTM; //view * proj TM
	D3DXHANDLE 			h_WorldTM;  // World TM
	D3DXHANDLE 			h_MatPallete;// ��Ű���� ���� ��Ʈ���� �ȷ�Ʈ
	D3DXHANDLE 			h_Mtrl_SpecularPower; // ������ ����ŧ�� �Ŀ�
	D3DXHANDLE			h_PointLightPos;  //����Ʈ����Ʈ�� ������   [2014.11.12 ]
	D3DXHANDLE			h_PointLightRadius;  //����Ʈ����Ʈ�� ����  [2014.11.12 ]

	D3DXHANDLE			h_LineColor;  //�ܰ��� Į��  [2014.11.13 ]
	D3DXHANDLE			h_LineOffset;  //�ܰ��� �΃� ����  [2014.11.13 ]
	D3DXHANDLE			h_LimLightColor;// ������ƮĮ��
	D3DXHANDLE			h_LimOffset; // ������Ʈ ����
	D3DXHANDLE			h_UVAni_Val ; // ���� �ִϸ��̼� �Ҷ� �ʿ��� �� // [2014.12.3 ]
	//////////////////////////////////////////////////////////////////////////
	// ���⼭���� �ؽ��� �ڵ�
	D3DXHANDLE 			h_TexDiffMap ;  //  ��ǻ�� �ؽ����� �ڵ�
	D3DXHANDLE 			h_TexSpecularMap ;  //  ����ŧ���� �ؽ����� �ڵ�
	D3DXHANDLE 			h_TexOpacityMap ;  // ����� �ؽ����� �ڵ�
	D3DXHANDLE 			h_TexNormMap ;  //  �븻�� �ؽ����� �ڵ�
	D3DXHANDLE 			h_TexShadowMap; // �׸��ڸ� �ؽ���
	//����Ʈ ����Ʈ
	D3DXHANDLE			h_TexTargetResult; // ����Ʈ����Ʈ �Ҷ� ���������� �������� �׷��� �ؽ��� [2014.11.8 ]
	D3DXHANDLE			h_Tex_Pass0;       // �н��� �������϶� �н����� �ؽ��������� �ʿ��ҋ� [2014.11.8 ]
	D3DXHANDLE			h_Tex_Pass1;
	D3DXHANDLE			h_Tex_Pass2;
	D3DXHANDLE			h_Tex_Pass3;
	D3DXHANDLE			h_Tex_Pass4;
	D3DXHANDLE			h_Tex_Pass5;
	D3DXHANDLE			h_Tex_Pass6;
	//////////////////////////////////////////////////////////////////////////
	// ���⼭���� ��ũ��ť �ڵ�
	D3DXHANDLE 			h_Tech_Skin_DirLight; // ��Ű�� + ppl + directional Light
	D3DXHANDLE 			h_Tech_Skin_SpecularMap_DirLight; // ��Ű�� + ppl + ����ŧ���� ���� +  directional Light [2014.11.6 ]
	D3DXHANDLE			h_Tech_NoEffect;

public:
	C_ConstantTable();
	~C_ConstantTable();

	int			        Initialize();


};


//////////////////////////////////////////////////////////////////////////
// �ܺ� �������� ����

extern CFX_SHADER            G_FXShader;
extern C_ConstantTable       G_ConstantTable;
//////////////////////////////////////////////////////////////////////////
// �ܺ� �������� ����


/****************************************************** End *****************************************************/
