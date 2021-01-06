#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv[]){
    char buf[20];
    int p_pipe[2],c_pipe[2];
    int cpid;
    //if pipe failed
    if(pipe(p_pipe)==-1 || pipe(c_pipe)==-1){
        fprintf(2,"pipe error!\n");
        exit(1);
    }
    cpid = fork();
    if(cpid<0){
        fprintf(2,"fork error!\n");
        exit(1);
    }
    if(cpid==0){
        close(p_pipe[1]);
        close(c_pipe[0]);
        read(p_pipe[0],buf,1);
        close(p_pipe[0]);
        printf("%d: received ping\n",getpid());
        write(c_pipe[1],"i",1);
        close(c_pipe[1]);
    }else{
        close(p_pipe[0]);
        close(c_pipe[1]);
        write(p_pipe[1],"m",1);
        read(c_pipe[0],buf,1);
        close(p_pipe[1]);
        close(c_pipe[0]);
        printf("%d: received pong\n",getpid());
        
    }
    exit(0);
}