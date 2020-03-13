#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>

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
        // this is child    
        char *argv[] = {"mkdir", "/home/afif/modul2/indomie", NULL};
        execv("/bin/mkdir", argv);
    } else {
        while ((wait(&status)) > 0);
        sleep(5);
        char *argv[] = {"mkdir", "/home/afif/modul2/sedaap", NULL};
        execv("/bin/mkdir", argv);
        }
  } 
  else {
    // this is parent
    while ((wait(&status)) > 0);
    char *argv[3] = {"unzip","jpg.zip", NULL};
    execv("/usr/bin/unzip", argv);
  }
}
