#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/wait.h>
#include<unistd.h>

int main(){
    int p[2], p1[2], status, status1;
    pid_t pp, pp2;
    if(pipe(p) == -1)
    {
        exit(EXIT_FAILURE);
    }
    if (pipe(p1)==-1){
        exit(EXIT_FAILURE);
    }
    pp2 = fork();
    if (pp2<0) exit(EXIT_FAILURE);
    if (pp2==0){
        close(p[0]);
        dup2(p[1], STDOUT_FILENO);
        char *argv[] = {"ps", "aux", NULL};
        execv("/bin/ps", argv);
    }
    while(wait(&status)>0);
    close(p[1]);
    dup2(p[0], STDIN_FILENO);
    
    pp = fork();
    if(pp < 0){
        exit(EXIT_FAILURE);
    }
    if(pp==0){
        close(p1[0]);
        dup2(p1[1], STDOUT_FILENO);
        char* argv[]={"sort", "-nrk", "3,3", NULL};
        execv("/bin/sort", argv);
    }

    while(wait(&status1)>0);
    close(p1[1]);
    dup2(p1[0], STDIN_FILENO);
    char* argv[]={"head", "-5", NULL};
    execv("/bin/head", argv);
}