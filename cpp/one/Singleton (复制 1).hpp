#include <iostream>

using namespace std;

class CSingleton
{  
	// 其它成员  
public:
	static CSingleton* GetInstance();
	void ReleaseInstance();

private:
	static int count;
	CSingleton(){}; // can't new
	~CSingleton(){}; //can't delete
	static CSingleton * m_pInstance;
};
