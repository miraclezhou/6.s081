#include "kernel/types.h"
#include "user/user.h"

void forkLoop(void){
    int p,n;
    int f_pipe[2];
    int pid;
    LOOP:
            if(read(0,&p,sizeof(int))>0){
                printf("prime %d\n",p);
            }else{
                close(0);
                exit(0);
            }
        pipe(f_pipe);
        pid=fork();
        if(pid<0){
            fprintf(2,"fork error!\n");
            exit(1);
        }
        if(pid==0){ /* child process */
            close(f_pipe[1]); /* close the write end.*/

            /* redirection */
            close(0);
            dup(f_pipe[0]);
            close(f_pipe[0]);
            goto LOOP;
        }
        else{ /* parent process */
            close(f_pipe[0]); /* close the read end.*/
            while(read(0,&n,sizeof(int))>0){ /* send the remaining nums to child.*/
                if(n%p!=0){
                    write(f_pipe[1],&n,sizeof(int));
                }
            }
            close(f_pipe[1]);
            close(0);
            wait(0); /* very important, wait for child's exit.*/
            exit(0); 
        }
          
}

int main(void){
    int n_pipe[2];
    pipe(n_pipe);
    int pid;
    pid=fork();
    if(pid<0){
        fprintf(2,"fork error!\n");
        exit(1);
    }
    if(pid==0){ /* child process */
        close(n_pipe[1]); /* close the write end.*/

        /* redirection */
        close(0);
        dup(n_pipe[0]);
        close(n_pipe[0]);

        /* fork the grandchild.*/
        forkLoop();
    }
    else{ /* parent process */
        int i;
        close(n_pipe[0]); /* close the read end.*/
        for(i=2;i<=35;i++){
            write(n_pipe[1],&i,sizeof(int)); /* send nums to child.*/
        }
        
        /* after write, close the write end.*/
        close(n_pipe[1]);
        wait(0);      
    }
    exit(0);
}