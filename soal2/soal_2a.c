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
        child_id = fork();

        if (child_id == 0) {
            char b[]="/home/afif/sisop/shift2/";
            char c[1000];
            struct tm ti;
            time_t t_of_day;
            time_t s, val = 1;
            struct tm* current_time;
            
            s = time(NULL);
            current_time = localtime(&s);
            
            ti.tm_year = current_time->tm_year;     // Year - 1900
            ti.tm_mon = current_time->tm_mon;       // Month, where 0 = jan
            ti.tm_mday = current_time->tm_mday;     // Day of the month
            ti.tm_hour = current_time->tm_hour;
            ti.tm_min = current_time->tm_min;
            ti.tm_sec = current_time->tm_sec;
            ti.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
            t_of_day = mktime(&ti);

            sprintf( c, "%ld", (long) t_of_day );
            strcat(b,c);
            char *binaryPath = "/bin/mkdir";
            char *argv[] = {binaryPath, b, NULL};
            execv(binaryPath, argv);
        }
        else{
            while ((wait(&status)) > 0);
        }            
        sleep(30);
    }
}
