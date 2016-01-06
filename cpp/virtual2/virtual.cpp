//------------------virtual.cpp
#include "virtual.h"

#include <iostream>
using namespace std;

void human::echo()
{
    cout<<"virtual:I'm a human"<<endl;
}

void woman::echo()
{
    cout<<"virtual:I'm a woman"<<endl;
}

void human::echo2()
{
    cout<<"I'm a human 2"<<endl;
}

void woman::echo2()
{
    cout<<"I'm a woman 2"<<endl;
}

void human::echo3()
{
    cout<<"I'm a human 3"<<endl;
}

human::~human()
{
    cout<<"virtual:~human"<<endl;
}

woman::~woman()
{
	cout<<"virtual:~woman"<<endl;
}

void woman::womanecho()
{
	cout<<"womanecho()"<<endl;
}

void woman::womanecho2()
{
	cout<<"womanecho()"<<endl;
}

void human::humanecho()
{
    cout<<"virtual: humanecho"<<endl;
}
