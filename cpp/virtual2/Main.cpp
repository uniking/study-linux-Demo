#include "virtual.h"
#include <iostream>
using namespace std;

int main()
{
     //对象指针为基类
    human* hm = new woman();
    cout<<"----human*----"<<endl;
    hm->echo(); //new对象的虚函数覆盖同名的基类虚函数
    hm->echo2();//调用human类已实现的
    hm->echo3();//调用human类已实现的

    //hm->womanecho(); //一般函数编译不过
    //hm->womanecho2(); //虚函数也编译不过
    delete hm;//虚析构没有被覆盖

    woman* wm = new woman();
    cout<<"----woman*----"<<endl;
    wm->echo();
    wm->echo2();//重载(多态)

    wm->echo3();//一般继承
    wm->humanecho();//虚函数继承
    delete wm;

    return 0;
}

