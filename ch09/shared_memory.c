#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHMSIZE 1024
#define SHMNAME "/my_shm"
#define SHMKEY (key_t)0111
#define MAX_READ 1024
void main(int argc, char *argv[])
{
    int shmid, len;
    int pid;
    void *shmaddr;
    int src_fd; /* source file descriptor */
    int dst_fd; /* destination file descriptor */
    char buf[MAX_READ];
    ssize_t rcnt;                                        /* read count */
    ssize_t tot_cnt = 0;                                 /* total write count */
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; /* == 0644 */
    pid = fork();
    if (pid == 0)
    {
        // child
        if ((shmid = shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0666)) == -1)
        {
            perror("shmget failed");
            exit(1);
        }
        if ((shmaddr = shmat(shmid, NULL, 0)) == (void *)-1)
        {
            perror("shmat failed");
            exit(1);
        }

        printf("Writting\n");
        if ((dst_fd = creat("copy.txt", mode)) == -1)
        {
            perror("dst open"); /* errno에 대응하는 메시지 출력됨 */
            exit(1);
        }
        write(dst_fd, (char *)shmaddr, strlen((char *)shmaddr));
        close(dst_fd);

        if (shmdt(shmaddr) == -1)
        {
            perror("shmdt failed");
            exit(1);
        }
        if (shmctl(shmid, IPC_RMID, 0) == -1)
        {
            perror("shmctl failed");
            exit(1);
        }
    }
    else if (pid > 0)
    {
        if ((shmid = shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0666)) == -1)
        {
            perror("shmget failed");
            exit(1);
        }
        if ((shmaddr = shmat(shmid, NULL, 0)) == (void *)-1)
        {
            perror("shmat failed");
            exit(1);
        }

        if ((src_fd = open(argv[1], O_RDONLY)) == -1)
        {
            perror("src open"); /* errno에 대응하는 메시지 출력됨 */
            exit(1);
        }
        while ((rcnt = read(src_fd, buf, MAX_READ)) > 0)
        {
            strcpy((char *)shmaddr, buf);
        }
        if (shmdt(shmaddr) == -1)
        {
            perror("shmdt failed");
            exit(1);
        }
        close(src_fd);
    }
}
