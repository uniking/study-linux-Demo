#ifndef _DEBUG_DEBUGGER_H
#define _DEBUG_DEBUGGER_H

#define r_eax 0
#define r_ecx 1
#define r_edx 2
#define r_ebx 3
#define r_esp 4
#define r_ebp 5
#define r_esi 6       
#define r_edi 7
#define r_eip 8
#define r_eflags 9
#define dr0 252
#define dr1 256
#define dr2 260
#define dr3 264
#define dr4 268
#define dr5 272
#define dr6 276
#define dr7 280

int OpenProcess(int pid);
int CloseProcess(int pid);
int ReadProcessMemory(int pid, char *BaseAddr, char* Buffer, unsigned long Size);
int WriteProcessMemory(int pid, char *BaseAddr, char* Buffer, unsigned long Size);


int ContinueProcess(int pid);
int SingleStepProcess(int pid);
int NextSyscallStopProcess(int pid);
int KillProcess(int pid);

unsigned int ReadProcessRegister(int pid, unsigned int reg);
int ReadProcessRegisterAll(int pid, struct user_regs_struct* regs);
int WriteProcessRegisterAll(int pid, struct user_regs_struct* regs);
unsigned int WriteProcessRegister(int pid, unsigned int reg);


#endif
