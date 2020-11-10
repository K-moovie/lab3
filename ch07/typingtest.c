#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#define PASSWORDSIZE 12
int main(void)
{
    int fd;
    int nread, cnt = 0, errcnt = 0, i = 0, total_cnt = 0;
    char ch, text[3][100] = {{"Too many cooks spoil the broth."}, {"Don’t judge a book by its cover."}, {"Strike while the iron is hot."}};
    struct termios init_attr, new_attr;
    time_t t_start, t_end;
    double t_diff = 0;

    for (i = 0; i < 3; i++)
    {
        fd = open(ttyname(fileno(stdin)), O_RDWR);
        tcgetattr(fd, &init_attr);
        new_attr = init_attr;
        new_attr.c_lflag &= ~ICANON;
        new_attr.c_lflag &= ~ECHO; /* ~(ECHO | ECHOE | ECHOK | ECHONL); */
        new_attr.c_cc[VMIN] = 1;
        new_attr.c_cc[VTIME] = 0;
        if (tcsetattr(fd, TCSANOW, &new_attr) != 0)
        {
            fprintf(stderr, "터미널 속성을 설정할 수 없음.\n");
        }
        printf("다음 문장을 그대로 입력하세요.\n%s\n", text[i]);
        time(&t_start);
        while ((nread = read(fd, &ch, 1)) > 0 && ch != '\n')
        {
            if (ch == text[i][cnt++])
                write(fd, &ch, 1);
            else
            {
                write(fd, "*", 1);
                errcnt++;
            }
        }
        printf("타이핑 오류의 횟수는 %d\n", errcnt);
        time(&t_end);
        tcsetattr(fd, TCSANOW, &init_attr);
        t_diff += difftime(t_end, t_start);
        total_cnt += cnt;
        total_cnt += errcnt;
        cnt = errcnt = 0;
        close(fd);
    }
    printf("분당 타자 %f타\n", (total_cnt * 60 / t_diff) / 3);
}

