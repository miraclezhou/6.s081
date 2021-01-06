#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"




int main(int argc,char* argv[]){
    char* cmd_args[MAXARG];
    char input_buf[100];
    char buf[MAXARG][100];
    int pid,args,i;

    
    // Check arguments.
    if(argc<2){
        fprintf(2,"Need more arguments!\n");
        exit(1);
    }

    args=argc-1;
    // Get cmdline.

    for(i=0;i<args;i++){
        cmd_args[i]=argv[i+1];
    }
    read(0,input_buf,100);

    int count=0;
    int n=0;
    for(i=0;i<strlen(input_buf)-1;i++,count++){
        if(input_buf[i]!='\n' && input_buf[i]!=' '){
            buf[n][count]=input_buf[i];
        }else{
            buf[n][i]=0;

            cmd_args[args]=buf[n];

            args++;
            count=-1;
            n++;
        }
    }
    buf[n][i]=0;
    cmd_args[args]=buf[n];
    args++;
    cmd_args[args]=0;
        pid=fork();
        if(pid<0){
            fprintf(2,"fork error!\n");
            exit(1);
        }
        if(pid==0){

            exec(cmd_args[0],cmd_args);
            exit(0);
        }
    
    wait(0);
    close(0);
    exit(0);
}
