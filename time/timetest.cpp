#include <time.h>
#include <stdio.h>
#include <locale>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>

using namespace std;

void print_tm(struct tm* t)
{
    printf("%d %d %d %d %d %d\n",t->tm_yday, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
}

void cpp_test()
{
    std::tm t = {};
    std::istringstream ss("2011-Februar-18 23:12:34");
    ss.imbue(std::locale("de_DE.utf-8"));
    ss >> std::get_time(&t, "%Y-%b-%d %H:%M:%S");
    if (ss.fail()) {
        std::cout << "Parse failed\n";
    } else {
        std::cout << std::put_time(&t, "%c") << '\n';
    }
}

int main(int argn, char* argv[])
{
    timespec time1, time2;
    time_t timer1, timer2;
    struct tm* t;

    /////////////////////////////////////
    //get cpu time from process start to this code
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    
    //////////////////////////////////////
    // local time
    timer1 = time(NULL);
    printf("%d => %s\n", timer1, ctime(&timer1));

    t = localtime(&timer1);
     print_tm(t);
    printf("%s\n", asctime(t));
    //printf("%d %d %s\n", timer2, time2);

    //t = getdate ("Wed May  4 04:52:05 2016");
     //print_tm(t);


    //c++
    cpp_test();
	return 0;
}
