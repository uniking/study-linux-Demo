#include "Singleton.hpp"

CSingleton * CSingleton::m_pInstance = NULL;
int CSingleton::count = 0;
CSingleton* CSingleton::GetInstance()
{
	if(m_pInstance == NULL)
		m_pInstance = new CSingleton;
	++count;
	return m_pInstance;
}

void CSingleton::ReleaseInstance()
{
	--count;
	if(count == 0)
	{
		cout<<"delete instance"<<endl;
		delete m_pInstance;
	}
}

/*
int main()
{
	//CSingleton one; //CSingleton::CSingleton()’是私有的

	CSingleton* pone = CSingleton::GetInstance();
	cout<<pone<<endl;

	CSingleton* ptwo = CSingleton::GetInstance();
	cout<<ptwo<<endl;

	pone->ReleaseInstance();
	ptwo->ReleaseInstance();
	//delete pone;
	//delete ptwo;
	return 0;
}
*/
