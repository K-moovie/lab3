/* sendmq.c */
/* message queue example */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#define BUFSIZE 16
#define QKEY (key_t)0111

typedef struct _msgq_data
{
    long type;
    char text[BUFSIZE];
} msgq_data;

int main()
{
    int qid, len, i;
    char buf[BUFSIZE];

    while (1)
    {
        if ((qid = msgget(QKEY, IPC_CREAT | 0666)) == -1)
        {
            perror("msgget failed");
            exit(1);
        }
        msgq_data *send_data = (msgq_data *)malloc(sizeof(msgq_data));
        send_data->type = 1;
        printf("전송할 메시지를 입력하세요.(q입력시 종료):");
        scanf("%s", buf);
        strcpy(send_data->text, buf);
        printf("%d", strlen(send_data->text));
        if (msgsnd(qid, &send_data, strlen(send_data->text), 0) == -1)
        {
            perror("msgsnd failed");
            exit(1);
        }
        if (!strcmp(buf, "q"))
        {
            break;
        }
        free(send_data);
    }
}
