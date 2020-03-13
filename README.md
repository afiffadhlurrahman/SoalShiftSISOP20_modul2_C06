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
        if((tm.tm_hour == hour || hourBintang) && (tm.tm_min == min || minBintang)
          && (tm.tm_sec == sec || secBintang)){
            if(fork()==0){
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
            if(fork()==0){
                execl("/bin/bash","bash",argv[4],NULL);
            }
        }
```
Kita melakukan check apakah jam saat/menit/detik saat ini sama dengan yang kita input pada argumen. dan jika `hourBintang` bernilai true, maka artinya kita telah menginputkan `*` pada konfigurasi jam dan yang berarti akan berjalan tiap jam. ini juga berlaku untuk `minBintang` dan `secBintang`.

Jika semuanya sudah terpenuhi, maka kita bisa mengeksekusi program bash dengan menggunakan `exec`. Namun sebelum melakukan `exec`, kita harus melakukan `fork()` karena seperti yang kita ketahui, `exec` akan membuat program berhenti setelah melakukan eksekusi. Maka dari itu kita harus menjalankannya di child. 
```
if(fork()==0)
```
Setelah ini, kita bisa melakukan fungsi `exec` sebagai berikut
```
execl("/bin/bash","bash",argv[4],NULL);
```
yang berarti kita menjalankan bash script yang pathnya merupakan `argv[4]`.

## Soal 2

## Soal 3
