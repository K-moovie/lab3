#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <memory.h>

void main(int argc, char *argv[])
{

    pid_t pid;
    int status, exit_status;
    int i = 0;
    char cmd[1024] = {0};
    for (i = 1; i < argc; i++)
    {
        strcat(cmd, " ");
        strcat(cmd, argv[i]);
    }
    pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
    }
    if (pid == 0)
    {
        execl("/bin/bash", "bin/bash", "-c", cmd, NULL);
    }
    if (pid > 0)
    {
        waitpid(pid, &status, 0);
    }
    exit(0);
}
