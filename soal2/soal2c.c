#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void closeAll(int fd1[2], int fd2[2]) {
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
}

int main() {
    int fd1[2], fd2[2];
    char *ps_arg[] = {"ps", "aux", NULL};
    char *sort_arg[] = {"sort", "-nrk", "3,3", NULL};
    char *head_arg[] = {"head", "-5", NULL};

    pipe(fd1);
    pipe(fd2);

    if (fork() == 0) {
        printf("masuk ps\n\n");
        dup2(fd1[1], STDOUT_FILENO);
        closeAll(fd1,fd2);
        printf("sudah exec ps\n\n");
        execv("/bin/ps", ps_arg);
    } 
    else if (fork() == 0) {
        printf("masuk sort\n\n");
        dup2(fd1[0], STDIN_FILENO);
        dup2(fd2[1], STDOUT_FILENO);
        closeAll(fd1,fd2);
        printf("sudah exec sort\n\n");
        execv("/bin/sort", sort_arg);        
    } 
    else if (fork() == 0) {
        printf("masuk head\n\n");
        dup2(fd2[0], STDIN_FILENO);
        closeAll(fd1,fd2);
        printf("sudah exec head\n\n");
        execv("/bin/head", head_arg);
    }
    closeAll(fd1,fd2);

    int i, status;
    for (i = 0; i < 3; i++)
        wait(&status);
    return 0;
}

// STDOUT_FILENO = 1;
// STDIN_FILENO = 0;
// ps aux | sort -nrk 3,3 | head -5
