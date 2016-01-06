#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/reg.h> //#include <linux/user.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/ucontext.h>
#include <sys/procfs.h>

int main()
{
	pid_t child;
	long orig_eax;
	child = fork();
	if(child == 0)
	{
		sleep(1);
		if(-1 == ptrace(PTRACE_TRACEME, 0, NULL, NULL))
			printf("ptrace error\n");
		else
			execl("/bin/ls", "ls", NULL);
	}
	else
	{
		wait(NULL);
		orig_eax = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
		printf("The child made a system call %ld\n", orig_eax);

		struct user_regs_struct regs;
		ptrace(PTRACE_GETREGS, child, NULL, &regs);
		printf("syscall with eax:%ld ebx:%ld ecx:%ld edx:%ld eip:%ld esp:%ld ebp=%ld\n",
				regs.eax,
				regs.ebx,
				regs.ecx,
				regs.edx,
				regs.eip,
				regs.esp,
				regs.ebp);





		ptrace(PTRACE_CONT, child, NULL, NULL);
	}

	return 0;
}
