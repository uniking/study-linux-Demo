#include "tuckunder.hpp"

bool find_brace_pair(string& content, int point, int& begin, int& end)
{
	stack<int> pp;
	bool bRtn = false;

	point = content.find('{', point);
	if(point == content.npos)
		return false;

	while(point < content.size())
	{
		if(content[point] == '{')
			pp.push(point);
		else if(content[point] == '}')
		{
			begin = pp.top();
			pp.pop();
		}
		else
			;

		if(pp.empty())
		{
			end = point;
			bRtn = true;
			break;
		}
		
		point++;
	}

	if(!pp.empty())
		cout<<endl<<"code { and } not match"<<endl;

	return bRtn;
}

void add_tab_between_brace_pair(int begin, int end)
{// every enter add one /t
}

void add_tuckunder(string& content)
{
	int point = 0;
	while(point < content.size())
	{
		int begin = 0;
		int end = 0;
		if(!find_brace_pair(content, point, begin, end))
		{
			cout<<"not find brace pair"<<endl;
			break;
		}

		add_tab_between_brace_pair(begin, end);

		point = begin;
	}
}
