#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#define MAX_PATH 256
/*
* 주어진 디렉토리 내에 존재하는 파일과 디렉토리를 나열하고, 
* 디렉토리의 경우 재귀적으로 방문해서 그 디렉토리 내에 존재하는 파일과 디렉토리를 나열하는 프로그램을 작성하시오. 
* 즉, “ls –R” 명령과 동일한 결과를 보이도록 하시오.
*/

void search(char *argv)
{
    DIR *pdir;
    struct dirent *pde;
    struct stat buf;
    int i = 0, count = 0;
    char temp[1024][MAX_PATH];
    printf("\n%s\n", argv);

    if ((pdir = opendir(argv)) < 0)
    {
        perror("OPENDIR");
        exit(1);
    }
    chdir(argv);
    while ((pde = readdir(pdir)) != NULL)
    {
        lstat(pde->d_name, &buf);
        if (S_ISDIR(buf.st_mode))
        {
            if (strcmp(pde->d_name, "..") && strcmp(pde->d_name, "."))
            {
                printf("%-20s", pde->d_name);
                if (++i % 3 == 0) printf("\n");
                sprintf(temp[count], "%s/%s", argv, pde->d_name);
                count++;
            }
        }
        else if (S_ISREG(buf.st_mode))
        {
            printf("%-20s", pde->d_name);
            if (++i % 3 == 0) printf("\n");
        }
    }
    printf("\n");
    for (i = 0; i < count; i++)
    {
        search(temp[i]);
    }
    
    closedir(pdir);
}

int main(int argc, char *argv[])
{
    char path[MAX_PATH];

    if (argc < 2)
    {
        fprintf(stderr, "PLEASE ENTER A DIRECTORY.");
        exit(1);
    }
    search(argv[1]);

    return 0;
}
