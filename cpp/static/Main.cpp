#include <iostream>
#include <string>

using namespace std;

class student
{
public:
	static int number;
private:
	static string school;

public:
	
	int init();
	string get();
};

// !!!!!!!!!!!!!
//静态变量必须在外部定义和赋值，即便它是private的
string student::school = "";

int student::init()
{
	school="YiXiao";
	return 0;
}

string student::get()
{
	return school;
}

int main()
{
	student s;
	s.init();

	cout<<s.get()<<endl; //YiXiao

	//cout<<student::school<<endl; //std::string student::school’是私有的
	//cout<<student::number<<endl; //对‘student::number’未定义的引用

	student s2;
	cout<<s2.get()<<endl; //YiXiao
	return 0;
}
