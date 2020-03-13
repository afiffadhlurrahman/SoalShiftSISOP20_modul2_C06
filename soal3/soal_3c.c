#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>


int main() {

  pid_t child_id;
    int status;
    child_id = fork();
    
    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        pid_t child_id;
        int status;
        child_id = fork();
        
        if (child_id < 0) {
          exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
        }

        if (child_id == 0) {
          pid_t child_id;
          int status;
          child_id = fork();
          
          if (child_id < 0) {
              exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
          }

          if (child_id == 0) {
              // this is child    
              char *argv[] = {"mkdir", "/home/vincent/modul2/indomie", NULL};
              execv("/bin/mkdir", argv);
          } else {
              while ((wait(&status)) > 0);
              sleep(5);
              char *argv[] = {"mkdir", "/home/vincent/modul2/sedaap", NULL};
              execv("/bin/mkdir", argv);
              }
        } 
        else {
          // this is parent
          while ((wait(&status)) > 0);
          char *argv[3] = {"unzip","/home/vincent/modul2/jpg.zip", NULL};
          execv("/usr/bin/unzip", argv);
        }
    } 
    else {
      //this is parent
        while ((wait(&status)) > 0);
        char filename[100] ={"/home/vincent/modul2/jpg/"},filename2[100];
        struct dirent *dp;
        DIR *fd = opendir("/home/vincent/modul2/jpg/");
        if(fd==NULL) return 1;
        //ubah folder
        
        while((dp = readdir(fd))!=NULL){

          if (!strcmp (dp->d_name, ".")) continue;
          if (!strcmp (dp->d_name, ".."))continue;
          strcpy(filename2,filename);
          strcat(filename2,dp->d_name);

          pid_t child_id5;
          child_id5 = fork();

          if (child_id5 < 0) {
            exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
          }  
          if(child_id5 == 0 && dp->d_type == 4){
              char *arg4v[] = {"mv",filename2,"/home/vincent/modul2/indomie/",NULL};
              execv("/bin/mv",arg4v);
          }
          if(child_id5 == 0 && dp->d_type == 8){
               char *arg5v[] = {"mv",filename2,"/home/vincent/modul2/sedaap/",NULL};
              execv("/bin/mv",arg5v);
          }
        }
    }

  
}
