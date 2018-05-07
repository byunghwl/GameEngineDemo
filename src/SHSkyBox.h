/************************************************************************/
/*							[2014.8.2 ]
							  스카이박스.h

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
	LPDIRECT3DCUBETEXTURE9				m_pCubeTex;   //큐브텍스쳐 
	
public:
	cSkyBox();
	~cSkyBox();
	// _pPath : 큐브텍스쳐 파일 경로
	HRESULT								Init(LPDIRECT3DDEVICE9 _pDevice, wchar_t* _pPath);

	void								OnFrameMove(float _dTime);
	void								OnFrameRender(D3DXVECTOR3 _CameraPos);


};





/****************************************************** End *****************************************************/