#include <unistd.h>
#include <fcntl.h>

#include <string>
#include <iostream>
using namespace std;

int exec_args_ext(const char* cmd, const char* param, string &output)
{
	int pipefd[2];
	pipe(pipefd);

	pid_t pid = fork();
	if (pid == -1)
	{ // fork error
		cout<<"[EXEC] exec_args fork error."<<endl;
		return -1;
	}
	else if (pid == 0)
	{ // child
		close(pipefd[0]);    // close reading end in the child
		dup2(pipefd[1], 1);  // send stdout to the pipe
		close(pipefd[1]);    // this descriptor is no longer needed

		int dev_null_fd = open("/dev/null", 0);
		if (dev_null_fd > 0)
		{
			dup2(dev_null_fd, 2);  // send stderr to /dev/null
			close(dev_null_fd);
		}

		execl(cmd, param);
	}
	else
	{ // parent
		close(pipefd[1]);    // close the write end of the pipe in the parent
		char buffer[1024] = {0};
		ssize_t len;
		while((len = read(pipefd[0], buffer, 1024)) > 0)
		{
			output += string(buffer, len);
			if (output.size() >= (256 * 1024)) // output no more than 256KB
				break;
		}
	}


	return -1;
}

int main()
{
	string output;
	exec_args_ext("/usr/bin/ls", "/", output);
	cout<<output<<endl;

	return 0;
}
