#include "tuckunder.hpp"

/*
beging => {
end => }
*/
bool find_brace_pair(string& content, int point, int& begin, int& end)
{
	stack<int> pp;
	bool bRtn = false;

	point = content.find('{', point);
	if(point == content.npos)
		return false;

	while(point <= content.size())
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
		cout<<endl<<"--------code { and } not match--------"<<endl;

	return bRtn;
}

void add_tab_between_brace_pair(string& content, int begin, int end)
{// every enter add one /t
	int index = begin;
	while(index <= end)
	{
		if(content[index] == '\n')
		{
			content.insert(index+1, "\t");
			end++;
		}
		index++;
	}

	content.erase(end-1, 1);
}

bool is_in_note(string& content, int point)
{
	// {  }  ;   
	bool bRtn = false;
	for(int index = point; index > 0; index--)
	{
		if(content[index] == '\n')
			break;
	}

	if(index > 0)
	{
		int i=index;
		int j=i+1;
		for(; j<point; i++,j++)
		{
			if(content[i] == '/' && content[j] == '/')
			{
				bRtn = true;
				break;
			}
		}
	}

	return bRtn;
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

		add_tab_between_brace_pair(content, begin, end);

		point = begin+1;
	}
}
