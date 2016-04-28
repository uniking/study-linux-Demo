#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

using namespace std;

#pragma pack(1)
typedef struct _file_in_file
{
    char flag;
    unsigned int file_long;
}file_in_file;


#pragma pack()

void h2n(unsigned int fl, unsigned int& nfl)
{
    char new_buf1[4];
    char new_buf2[4];
    unsigned int* p_new = (unsigned int*)new_buf1;
    *p_new = fl;
    
    new_buf2[3] = new_buf1[0];
    new_buf2[2] = new_buf1[1];
    new_buf2[1] = new_buf1[2];
    new_buf2[0] = new_buf1[3];

    p_new = (unsigned int*)new_buf2;
    nfl = *p_new;
}
void create_file(const char* buffer, unsigned int file_length, const char* file_name)
{
    unsigned int new_l;
    h2n(file_length, new_l);
    ofstream outfile(file_name);
    outfile.write(buffer, new_l);
}

/*
return nexit file pointer
*/
const char* find_file(const char* buffer, const char* file_name)
{
    const char* next_file = NULL;
    unsigned int new_l;

    file_in_file* p = (file_in_file*)buffer;
    h2n(p->file_long, new_l);

    if(p->flag == 0)
    {
        create_file(buffer + 5, new_l, file_name);
        next_file = buffer + 5 +new_l;
    }
    else if(p->flag == 1)
    {
        next_file = buffer + 5 +new_l;
    }
    else if(p->flag == 2)
    {
        next_file = NULL;
    }

    return next_file;
}

int main(int argn, char* argv[])
{

cout<<sizeof(file_in_file)<<endl;
    if(argn != 2)
    {
        cout<<argv[0]<<" filename"<<endl;
        return 0;
    }

   ifstream infile(argv[1]);
	string sContent;
	ostringstream os;
	os << infile.rdbuf();
	sContent = os.str();

    const char* buffer= sContent.c_str();
    int c=0;
    char name_buf[128];
    while(true)
    {
       sprintf(name_buf, "file_%d.png", c);
        buffer = find_file(buffer, name_buf);
        if(buffer == NULL)
            break;
        if(sContent.size() <= (buffer - sContent.c_str()) )
            break;
        c++;
    }
    
}
