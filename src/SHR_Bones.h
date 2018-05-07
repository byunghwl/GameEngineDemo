
/************************************************************************/
/*							SHR_Bones.h
					뼈대정보 가지고 있는 리소스
						[2014.9.15 배성환]
				
*/
/************************************************************************/
#pragma once



class SHR_Mesh;
/************************************************************************/
/*						class SHR_Bones
*/
/************************************************************************/
class SHR_Bones : public IResource, 
				  public IRefCounter

{
private:

	std::vector<SHR_Mesh*>				m_Vec_BoneList;

public:
	SHR_Bones();
	virtual ~SHR_Bones();
	int									Create(const WCHAR* _pName , const WCHAR* _pPath  );


};