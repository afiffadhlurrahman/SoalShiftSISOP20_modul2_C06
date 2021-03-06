#include <stdio.h>
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
#include <wait.h>

int main() {
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

    while (1) {
        pid_t child_id;
        int status;  

        char buffer[80];
    	char b[]="/home/afif/sisop/shift2/";
        time_t rawtime;
	    struct tm *info;
        time( &rawtime );
        info = localtime( &rawtime );
        strftime(buffer,80,"%Y-%m-%d_%H:%M:%S", info);
        strcat(b,buffer);  	      

        child_id = fork();
        if (child_id < 0) {
            exit(EXIT_FAILURE);            
        }
      
        if (child_id == 0) {
            if(fork()==0){
                char *binaryPath = "/bin/mkdir";
                char *argv[] = {binaryPath, b, NULL};
                execv(binaryPath, argv);
            }
            else{
                while ((wait(&status)) > 0);
                int count=20;
                while(count>0){
                    if (fork() == 0)
                    {
                        chdir(b);
                        
                        struct tm t;
                        time_t t_of_day;
                        time_t s, val = 1;

                        struct tm* current_time;
                        s = time(NULL);
                        current_time = localtime(&s);
                        
                        t.tm_year = current_time->tm_year;          // Year - 1900
                        t.tm_mon = current_time->tm_mon;            // Month, where 0 = jan
                        t.tm_mday = current_time->tm_mday;          // Day of the month
                        t.tm_hour = current_time->tm_hour;
                        t.tm_min = current_time->tm_min;
                        t.tm_sec = current_time->tm_sec;
                        t.tm_isdst = -1;                    // Is DST on? 1 = yes, 0 = no, -1 = unknown
                        t_of_day = mktime(&t);
                        
                        t_of_day = (t_of_day%1000)+100;
                        char destination[1000]="https://picsum.photos/";
                        char buffer[1000];
                        
                        sprintf(buffer,"%ld",(long)t_of_day);
                        strcat(destination,buffer);
                        
                        time_t rawtime;
                        struct tm *info;
                        char name[80];
                        char namefile[80];

                        time( &rawtime );
                        info = localtime( &rawtime );

                        strftime(name,80,"%Y-%m-%d_%I:%M:%S", info);
                        strcat(namefile,name);

                        char *binaryPath1 = "/usr/bin/wget";
                        char *argv[] = {binaryPath1,"-P",b,"-qO",namefile, destination, NULL};
                        execv(binaryPath1, argv);
                    }
                    count--;
                    sleep(5);   
                }
                    //zip
            }
        }
        else{
            sleep(30);
        }            
    }
}
