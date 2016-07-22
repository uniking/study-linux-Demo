#include "FileHash.h"

#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/inotify.h>

#include <FuzzyHash.h>

#include "file_parse/file_parser.h"
#include "ndlphelper/ndlp_helper.hpp"
#include "file_parse/content_parser.h"

#include "json/json.h"

#include <string>
#include <vector>
#include <iostream>
using namespace std;

bool inotify_dir(const char* dir)
{
    int fd = 0;
    int nwd = 0;
    char buffer[1024*2];

    do
    {
        fd = inotify_init();
        if(-1 == fd)
        {
            cout<<"inotify error"<<endl;
            break;
        }

        nwd = inotify_add_watch(fd, dir, IN_CREATE | IN_MODIFY);
        if(-1 == nwd)
        {
            cout<<"inotify_add_watch error"<<endl;
            break;
        }

        while(true)
        {
            read(fd, buffer, 1024*2);
            struct inotify_event* in_ev = (struct inotify_event*)buffer;
            cout<<in_ev->name<<endl;
        }
        

        inotify_rm_watch(fd, nwd);
        close(fd);
    }
    while(false);


}

void *thr_monitor_file_hash(void* arg)
{
    const char* dir = (const char*) arg;

    inotify_dir(dir);
}

void *thr_get_file_hash(void* arg)
{
    const char* dir = (const char*) arg;

    while(true)
    {
        CDiscover::collect_file_hash(string& dir_path);
    }
}

bool create_dir_monitor_thread(const char* dir)
{
    pthread_t ntid;
    pthread_t ntid2;

    if(pthread_create(&ntid, NULL, thr_monitor_file_hash, dir) != 0)
    {
        printf("create_dir_monitor_thread pthread_create error\n");
        return false;
    }

    if(pthread_create(&ntid2, NULL, thr_get_file_hash, dir) != 0)
    {
        printf("create_dir_monitor_thread pthread_create error\n");
        return false;
    }

    return true;
}
