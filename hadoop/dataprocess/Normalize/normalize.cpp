#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef struct _STATISTICS
{
	float max;
	float min;
	float range; //range = max - min
}STATISTICS;

typedef vector< vector<float> > dataMatrix;// rol<row>

typedef vector<STATISTICS> dataStatistics;


void Tokenize(const string& str,
        vector<string>& tokens,
        const string& delimiters)
{  
    // Skip delimiters at beginning.  
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);  
    // Find first "non-delimiter".  
    string::size_type pos     = str.find_first_of(delimiters, lastPos);  
    while (string::npos != pos || string::npos != lastPos)  
    {  
        // Found a token, add it to the vector.  
        tokens.push_back(str.substr(lastPos, pos - lastPos));  
        // Skip delimiters.  Note the "not_of"  
        lastPos = str.find_first_not_of(delimiters, pos);  
        // Find next "non-delimiter"  
        pos = str.find_first_of(delimiters, lastPos);  
    }  
}

bool get_weight_value(string path, vector<float>& weight)
{
	ifstream file(path.c_str());
	if(!file.is_open())
		return false;

	string sub;
	getline(file, sub);
	if(sub.size() == 0)
	{
		cout<<"weight in the first line"<<endl;
		return false;
	}

	vector<string> line_words;
	Tokenize(sub, line_words, " ");
	vector<string>::iterator p_word = line_words.begin();
	while(p_word != line_words.end())
	{
		weight.push_back( atof((*p_word).c_str()) );
		p_word++;
	}


	return true;
}

bool file_to_matrix(string path, dataMatrix& dataM)
{
	ifstream file(path.c_str());
	if(!file.is_open())
		return false;

	while(true)
	{
		string sub;
		getline(file, sub);
		if(sub.size() == 0)
			break;

		vector<float> val;
		vector<string> line_words;
		Tokenize(sub, line_words, " ");
		vector<string>::iterator p_word = line_words.begin();
		while(p_word != line_words.end())
		{
			val.push_back( atof((*p_word).c_str()) );
			p_word++;
		}
		dataM.push_back(val);
	}

	return true;
}

bool get_attr_statistics(dataMatrix& dataM, dataStatistics& dataS)
{
	int row = dataM.size();
	int rol = (dataM[0]).size();
	int index = 0;
	float max=0;
	float min=0;

	while(index < rol)
	{
		dataMatrix::iterator pRow = dataM.begin();
		max = (*pRow)[index];
		min = (*pRow)[index];
		while(pRow != dataM.end())
		{
			if((*pRow)[index] > max)
				max = (*pRow)[index];
			if((*pRow)[index] < min)
				min = (*pRow)[index];

			pRow++;
		}

		STATISTICS st;
		st.max = max;
		st.min = min;
		st.range = max-min;
		dataS.push_back(st);

		index++;
	}
	
	return true;
}

bool get_attr_normal(dataMatrix& dataM, dataStatistics& dataS, vector<float>& weight, dataMatrix& dataMN)
{
	dataMatrix::iterator pRow = dataM.begin();
	int rol_size = (*pRow).size();

	if(weight.size() < rol_size)
	{
		cout<<"weight size < rol size"<<endl;
		return false;
	}
	
	while(pRow != dataM.end())
	{
		int index = 0;
		vector<float> normal_row;
		while(index < rol_size)
		{
			if(dataS[index].range == 0)
				normal_row.push_back(0);
			else
				normal_row.push_back( (((*pRow)[index] - dataS[index].min) / dataS[index].range) * weight[index] );
			index++;
		}
		dataMN.push_back(normal_row);

		pRow++;
	}
	return true;
}

void print_dataMax(dataMatrix& dataM)
{
	dataMatrix::iterator pRow = dataM.begin();
	while(pRow != dataM.end())
	{
		vector<float>::iterator item = (*pRow).begin();
		cout<<*item;
		item++;
		while(item != (*pRow).end())
		{
			cout<<" "<<*item;
			item++;
		}
		cout<<endl;

		pRow++;
	}
}

/*
	normalize -w weight_file -i attr_file
*/
int main(int argn, char* argv[])
{
	if(argn != 5)
	{
		cout<<argv[0]<<" -w weight_file -i attr_file"<<endl;
		return 0;
	}

	string weight = "-w";
	string attr = "-i";
	if(weight == argv[1])
		weight = argv[2];
	else if(attr == argv[1])
		attr = argv[2];
	else
		cout<<"param error"<<endl;

	if(weight == argv[3])
		weight = argv[4];
	else if(attr == argv[3])
		attr = argv[4];
	else
		cout<<"param error"<<endl;

	vector<float> wMax;
	if( !get_weight_value(weight, wMax) )
	{
		cout<<"get weight value error"<<endl;
		return 0;
	}

	dataMatrix dataM;
	if( !file_to_matrix(attr, dataM) )
	{
		cout<<"file to matrix error"<<endl;
		return 0;
	}
	
	dataStatistics dataS;
	get_attr_statistics(dataM, dataS);

	dataMatrix dataMN;
	if( get_attr_normal(dataM, dataS, wMax, dataMN) )
		print_dataMax(dataMN);

	return 0;	
}
