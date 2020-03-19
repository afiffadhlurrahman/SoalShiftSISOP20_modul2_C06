# SoalShiftSISOP20_modul2_C06
- Muhammad Afif Fadhlurrahman 05111840000093
- Vincentius Tanubrata 05111840000091

## Soal 1
Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan
ketentuan sebagai berikut:
a. Program menerima 4 argumen berupa:
i. Detik: 0-59 atau * (any value)
ii. Menit: 0-59 atau * (any value)
iii. Jam: 0-23 atau * (any value)
iv. Path file .sh
b. Program akan mengeluarkan pesan error jika argumen yang diberikan tidak
sesuai
c. Program hanya menerima 1 config cron
d. Program berjalan di background (daemon)
e. Tidak boleh menggunakan fungsi system()

Contoh: ./program \* 34 7 /home/somi/test.sh
Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap
detik pada jam 07:34.

### Penyelesaian no.1 
Soal no1 merupakan program menyerupai crontab dengan menggunakan daemon pada c
```
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
    	time_t currTime = time(NULL);

        struct tm tm = *localtime(&currTime);
        if((tm.tm_hour == hour || hourBintang) && (tm.tm_min == min || minBintang)
          && (tm.tm_sec == sec || secBintang)){
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
```
#### INPUT
program ini menggunakan argumen untuk inputnya (detik,menit,jam,dan path file), maka dari itu kita menggunakan `int main(int argc, char** argv)` , dimana `argc` untuk mengetahui jumlah argumen, dan `argv` untuk mengetahui argumen apa yang diinputkan.

#### ERROR HANDLING
argumen yang kita inputkan terdapat 4 buah,yang berupa detik(0-59) atau *(any value), menit(0-59) atau *(any value), jam (0-23) atau *(any value), dan yang terakhir path file bash.sh. Untuk mengatasi kesalahan dalam input, kita bisa melakukan error handling sebagai berikut 
```
if(argc != 5){
        printf("argumen tidak sesuai!\n");
        return 0;
    } 
```
error handling diatas bertujuan agar input yang diberikan haruslah 4, kenapa pada kodingan diatas `argc!=5`? karena argumen pertama berupa `./soal1`, sehingga argumen untuk input mulai pada argumen ke 1-5 (total 4 argumen)

```
if(strcmp(argv[1],"*")==0)secBintang=true;
    else sec = atoi(argv[1]);

if(strcmp(argv[2],"*")==0)minBintang=true;
    else min = atoi(argv[2]);

if(strcmp(argv[3],"*")==0)hourBintang=true;
    else hour = atoi(argv[3]);
```
pada kodingan diatas, kita merubah argumen menjadi integer dengan menggunakan fungsi `atoi`, kita merubah argumen pertama menjadi detik, argumen kedua menjadi menit, dan argumen ketiga menjadi jam. namun jika inputan berupa `*`, kita tidak merubahnya menjadi integer, melainkan melakukan flaging pada detik/menit/jam yang inputnya berupa `*` tadi.
 
Kemudian seteah membuat menjadi integer, kita lakukan error handling lagi jika argumen tidak berada pada batasan yang ditentukan seperti berikut.
```
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
```
kemudian jika argumen tidak sesuai, kita ubah `check` menjadi false dan return 0 yang berarti program akan berhenti
```
if(!check) return 0;
```
#### CURRENT TIME

untuk mendapatkan waktu saat ini, kita menggunakan kita fungsi yang terdapat di library `time.h` seperti berikut :
```
time_t currTime = time(NULL);
struct tm tm = *localtime(&currTime);
```
#### DAEMON PROCESS
Karena process ini berjalan setiap waktu tertentu ( seperti crontab ), kita perlu menggunakan daemon sebagai process yang berjalan pada background
```
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
```
#### MAIN PROGRAM
program ini berjalan pada waktu tertenu dengan menggunakan daemon yang berjalan pada background. program ini akan mengecek apakah waktu sudah sesuai dengan konfigurasi cron yang diberikan (dari argumen), Jika sudah sesuai maka program bash akan jalan.

untuk melakukan ini, kita perlu melakukan checking dalam `while(1)` di process daemon dan process checkingnya adalah sebagai berikut,
```
if((tm.tm_hour == hour || hourBintang) && (tm.tm_min == min || minBintang)
          && (tm.tm_sec == sec || secBintang)){
            pid_t child_id1;
            child_id1 = fork();
            if (child_id1 < 0) {
            exit(EXIT_FAILURE);
            }  
            if(child_id1==0){
                execl("/bin/bash","bash",argv[4],NULL);
            }
        }
```
Kita melakukan check apakah jam saat/menit/detik saat ini sama dengan yang kita input pada argumen. dan jika `hourBintang` bernilai true, maka artinya kita telah menginputkan `*` pada konfigurasi jam dan yang berarti akan berjalan tiap jam. ini juga berlaku untuk `minBintang` dan `secBintang`.

Jika semuanya sudah terpenuhi, maka kita bisa mengeksekusi program bash dengan menggunakan `exec`. Namun sebelum melakukan `exec`, kita harus melakukan `fork()` karena seperti yang kita ketahui, `exec` akan membuat program berhenti setelah melakukan eksekusi. Maka dari itu kita harus menjalankannya di child. 
```
pid_t child_id1;
child_id1 = fork();
```
Setelah ini, kita bisa melakukan fungsi `exec` dalam child `if(child_id1==0)` sebagai berikut
```
execl("/bin/bash","bash",argv[4],NULL);
```
yang berarti kita menjalankan bash script yang pathnya merupakan `argv[4]`.

![Input dan Hasil](https://github.com/afiffadhlurrahman/SoalShiftSISOP20_modul2_C06/tree/master/images/Capture8.png)

## Soal 2
Shisoppu mantappu! itulah yang selalu dikatakan Kiwa setiap hari karena sekarang dia
merasa sudah jago materi sisop. Karena merasa jago, suatu hari Kiwa iseng membuat
sebuah program.
a. Pertama-tama, Kiwa membuat sebuah folder khusus, di dalamnya dia membuat
sebuah program C yang per 30 detik membuat sebuah folder dengan nama
timestamp [YYYY-mm-dd_HH:ii:ss].
b. Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari
https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap
gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah

detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-
mm-dd_HH:ii:ss].

c. Agar rapi, setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan
folder akan di delete(sehingga hanya menyisakan .zip).

d. Karena takut program tersebut lepas kendali, Kiwa ingin program tersebut men-
generate sebuah program "killer" yang siap di run(executable) untuk

menterminasi semua operasi program tersebut. Setelah di run, program yang
menterminasi ini lalu akan mendelete dirinya sendiri.
e. Kiwa menambahkan bahwa program utama bisa dirun dalam dua mode, yaitu
MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan
dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen
-b. Ketika dijalankan dalam MODE_A, program utama akan langsung
menghentikan semua operasinya ketika program killer dijalankan. Untuk
MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi
membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua
folder terisi gambar, terzip lalu di delete).

Kiwa lalu terbangun dan sedih karena menyadari programnya hanya sebuah mimpi.
Buatlah program dalam mimpi Kiwa jadi nyata!
Catatan:
- Tidak boleh memakai system().
- Program utama harus ter-detach dari terminal
Hint:
- Gunakan fitur picsum.photos untuk mendapatkan gambar dengan ukuran
tertentu
- Epoch Unix bisa didapatkan dari time()

### Penyelesaian no. 2
```
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
                        
                        t.tm_year = current_time->tm_year;  // Year - 1900
                        t.tm_mon = current_time->tm_mon;           // Month, where 0 = jan
                        t.tm_mday = current_time->tm_mday;          // Day of the month
                        t.tm_hour = current_time->tm_hour;
                        t.tm_min = current_time->tm_min;
                        t.tm_sec = current_time->tm_sec;
                        t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
                        t_of_day = mktime(&t);
                        //zip
                        t_of_day = (t_of_day%1000)+100;
                        char destination[1000]="https://picsum.photos/";
                        char buffer1[1000];
                        
                        sprintf(buffer1,"%ld",(long)t_of_day);
                        strcat(destination,buffer1);
                        
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
                char *binaryPath2 = "/usr/bin/zip";
                char *argv  [] = {binaryPath2,"-rm",b,b, NULL};
                execv(binaryPath2, argv);
            }
        }
        else{
            sleep(30);
        }            
    }
}

```
untuk killernya
```
void killer(char mode[]){
    FILE *fl;
    fl = fopen("killer.sh", "w");
    fprintf(fl,"rm $0\n");
    if(strcmp(mode, "-a")==0) fprintf(fl, "#!/bin/bash\nkill -9 -%d", getpid());
    else if(strcmp(mode, "-b")==0) fprintf(fl, "#!/bin/bash\nkill %d", getpid());
    int status;
    pid_t child_id;
    child_id = fork();
    if(child_id==0){ 
        char *argv[] = {"chmod", "u+x", "killer.sh", NULL};
        execv("/bin/chmod", argv);
    }
  
  fclose(fl);
}
```
## Soal 3
Jaya adalah seorang programmer handal mahasiswa informatika. Suatu hari dia
memperoleh tugas yang banyak dan berbeda tetapi harus dikerjakan secara bersamaan
(multiprocessing).

a. Program buatan jaya harus bisa membuat dua direktori di
“/home/[USER]/modul2/”. Direktori yang pertama diberi nama “indomie”, lalu
lima detik kemudian membuat direktori yang kedua bernama “sedaap”.

b. Kemudian program tersebut harus meng-ekstrak file jpg.zip di direktori
“/home/[USER]/modul2/”. Setelah tugas sebelumnya selesai, ternyata tidak
hanya itu tugasnya.

c. Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di
dalam direktori “home/[USER]/modul2/jpg/”) harus dipindahkan sesuai dengan
pengelompokan, semua file harus dipindahkan ke
“/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke
“/home/[USER]/modul2/indomie/”.

d. Untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/”
harus membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu
3 detik kemudian membuat file bernama “coba2.txt”.
(contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”).
Karena Jaya terlalu banyak tugas dia jadi stress, jadi bantulah Jaya agar bisa membuat
program tersebut.
Catatan :
- Tidak boleh memakai system().
- Tidak boleh memakai function C mkdir() ataupun rename().
- Gunakan exec dan fork
- Direktori “.” dan “..” tidak termasuk
### Penyelesaian no.3
```
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
          exit(EXIT_FAILURE); /* Jika gagal membuat proses baru, 
program akan berhenti*/
        }

        if (child_id == 0) {
          pid_t child_id;
          child_id = fork();
          int status9;
          if (child_id < 0) {
              exit(EXIT_FAILURE); /* Jika gagal membuat proses baru, 
program akan berhenti*/
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
            exit(EXIT_FAILURE); /* Jika gagal membuat proses baru, 
program akan berhenti*/
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
  } else {
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
                exit(EXIT_FAILURE); /* Jika gagal membuat proses baru, 
program akan berhenti*/
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
                    exit(EXIT_FAILURE); /* Jika gagal membuat proses baru, 
program akan berhenti*/
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
```
Program ini merupakan program yang membutuhkan forking yang cukup banyak karena merupakan program yang dikerjakan secara bersamaan (multiprocessing).

#### PEMBUATAN 2 DIREKTORI
Pada process awal, kita menggunakan `fork()` untuk membuat 2 direkotri yaitu indomie dan sedaap.
```
pid_t child_id;
child_id = fork();
```
Pada tahap ini kita membagi process menjadi parent dan child. Jika `child_id==0` maka itu merupakan childnya. Jika `child_id < 0` maka program akan berhenti karena gagal dalam membuat process
```
if (child_id < 0) {
    exit(EXIT_FAILURE);
}
```
Kemudian pada child process , kita membuat directory indomie dengan menggunakan `exec`, kemudian dalam parent process kita melakukan wait dengan cara `while ((wait(&status9)) > 0);` dan lakukan `sleep` selama 5 detik dan lakukan `exec` untuk membuat directory seedap. `sleep` dilakukan agar directory seedap terbuat 5 detik setelah directory indomie dibuat 
```
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
        
```
#### EKSTRAK JPG.ZIP
Process ekstrak ini berhubungan dengan pembuatan directory indomie dan seedap tadi.
Setelah 2 directory(indomie dan seedap) tersebut dibuat, program ini harus bisa meng-ekstrak jpg.zip. Hal ini dapat dilakukan dengan melakukan forking lagi yang process `fork()`nya kurang lebih sama dengan yang sebelumnya. yaitu membagi menjadi child process dan parent process. 

dimana child processnya merupakan pembuatan directory tadi (indomie dan seedap), dan parent processnya adalah sebagai berikut,
```
while ((wait(&status3)) > 0);
char *argv[3] = {"unzip","/home/vincent/modul2/jpg.zip", NULL};
execv("/usr/bin/unzip", argv);
```
fungsi `while ((wait(&status3)) > 0);` untuk menunggu process child jalan,lalu kita menggunakan `execv`untuk melakukan unzip/ekstrak jpg.zip

#### MEMINDAHKAN HASIL EKSTRAK
Process ini juga berhubungan dengan 2 process sebelumnya, dimana setelah membuat directory dan mengekstrak jpg.zip, kita memindahkan hasil ekstrak tersebut kedalam directory indomie dan sedap. Jika yang dipindahkan berupa directory, maka kita pindahkan ke directory indomie dan jika yang dipindahkan berupa file, kita pindahkan ke directory seedap.

Pada tahap ini, kita menggunakan forking yang sama juga seperti process sebelumnya yaotu membuat child process dan parent process dimana child processnya adalah process ekstrak sebelumnya dan parent processnya adalah
```
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
            exit(EXIT_FAILURE);
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
```
disini kita menggunakan dirent dalam library `dirent.h` untuk mengambil file yang terdapat di diretory tertentu. dalam hal ini kita memasukan directory `/home/vincent/modul2/jpg/` untuk mengambil file/ directory dalam directory jpg
```
struct dirent *dp;
DIR *fd = opendir("/home/vincent/modul2/jpg/");
```
kemudian kita melakukan looping dalam directory jpg tersebut dengan `while((dp = readdir(fd))!=NULL)` dan didalamnya untuk setiap file yang ada kita check dan pindahkan ke directory seedap dan untuk setiap directory yang ada kita pindahkan ke directory indomie. untuk directory `.` dan `..` tidak termasuk maka kita lakukan continue
```
if (!strcmp (dp->d_name, ".")) continue;
if (!strcmp (dp->d_name, ".."))continue;
```
kemudian kita lakukan strcpy pada nama file yang ingin dipindahkan yaitu `"/home/vincent/modul2/jpg/"` dan lakukan strcat pada file yang ditemukan sehingga terbentuk path baru yang merupakan path file dalam directory jpg. 
```
strcpy(filename2,filename);
strcat(filename2,dp->d_name);
```
Kemudian kita lakukan `fork()` lagi karena kita mau menggunakan exec. setelah melakukan fork, akan terbagi menjadi child dan parent. pada child process kita bagi untuk directory dan untuk file. Jika yang ingin dipindahkan berupa directory, maka kita pindahkan ke indomie dengan menggunakan exec
```
if(child_id5 == 0 && dp->d_type == 4){
      char *arg4v[] = {"mv",filename2,"/home/vincent/modul2/indomie/",NULL};
      execv("/bin/mv",arg4v);
}
```
dan jika yang ingin dipindahkan berupa file, kita pindahkan ke seedap dengan
```
if(child_id5 == 0 && dp->d_type == 8){
      char *arg5v[] = {"mv",filename2,"/home/vincent/modul2/sedaap/",NULL};
      execv("/bin/mv",arg5v);
}
```
#### MEMBUAT 2 FILE KOSONG
Terakhir, setelah membuat directory, ekstrak dan memindahkan hasil ekstrak, kita haru membuat 2 file kosong `coba1.txt` dan `coba2.txt` dalam directory yang terdapat di directory indomie. dimana pembuatan file tersebut harus memiliki selang 3 detik.

Untuk melakukan ini, kita harus melakukan process yang sama seperti sebelumnya yaitu forking dengan child processnya adalah memindahkan hasil ekstrak dan parent processnya adalah 
```
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
                exit(EXIT_FAILURE);
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
                    exit(EXIT_FAILURE);
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
```
pada awalnya kita melakukan wait seperti biasa, lalu kita juga menggunakan dirent untuk bisa mendapatkan file/directory dalam suatu directory, dan sama seperti sebelumnya, kita lakukan looping dalam directory tertentu untuk mendapatkan file/directory dalam directory tersebut. Dalam kasus ini, kita melakukan looping dalam directory indomie
```
DIR *fd = opendir("/home/vincent/modul2/indomie/");
```
dan sama seperti sebelumnya kita tidak perlu directory `.` dan `..` maka kita continue.

Lalu disini, kita melakukan forking lagi karena disini kita berusaha membuat file `coba1.txt` dan 3 detik kemudian membuat file `coba2.txt` dengan menggunakan exec, maka pada child kita melakukan strcpy file dari file `"/home/vincent/modul2/indomie/"` lalu menggunakan strcat menggabungkan directory yang dituju, dan strcat lagi untuk menggabungkan file `/coba1.txt`. setelah dilakukan penggabungan, nnti akan menjadi
```
"/home/vincent/modul2/indomie/[namaDirectory]/coba1.txt"
```
Setelah ini, kita menggunakan execv untuk membuat file
```
char *argv6[] = {"touch",filename4, NULL};
execv("/usr/bin/touch", argv6); 
```
Kemudian pada parent process, kita gunakan wait dan `sleep(3)` untuk memberi jeda waktu sebanyak 3 detik setelah coba1.txt dibuat.

Dalam parent process ini kita melakukan forking lagi karena kita akan menggunakan exec lagi. process yang dilakukan kurang lebih sama dengan yang sebelumnya. hanya saja, kita membuat file `coba2.txt`. kita juga menggunakan execv untuk membuat file tersebut
```
char *argv7[] = {"touch",filename4, NULL};
execv("/usr/bin/touch", argv7);
```
![hasil directory di Indomie](https://github.com/afiffadhlurrahman/SoalShiftSISOP20_modul2_C06/tree/master/images/jumlahDir.png)
![hasil file di Sedaap](https://github.com/afiffadhlurrahman/SoalShiftSISOP20_modul2_C06/tree/master/images/jumlahFile.png)
![beda waktu indomie sedaap](https://github.com/afiffadhlurrahman/SoalShiftSISOP20_modul2_C06/tree/master/images/bedaWaktuIndomieSedaap.png)
![beda waktu cobae](https://github.com/afiffadhlurrahman/SoalShiftSISOP20_modul2_C06/tree/master/images/bedaWaktuCoba.png)
