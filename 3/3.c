#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

void copy_file(char *src, char *dest) {
    FILE *fp1;
    FILE *fp2;
    struct stat st;
    int ch;
    fp1 = fopen(src, "r");
    fp2 = fopen(dest, "w");
 
    if (fp1 == NULL || fp2 == NULL) {
        printf("File does not exist!\n");
        exit(1);
    }

    stat(src, &st);
    printf("Pid - %d, file - %s, size - %ld\n", getpid(), src, st.st_size);
    
    while (1) {
        ch = fgetc(fp1);

        if (feof(fp1)) {
            break;
        }

        fputc(ch, fp2);
    }
}

int main(int argc[], char *argv[])
{
    DIR *dr1;
    DIR *dr2;
    struct dirent *d1;
    struct dirent *d2;
    int i, flag, flag_name, k, N, m, cur;
    pid_t pids[1024], pid_end;
    char file1[256], file2[256];
    
    dr1 = opendir (argv[1]); 
    dr2 = opendir (argv[2]); 
    N = atoi(argv[3]);

    i = 0;

    while ((d1 = readdir(dr1)) != NULL) {
        flag = 1;
        while ((d2 = readdir(dr2)) != NULL) 
        {
            for (flag_name = 1, k = 0; (d1->d_name[k] != '\0'); k++)
            {  
                if (d1->d_name[k] != d2->d_name[k]) flag_name = 0;
            }
            if ((flag_name == 1) && (d1->d_type == d2->d_type) && (d1->d_reclen == d2->d_reclen)) flag = 0;
        }

        rewinddir(dr2);

        if (flag)
        {
            if (i == N)
            {
                pid_end = wait(NULL);    
                for (m = 0; (m < N) && (pids[m] != pid_end); m++);
                cur = m;
            }
            else cur = i;

            pids[cur] = fork();
            if (!pids[cur]) 
            {
                strncpy(file1, "", sizeof(file1));
                strcpy(file1, argv[1]);
                strcat(file1, "/");
                strcat(file1, d1->d_name);
                strncpy(file2, "", sizeof(file2));
                strcpy(file2, argv[2]);
                strcat(file2, "/");
                strcat(file2, d1->d_name);
                copy_file(file1, file2);
                exit(0);
            }
        }
    }

    closedir (dr1);
    closedir (dr2);

    for (i = 0; i < N; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return 0;
}