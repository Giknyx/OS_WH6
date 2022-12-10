#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc[], char *argv[])
{
    pid_t pid1, pid2;
    struct timeval time_now;
    struct tm *time_str_tm;

    pid1 = fork();
    if (pid1 == 0) {
        gettimeofday(&time_now, NULL);
        time_str_tm = gmtime(&time_now.tv_sec);
        printf ("Это Дочерний процесс, pid=%d, %02i:%02i:%02i:%06li\n", getpid(),
            time_str_tm->tm_hour, time_str_tm->tm_min, time_str_tm->tm_sec, time_now.tv_usec); 
    }
    else if (pid1 > 0) {
        gettimeofday(&time_now, NULL);
        time_str_tm = gmtime(&time_now.tv_sec);
        printf ("Это Родительский процесс, pid=%d, %02i:%02i:%02i:%06li\n", getpid(),
            time_str_tm->tm_hour, time_str_tm->tm_min, time_str_tm->tm_sec, time_now.tv_usec);
        pid2 = fork();

        if (pid2 == 0) {
            gettimeofday(&time_now, NULL);
            time_str_tm = gmtime(&time_now.tv_sec);
            printf ("Это Дочерний процесс, pid=%d, %02i:%02i:%02i:%06li\n", getpid(),
            time_str_tm->tm_hour, time_str_tm->tm_min, time_str_tm->tm_sec, time_now.tv_usec);
        }
        else if (pid2 < 0) 
        {
            printf ("Ошибка вызова fork, потомок не создан\n");
        }
    }
    else
        printf ("Ошибка вызова fork, потомок не создан\n"); 

    
    if (pid1 > 0){
        int status;
        wait(&status);
        system("ps -x");
    }
}