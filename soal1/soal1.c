#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

int main(int argc, char** argv){

    bool secBintang=false, minBintang=false, hourBintang=false,check=true;
    time_t currTime = time(NULL);

    struct tm tm = *localtime(&currTime);
    int sec,min,hour;

    if(argc != 5){
        printf("argumen tidak sesuai!\n");
        return 0;
    } 

    if(strcmp(argv[1],"*")==0)secBintang=true;
    else sec = atoi(argv[1]);

    if(strcmp(argv[2],"*")==0)minBintang=true;
    else min = atoi(argv[2]);

    if(strcmp(argv[3],"*")==0)hourBintang=true;
    else hour = atoi(argv[3]);

    if((sec<0 || sec>59) && !secBintang && check) {
        printf("argumen pertama tidak dalam range (0-59)!\n");
        check=false;
    }
    if((min<0 || min>59) && !minBintang && check){ 
        printf("argumen kedua tidak dalam range (0-59)!\n");
        check=false;
    }
    if((hour<0 || hour>23) && !hourBintang && check) {
        printf("argumen ketiga tidak dalam range (0-23)!\n");
        check=false;
    }
    if(!check) return 0;

    //daemon process begin!--------------------------------------------
    pid_t pid, sid;        // Variabel untuk menyimpan PID

    pid = fork();     // Menyimpan PID dari Child Process

    /* Keluar saat fork gagal
    * (nilai variabel pid < 0) */
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
    * (nilai variabel pid adalah PID dari child process) */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
   //daemon process end!--------------------------------------------
    
    while(1){
        if((tm.tm_hour == hour || hourBintang) && (tm.tm_min == min || minBintang)&& (tm.tm_sec == sec || secBintang)){
            pid_t child_id1;
            child_id1 = fork();
            if (child_id1 < 0) {
            exit(EXIT_FAILURE);
            }  
            if(child_id1==0){
                execl("/bin/bash","bash",argv[4],NULL);
            }
        }
        sleep(1);
    }
    
}
