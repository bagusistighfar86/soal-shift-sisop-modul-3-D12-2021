#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int pid;
int fd1[2];
int fd2[2];

void ps_comm();
void sort_comm();
void head_comm();

char *ps_arg[] = {"ps", "aux", NULL};
char *sort_arg[] = {"sort", "-nrk", "3,3", NULL};
char *head_arg[] = {"head", "-5", NULL};

int main(){

    // create fd1
    if (pipe(fd1) == -1){
        perror("failed create pipe1");
        exit(1);
    }

    if ((pid = fork()) == -1){
        exit(1);
    }
    else if (pid == 0){
        ps_comm();
    }

    if (pipe(fd2) == -1){
        perror("failed create pipe2");
        exit(1);
    }

    if ((pid = fork()) == -1){
        exit(1);
    }
    else if (pid == 0){
        sort_comm();
    }

    close(fd1[0]);
    close(fd1[1]);

    if ((pid = fork()) == -1){
        exit(1);
    }
    else if (pid == 0){
        head_comm();
    }

    int i, status;
    for (i = 0; i < 3; i++)
        wait(&status);
    return 0;
}

void ps_comm(){

    // output ke fd1
    dup2(fd1[1], 1);

    close(fd1[0]);
    close(fd1[1]);

    // exec ps
    execv("/bin/ps", ps_arg);
    _exit(1);
}

void sort_comm(){
    // input dari fd1
    dup2(fd1[0], 0);
    // output ke fd2
    dup2(fd2[1], 1);

    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);

    execv("/bin/sort", sort_arg);   
    _exit(1);
}

void head_comm(){
    // input dari fd2
    dup2(fd2[0], 0);

    close(fd2[0]);
    close(fd2[1]);

    execv("/bin/head", head_arg);
    _exit(1);
}

