#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void find(char* path,char* thing){
    char buf[512],*p;
    int fd;
    struct dirent dt;
    struct stat st;

    // Open the current diractory. 
    if((fd=open(path,0))<0){
        fprintf(2,"Open current path error!\n");
        close(fd);
    }

    while(read(fd,&dt,sizeof(dt)) == sizeof(dt)){
        // Update path.
        strcpy(buf,path);
        p=buf+strlen(buf);
        *p++ = '/';

        // Empty dirent, skip.
        if(dt.inum == 0){
            continue;
        }

        // Get the full pathname.
        memmove(p,dt.name,DIRSIZ);
        p[DIRSIZ] = 0;

        // Stat the dirent.
        if(stat(buf,&st) < 0){
            fprintf(2,"stat error!\n");
            continue;
        }

        switch(st.type){
        // If type is file, compare the file's name with the thing; if same, print it.
        case T_FILE:
            if(!strcmp(dt.name,thing)){
                printf("%s\n",buf);
            }
            break;
        
        case T_DIR:
            // Don't recurse into "." and "..".
            if(strcmp(dt.name,".")  &&  strcmp(dt.name,"..")){
                find(buf,thing);
            }
            break;
        }
    }

    // Don't forget to close after usage.
    close(fd);
    return;
}

int main(int argc,char *argv[]){
    // Check the argument.
    if(argc < 3){
        fprintf(2,"need more argument!\n");
        exit(1);
    }
    int i;
    for(i = 2;i < argc;i ++){
        find(argv[1],argv[i]);
    }
    exit(0);
}