/************************************************************************/
/*							[2014.8.2 ]
							  ��ī�̹ڽ�.h

*/
/************************************************************************/

#pragma once


/************************************************************************/
/*								[2014.8.2 ]
							class cSkyBox
*/
/************************************************************************/

class cSkyBox
{

private:

	LPDIRECT3DDEVICE9					m_pDevice;
	LPDIRECT3DVERTEXBUFFER9				m_pVtxBuf;
	LPDIRECT3DINDEXBUFFER9				m_pIdxBuf;
	LPDIRECT3DCUBETEXTURE9				m_pCubeTex;   //ť���ؽ��� 
	
public:
	cSkyBox();
	~cSkyBox();
	// _pPath : ť���ؽ��� ���� ���
	HRESULT								Init(LPDIRECT3DDEVICE9 _pDevice, wchar_t* _pPath);

	void								OnFrameMove(float _dTime);
	void								OnFrameRender(D3DXVECTOR3 _CameraPos);


};





/****************************************************** End *****************************************************/