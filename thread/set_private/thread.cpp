#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <assert.h>
#include <unistd.h>

using namespace std;

static int get_thread_policy( pthread_attr_t &attr )
{
        int policy;
        int rs = pthread_attr_getschedpolicy( &attr, &policy );
        assert( rs == 0 );

	cout<<"get_thread_policy ";
        switch ( policy )
        {
        case SCHED_FIFO:
                cout << "policy = SCHED_FIFO" << endl;
                break;

        case SCHED_RR:
                cout << "policy = SCHED_RR" << endl;
                break;

        case SCHED_OTHER:
                cout << "policy = SCHED_OTHER" << endl;
                break;

        default:
                cout << "policy = UNKNOWN" << endl;
                break;
        }

        return policy;
}

static void set_thread_policy( pthread_attr_t &attr,  int policy )
{
        int rs = pthread_attr_setschedpolicy( &attr, policy );
        assert( rs == 0 );
        get_thread_policy( attr );
}

static void show_thread_priority( pthread_attr_t &attr, int policy )
{
        int priority = sched_get_priority_max( policy );
        assert( priority != -1 );
        cout << "show_thread_priority max_priority = " << priority << endl;

        priority = sched_get_priority_min( policy );
        assert( priority != -1 );
        cout << "show_thread_priority min_priority = " << priority << endl;
}

static int get_thread_priority( pthread_attr_t &attr )
{
        struct sched_param param;

        int rs = pthread_attr_getschedparam( &attr, &param );
        assert( rs == 0 );
        cout << "get_thread_priority priority = " << param.__sched_priority << endl;

        return param.__sched_priority;
}

static int set_thread_priority( pthread_attr_t &attr , int priority)
{
        struct sched_param param;
	param.__sched_priority = priority;

        int rs = pthread_attr_setschedparam( &attr, &param );
        assert( rs == 0 );
        cout << "set_thread_priority priority = " << param.__sched_priority << endl;

        return param.__sched_priority;
}



int main( void )
{
        pthread_attr_t attr;
        struct sched_param sched;
        int rs;

        rs = pthread_attr_init( &attr );
        assert( rs == 0 );

        int policy = get_thread_policy( attr );

        cout << "Show current configuration of priority" << endl;
        show_thread_priority( attr, policy );

        cout << "Show SCHED_FIFO of priority" << endl;
        show_thread_priority( attr, SCHED_FIFO );

        cout << "Show SCHED_RR of priority" << endl;
        show_thread_priority( attr, SCHED_RR );

        cout << "Show priority of current thread" << endl;
        int priority = get_thread_priority( attr );

        cout << "Set thread policy" << endl;
        cout << "Set SCHED_FIFO policy" << endl;
        set_thread_policy( attr, SCHED_FIFO );
	set_thread_priority(attr, 40);
	priority = get_thread_priority( attr );

	sleep(300);
	

        cout << "Set SCHED_RR policy" << endl;
        set_thread_policy( attr, SCHED_RR );
        cout << "Restore current policy" << endl;
        set_thread_policy( attr, policy );


        rs = pthread_attr_destroy( &attr );
        assert( rs == 0 );

        return 0;
}
