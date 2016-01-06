#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/reg.h> //#include <linux/user.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/ucontext.h>
#include <sys/procfs.h>
#include <string.h>

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

extern "C"
{
#include "./libdebug/mydebugger.h"
}

int main(int argc, char* argv[])
{
	int iRtn = 0;
	int pid = 0;

	cout<<"--------------------------"<<endl;
	cout<<"target process id="<<pid<<endl;
	cout<<"0 pid :to input target process id"<<endl;
	cout<<"1 open :to attach target process"<<endl;
	cout<<"2 reg :to show target process reg"<<endl;
	cout<<"3 exit :to exit this process"<<endl;
	cout<<"4 kill :to kill target process"<<endl;
	cout<<"5 close :to detach target process"<<endl;
	cout<<"6 cont :to continue target process"<<endl;
	cout<<"7 single : to single step target process"<<endl;
	cout<<"8 syscall ：next syscall top target process"<<endl;
	cout<<"9 readreg ：read target process register"<<endl;
	cout<<"--------------------------"<<endl;

	
	string iStr;
	while(true)
	{
		cin>>iStr;
		cout<<iStr<<endl;
		const char* pAgr = iStr.c_str();
		if(0 == strcmp(pAgr, "pid"))
		{
			cout<<"please input a process id:";
			string inPid;
			cin>>inPid;
			pid = atoi(inPid.c_str());
			cout<<"input pid="<<pid<<endl;
		}
		else if(0 == strcmp(pAgr, "open"))
		{
			cout<<"input open"<<endl;
			if(0 == OpenProcess(pid))
			{
				cout<<"OpenProcess ok"<<endl;
			}
			else
			{
				cout <<"OpenProcess error"<<endl;
				break;
			}
		}
		else if(0 == strcmp(pAgr, "exit"))
		{
			cout<<"input exit"<<endl;
			break;
		}
		else if(0 == strcmp(pAgr, "cont"))
		{
			cout<<"input cont"<<endl;
			if(0 == ContinueProcess(pid))
			{
				cout<<"ContinueProcess ok"<<endl;
			}
			else
			{
				cout <<"ContinueProcess error"<<endl;
				break;
			}
		}
		else if(0 == strcmp(pAgr, "close"))
		{
			cout<<"input exit"<<endl;
			if(0 == CloseProcess(pid))
			{
				cout<<"CloseProcess ok"<<endl;
			}
			else
			{
				cout <<"CloseProcess error"<<endl;
				break;
			}
		}
		else if(0 == strcmp(pAgr, "kill"))
		{
			cout<<"input exit"<<endl;
			if(0 == KillProcess(pid))
			{
				cout<<"KillProcess ok"<<endl;
			}
			else
			{
				cout <<"KillProcess error"<<endl;
				break;
			}
		}
		else if(0 == strcmp(pAgr, "single"))
		{
			cout<<"input single"<<endl;
			if(0 == SingleStepProcess(pid))
			{
				cout<<"SingleStepProcess ok"<<endl;
			}
			else
			{
				cout <<"SingleStepProcess error"<<endl;
				break;
			}
		}
		else if(0 == strcmp(pAgr, "syscall"))
		{
			cout<<"input syscall"<<endl;
			if(0 == NextSyscallStopProcess(pid))
			{
				cout<<"NextSyscallStopProcess ok"<<endl;
			}
			else
			{
				cout <<"NextSyscallStopProcess error"<<endl;
				break;
			}
		}
		else if(0 == strcmp(pAgr, "readreg"))
		{
			cout<<"input syscall"<<endl;
			struct user_regs_struct regs;
			if(0 == ReadProcessRegisterAll(pid, &regs))
			{
				cout<<"rax:0x"<<hex<<regs.rax<<endl;
				cout<<"rbx:0x"<<hex<<regs.rbx<<endl;
				cout<<"rcx:0x"<<hex<<regs.rcx<<endl;
				cout<<"rdx:0x"<<hex<<regs.rdx<<endl;
				cout<<"rdi:0x"<<hex<<regs.rdi<<endl;
				cout<<"rsi:0x"<<hex<<regs.rsi<<endl;
				cout<<"rsp:0x"<<hex<<regs.rsp<<endl;
				cout<<"rbp:0x"<<hex<<regs.rbp<<endl;
				cout<<"rip:0x"<<hex<<regs.rip<<endl;
			}
			else
			{
				cout <<"NextSyscallStopProcess error"<<endl;
				break;
			}
		}



		else
		{
			cout<<"unknow command\n"<<endl;
		}


	}

	return iRtn;
}
