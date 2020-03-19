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
      // this is child    
    pid_t child_id;
    int status2;
    child_id = fork();
    
    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        pid_t child_id;
        int status3;
        child_id = fork();
        
        if (child_id < 0) {
          exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
        }

        if (child_id == 0) {
          pid_t child_id;
          child_id = fork();
          int status9;
          if (child_id < 0) {
              exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
          }

          if (child_id == 0) {
              // this is child    
              char *argv[] = {"mkdir", "/home/vincent/modul2/indomie", NULL};
              execv("/bin/mkdir", argv);
          } else {
              while ((wait(&status9)) > 0);
              sleep(5);
              char *argv[] = {"mkdir", "/home/vincent/modul2/sedaap", NULL};
              execv("/bin/mkdir", argv);
              }
        } 
        else {
          // this is parent
          while ((wait(&status3)) > 0);
          char *argv[3] = {"unzip","/home/vincent/modul2/jpg.zip", NULL};
          execv("/usr/bin/unzip", argv);
        }
    } 
    else {
      //this is parent
        while ((wait(&status2)) > 0);
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
  else {
        while ((wait(&status)) > 0);
        sleep(1);
        char filename3[100] ={"/home/vincent/modul2/indomie/"},filename4[100];
        struct dirent *dp;
        DIR *fd = opendir("/home/vincent/modul2/indomie/");
        if(fd==NULL) return 1;
        while((dp = readdir(fd)) !=NULL){
            if (!strcmp (dp->d_name, ".")) continue;
            if (!strcmp (dp->d_name, ".."))continue;
            
            pid_t child_id3;
            int status10;
            child_id3 = fork();
            if (child_id3 < 0) {
                exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
            }
            if(child_id3==0){
                strcpy(filename4,filename3);
                strcat(filename4,dp->d_name);
                strcat(filename4,"/coba1.txt");

                char *argv6[] = {"touch",filename4, NULL};
                execv("/usr/bin/touch", argv6);  
            }
            else{
                while ((wait(&status10)) > 0);
                sleep(3);
                pid_t child_id4;;
                child_id4=fork();
                if (child_id4 < 0) {
                    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
                }
                if(child_id4==0){
                    strcpy(filename4,filename3);
                    strcat(filename4,dp->d_name);
                    strcat(filename4,"/coba2.txt");
                    char *argv7[] = {"touch",filename4, NULL};
                    execv("/usr/bin/touch", argv7);
                }
            }
        }
  }  
}
