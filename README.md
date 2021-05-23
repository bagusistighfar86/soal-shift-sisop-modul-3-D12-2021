# soal-shift-sisop-modul-3-D12-2021
## Anggota Kelompok
| Nama | NRP |
|------|-----|
|Muhammad Bagus Istighfar|05111940000049|
|Rizqi Rifaldi|05111940000068|

## Soal 1

Soal 1 dari modul 3 tidak dapat kami selesaikan dikarenakan beberapa hal :
1. Salah satu anggota kelompok kami tidak ada kabar jadi hanya 2 orang yanh mengerjakan soal modul 3
2. Kesulitan dari soal 1 yang menurut kami tinggi dan cukup kompleks sehingga kami tidak bisa mengerjakan soal 1
## Soal 2

### Soal 2a
Soal ini diperintahkan untuk membuat program perkailan matriks 4x3 dengan matriks 4x6 dengan memanfaatkan thread.
Berikut adalah library yang di gunakan beserta global variable untuk ukuran matriks.
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define baris 4 
#define bk 3
#define kolom 6
```
Berikut adalah fungsi main :

```c
int main() {
    key_t key = 1234;
    int m1[baris][bk], m2[bk][kolom], matriks_hasil[baris * kolom];

    int shmid = shmget(key, sizeof(int) * baris * kolom, IPC_CREAT | 0666);
    int* matriks_share = (int*)shmat(shmid, NULL, 0);

    int i, j;
    printf("matriks %dx%d :\n", baris, bk);
    for (i = 0; i < baris; i++) {
        for (j = 0; j < bk; j++) scanf("%d", &m1[i][j]);
    }

    printf("\nmatriks %dx%d :\n", bk, kolom);
    for (i = 0; i < bk; i++) {
        for (j = 0; j < kolom; j++) scanf("%d", &m2[i][j]);
    }
```
`key` digunakan sebagai index dalam share memory yang akan digunakan untuk soal 2b.

`int m1[baris][bk], m2[bk][kolom], matriks_hasil[baris * kolom];` merupakan deklarasi matriks 1, matriks 2, dan matriks hasil yang digunakan untuk menyimpan jawaban dari soal2a ini.

`int shmid = shmget(key, sizeof(int) * baris * kolom, IPC_CREAT | 0666);` digunakan untuk id yang akan digunakan pada shared memory

`int* matriks_share = (int*)shmat(shmid, NULL, 0);` pointer matriks_share digunakan untuk mempassing hasil dari matriks soal 2a ini ke soal 2b.

Kemudian, 2 for diatas untuk input data ke dalam matriks 1 dan matriks 2 seperti biasa.

```c
int err, k;
    pthread_t* tid = (pthread_t*)malloc((24) * sizeof(pthread_t));

    int count = 0;
    int* arg = NULL;

    for (i = 0; i < baris; i++) {
        for (j = 0; j < kolom; j++) {
            arg = (int*)malloc((6) * sizeof(int));

            for (k = 0; k < bk; k++) {
                arg[k] = m1[i][k];
                //printf("1.arg[%d] = m1[%d][%d] = %d\n",k+1,i,k,m1[i][k]);
            }
            for (k = 0; k < bk; k++) {
                arg[k + bk] = m2[k][j];
                //printf("2.arg[%d] = m1[%d][%d] = %d\n",k+1+bk,k,j,m2[k][j]);
            }

            err = pthread_create(&(tid[count++]), NULL, &kali, (void*)arg);
            if (err != 0)
                printf("\n Can't create thread : [%s]", strerror(err));
        }
    }
```
`pthread_t* tid = (pthread_t*)malloc((24) * sizeof(pthread_t));` digunakan untuk mengaolakasikan 24 memori untuk mendapatkan 24 id sebagai thread yang akan digunakan untuk melakukan perhitungan perkalian sebanyak 24.

`int count = 0;` digunakan untuk menghitung id dari 0 - 23.

`int* arg = NULL;`digunakan sebagai pointer untuk menyimpan data matriks 1 dan 2 secara linear. Dibawah ini adalah contoh ilustrasi perhitungan :


[![1621698221145.jpg](https://i.postimg.cc/zBg4LpbJ/1621698221145.jpg)](https://postimg.cc/wy9wZXQP)

Kemudian di dalam kedua for diatas dilakukan perhitungan sebanyak 24 kali sesuai aturan perkalian matriks seperti biasa. Perhitungan dilakukan pada thread yang dibuat untuk menjalankan fungsi void* kali(). Berikut adalah fungsi void* kali : 
```c
void* kali(void* argvar) {
    int* arg = (int*)argvar;
    int i, n = 0;

    for (i = 0; i < bk; i++) 
        n += arg[i] * arg[i + bk];

    int* p = (int*)malloc(sizeof(int));
    *p = n;
    pthread_exit(p);
}
```
void* argvar adalah pointer yang dipassing dari main ke fungsi kali. hasil perhitungan di simpan dalam isi dari pointer p yang akan direturn.

Setelah itu akan dijalankan thread pada for dibawah ini sesuai id yang disimpan pada `count` dari 0-23, lalu dicetak hasil perkalian matriksnya.
```c
    printf("\nHasil Perkalian :\n");
    for (i = 0; i < baris * kolom; i++) {
        void* k;
        pthread_join(tid[i], &k);
        int* p = (int*)k;

        printf("%d ", *p);
        if ((i + 1) % kolom == 0) {
            printf("\n");
        }
        matriks_hasil[i] = *p;
    }

    for (i = 0; i < baris * kolom; i++) {
        matriks_share[i] = matriks_hasil[i];
    }

    shmdt(matriks_share);
    exit(0);
    return 0;
}
```
Kemudian setiap hasil yang disimpan dalam matriks_hasil disimpan dalam matriks_share yang akan dipassing dan digunakan pada soal 2b.

**OUTPUT**

[![1621749612684.jpg](https://i.postimg.cc/0y85Qkmt/1621749612684.jpg)](https://postimg.cc/9rnHgjLT)

### Soal 2b
Soal ini memerintahkan kita untuk input matriks baru dengan ukuran 4x6. Matriks baru ini memiliki hubungan dengan matriks hasil dari soal 2a untuk dilakukan perhitungan dengan memerhatikan 3 kondisi sebagai berikut :
If a >= b  -> a!/(a-b)!
If b > a -> a!
If 0 -> 0
a adalah matriks lama dan b adalah matriks baru. Kemudian hasil dari faktorial tersebut menjadi matriks hasil baru.

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define baris 4 
#define kolom 6

long long matriks[baris][kolom];
int matriks_shared[baris][kolom], matriks_baru[baris][kolom];

pthread_t tid[baris * kolom];

typedef struct thread_args {
    long long  A, B, baris_baru, kolom_baru;
} args;
```
Di atas adalah library dan global variable yang digunakan. Kami membuat struct thread_args untuk menyimpan isi dari matriks lama pada A, matriks baru pada B, dan index matriks hasil faktorial pada baris_baru dan kolom_baru. Kemudian dideklarasikan pula tid untuk meyimpan 24 id thread.
Berikut adalah main soal 2b :
```c
int main(int argc, char const *argv[]) {
    int i, j, k, err;
    args arg[baris * kolom];

    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    int *hasil = (int *)shmat(shmid, NULL, 0);

    printf("matriks A:\n");
    for (i = 0; i < baris; i++) {
        for (j = 0; j < kolom; j++) {
            matriks_shared[i][j] = hasil[k++];
            printf("%d ", matriks_shared[i][j]);
        }
        printf("\n");
    }

    printf("\nmatriks B:\n");
    for (i = 0; i < baris; i++) {
        for (j = 0; j < kolom; j++) {
            // printf("%d:%d ", i, j);
            scanf("%d", &matriks_baru[i][j]);
            // printf("%d ", matriks_baru[i][j]);
        }
        // printf("\n");
    }
```
`key` yang digunakan sama pada soal 2a yakni 1234. 2 for di atas untuk menginput data dari matriks_shared dan matriks_baru.


```c
    for (i = 0; i < baris; i++) {
        for (j = 0; j < kolom; j++) {
            arg[i * kolom + j].A = matriks_shared[i][j];
            arg[i * kolom + j].B = matriks_baru[i][j];
            arg[i * kolom + j].baris_baru = i;
            arg[i * kolom + j].kolom_baru = j;
            // printf("%d => %d %d %d %d\n", i * kolom + j, arg[i * kolom + j].A,
            //        arg[i * kolom + j].B, arg[i * kolom + j].i, arg[i * kolom + j].j);

            err = pthread_create(&(tid[i * kolom + j]), NULL, calculator,
                                 &arg[i * kolom + j]);
            if (err) printf("Error");
        }
    }

    for (i = 0; i < baris * kolom; i++) 
        pthread_join(tid[i], NULL);

    printf("\nHasil:\n");
    for (i = 0; i < baris; i++) {
        for (j = 0; j < kolom; j++) {
            printf("%lld ", matriks[i][j]);
        }
        printf("\n");
    }

    shmdt((void *)hasil);
    return 0;
}
```
Pada for di atas untuk menyimpan matriks shared, matriks baru, sekaligus index untuk matriks hasil pada struct yang telah dibuat. Pada for tersebut dilakukan pembuatan thread untuk menjalankan fungsi calculator(). Thread akan dijalankan pada for dibawahnya sebanyak 24 kali. Kemudian akan dicetak hasil dari perhitungan pada fungsi calucalator().

Berikut adalah fungsi calculator() :
```c
void *calculator(void *arg) {
    pthread_t id = pthread_self();
    int i;

    for (i = 0; i < baris * kolom; i++) {
        if (pthread_equal(id, tid[i])) {
            args *arg_matriks = (args *)arg;

            matriks[arg_matriks->baris_baru][arg_matriks->kolom_baru] =
                factorial(arg_matriks->A, arg_matriks->B);

            // printf("A:B,a:b -> res => %d:%d, %d:%d -> %d\n", arg_matriks->A,
            //        arg_matriks->B, arg_matriks->baris_baru, arg_matriks->kolom_baru,
            //        matriks[arg_matriks->baris_baru][arg_matriks->kolom_baru]);
        }
    }
}
```
Fungsi ini akan menjalankan fungsi factorial(). Berikut adalah fungsi factorial ():
```c
long long factorial(int a, int b) {
    long long result = 1, i;
    // printf("%d:%d\n", a, b);
    if (a == 0 || b == 0)
        result = 0;
    else if (a >= b)
        for (i = a - b + 1; i <= a; i++) {
            result *= i;
            // printf("result = %d\n",result);
        }
    else if (a < b)
        for (i = 1; i <= a; i++) result *= i;

    return result;
}
```

**Catatan** : hasil disimpan pada tipe data long long!

**OUTPUT**

[![1621749667599.jpg](https://i.postimg.cc/PrHT3X6t/1621749667599.jpg)](https://postimg.cc/LhxchpK7)

### Soal 2c
Pada soal ini diperintahkan untuk mengecek 5 proses teratas yang memakan pada resource kompputer kita dengan memanfaatkan command `“ps aux | sort -nrk 3,3 | head -5”` **(Catatan!: Harus menggunakan IPC Pipes)**

Berikut adalah library dan deklarasi fungsi serta global variable yang digunakan :
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int pid;
int fd1[2];
int fd2[2];

void ps_comm();
void sort_comm();
void head_comm();
void closeAll();

char *ps_arg[] = {"ps", "aux", NULL};
char *sort_arg[] = {"sort", "-nrk", "3,3", NULL};
char *head_arg[] = {"head", "-5", NULL};
```
`fd1[],fd2[]`digunakan untuk meyimpan file descriptor 1 untuk pipe 1 dan file descriptor 2 untuk pipe 2.

Kemudian ada 4 fungsi yakni ps_comm() untuk melakukan comman ps, sort_comm() untuk melakukan command sort, head_comm() untuk melakukan command head, dan fungsi closeAll() untuk menutup semua file descriptor.

`char *ps_arg[] = {"ps", "aux", NULL};` digunakan untuk menyimpan argumen dari command ps aux.

`char *sort_arg[] = {"sort", "-nrk", "3,3", NULL};` digunakan untuk menyimpan argumen dari command sort -nrk 3,3.

`char *head_arg[] = {"head", "-5", NULL};` digunakan untuk menyimpan argumen dari command head -5.


Berikut adalah main dari soal 2c :
```c
int main(){

    // create fd1
    if (pipe(fd1) == -1){
        perror("failed create pipe1");
        exit(1);
    }

    if ((pid = fork()) == -1){
        exit(1);
    }
    else if (pid == 0){
        ps_comm();
    }

    if (pipe(fd2) == -1){
        perror("failed create pipe2");
        exit(1);
    }

    if ((pid = fork()) == -1){
        exit(1);
    }
    else if (pid == 0){
        sort_comm();
    }

    if ((pid = fork()) == -1){
        exit(1);
    }
    else if (pid == 0){
        head_comm();
    }

    closeAll();

    int i, status;
    for (i = 0; i < 3; i++)
        wait(&status);
    return 0;
}

void ps_comm(){

    // output ke fd1
    dup2(fd1[1], 1);

    closeAll();

    // exec ps
    execv("/bin/ps", ps_arg);
    _exit(1);
}
}
```
Membuat pipe1 dengan fd1[]. Pada child proses fork ini menjalankan ps_comm();
```c
 // create fd1
    if (pipe(fd1) == -1){
        perror("failed create pipe1");
        exit(1);
    }

    if ((pid = fork()) == -1){
        exit(1);
    }
    else if (pid == 0){
        ps_comm();
    }
```

Membuat pipe2 dengan fd2[]. Pada child proses fork ini menjalankan sort_comm();
```c
 if (pipe(fd2) == -1){
        perror("failed create pipe2");
        exit(1);
    }

    if ((pid = fork()) == -1){
        exit(1);
    }
    else if (pid == 0){
        sort_comm();
    }
```

Jangan lupa ditutup dulu fd1 dan fd2 setiap proses yang menggunakan file descriptor dengan fungsi closeAll().

Berikut adalah fungsi ps_comm():
```c
void ps_comm(){

    // output ke fd1
    dup2(fd1[1], 1);

    closeAll();

    // exec ps
    execv("/bin/ps", ps_arg);
    _exit(1);
}
```

Berikut adalah fungsi sort_comm():
```c
void sort_comm(){
    // input dari fd1
    dup2(fd1[0], 0);
    // output ke fd2
    dup2(fd2[1], 1);

    closeAll();

    execv("/bin/sort", sort_arg);   
    _exit(1);
}
```

Berikut adalah fungsi head_comm():
```c
void head_comm(){
    // input dari fd2
    dup2(fd2[0], 0);

    closeAll();

    execv("/bin/head", head_arg);
    _exit(1);
}
}
```

Berikut adalah fungsi closeAll():
```c
void closeAll(){
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
}
```

Pada fungsi ps_comm(), sort_comm(), dan head_comm() terdapat dup2(int old_fd, int new_fd) yang digunakan untuk menduplikasikan file descriptor pipe tersebut (old_fd) ke dalam file descriptor dari stdout atau stdin(new_fd). stdout bernilai 1 dan stdin bernilai 0. 

Berikut adalah ilustrasi untuk file descriptor pada pipe :

[![pipe-fork.png](https://i.postimg.cc/TYmxpV52/pipe-fork.png)](https://postimg.cc/CzFX2f49)

**OUTPUT**

[![1621749704668.jpg](https://i.postimg.cc/DZbnD8kz/1621749704668.jpg)](https://postimg.cc/JDmfH7hw)
 

## Soal 3
Seorang mahasiswa bernama Alex sedang mengalami masa gabut. Di saat masa gabutnya, ia memikirkan untuk merapikan sejumlah file yang ada di laptopnya. Karena jumlah filenya terlalu banyak, Alex meminta saran ke Ayub. Ayub menyarankan untuk membuat sebuah program C agar file-file dapat dikategorikan. Program ini akan memindahkan file sesuai ekstensinya ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan.

Untuk mengerjakan Soal 3, saya membuat beberapa fungsi yang dapat memudahkan pengerjaan soal, yaitu :

1.Fungsi ubahlc

Fungsi ini digunakan untuk merubah string menjadi lowercase, dikarenakan pada soal, ekstensi / tipe file bersifat case insensitive, dimana .jpg dan .JPG dianggap sama. 

```C
char *ubahlc(char *string){
	unsigned char *temp;
  	temp = (unsigned char *)string;
  	while(*temp){
    		*temp = tolower(*temp); 
    		temp++;
	}
	return string;
}
```
2.Fungsi getnama

Fungsi ini digunakan untuk mengambil hanya nama file dari path file yang diinputkan. Contoh jika path yang diinputkan adalah ```/home/rizqi/soal3/coco.c``` maka yang diambil hanya ```coco.c``` saja. 

Source Code :
```C
char *getnama(char *direc){ 
	char *nama;
	nama = strrchr(direc, '/');
	if(nama == direc){
		return "";
	}
	return nama + 1;
}
```
Dengan menggunakan ```strrchr(direc,'/')```, akan melihat tanda "/" terakhir pada path dan akan mengembalikan string mulai dari tanda "/" sampai dengan akhir string, selanjutnya fungsi akan mengembalikan string nama mulai dari indeks 1 untuk men-skip tanda "/".

3.Fungsi gettipe

Fungsi ini digunakan untuk mengambil hanya tipe atau ekstensi dari file. Contoh jika file bernama coco.jpg maka fungsi akan mengambil jpg saja. Fungsi ini juga berlaku untuk file berupa hidden dan tidak diketahui ekstensi atau tipenya (Unknown).

Source Code :

```C
char *gettipe(char *direc){
	int i = 0;
	char *tipehid = {"Hidden"};
	char *tipeunk = {"Unknown"};
	char *temp = strrchr(direc, '/'); 
	if(temp[1] == '.'){ // file hidden
		return tipehid;
	}
	while(i < strlen(temp) && temp[i] != '.'){ 
		i++;
	}
	if(i == strlen(temp)){ 
		return tipeunk;
	} 
	int j = i;
	char tipe[400];
	while(i < strlen(temp)){  
		tipe[i-j] = temp[i];
		i++;
	}
	return ubahlc(tipe + 1);
}
```
Pada fungsi ini, pertama akan mengambil nama file dari path dengan menggunakan ```strrchr```, kemudian mengecek nama file, jika indeks 1 dari nama file (indeks 0 adalah "/") merupakan "." , berarti file tersebut akan dikategorikan sebagai Hidden. Kemudian fungsi mengecek keberadaan tanda "." pada nama file menggunakan loop while, dan jika tidak ada sampai akhir dari string, maka file akan dikategorikan sebagai Unknown. Jika terdapat tanda "." pada string, selanjutnya akan masuk ke loop kedua untuk menyalin tipe file dan fungsi akan mengembalikan string tipe yang sudah diubah menjadi lowercase menggunakan fungsi ubahlc mulai dari indeks 1 (indeks 0 adalah tanda ".") .
### Soal 3a
Program menerima opsi -f seperti contoh di atas, jadi pengguna bisa menambahkan argumen file yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna. Output yang dikeluarkan adalah seperti ini :

```
File 1 : Berhasil Dikategorikan (jika berhasil)
File 2 : Sad, gagal :( (jika gagal)
File 3 : Berhasil Dikategorikan
```
Untuk mengerjakan no 3a, saya menggunakan fungsi kategorifile yang berfungsi untuk mengkategorikan file ke dalam folder yang bernama ekstensi atau tipenya. 

Source Code :
```C
void* kategorifile(void *arg){
	char *pathasal;
	char pathfile[200];
	char *pathtipe;
	char tipe[500];
	pathasal = (char *)arg; 
	memcpy(pathfile, pathasal, 400);
	pathtipe = gettipe(pathasal);
	strcpy(tipe, pathtipe);
	DIR *dir = opendir(pathtipe);
	if(dir){
		closedir(dir); 
	}
	else if(ENOENT == errno){ 
		mkdir(pathtipe, 0755);
	} 
	char *nama;
	char *pathprogram;
	char tujuan[200] = {};
	nama = getnama(pathfile);
	pathprogram = getenv("PWD");
	strcat(tujuan,pathprogram);
	strcat(tujuan,"/");
	strcat(tujuan,tipe);
	strcat(tujuan,"/");
	strcat(tujuan,nama); 
	rename(pathfile, tujuan); 
}
```
Pertama - tama, fungsi akan mengambil argumen parameter menjadi path asal, dan mengambil tipe dari path asal, kemudian membuka direktori untuk mengecek apakah direktori yang bernama tipe file sudah ada atau belum, dan jika sudah ada direktori akan ditutup dan jika belum ada maka direktori akan dibuat. Kemudian fungsi kategorifile ini juga menggunakan fungsi getnama untuk mengambil nama file, dan menggunakan ```getenv("PWD")``` untuk mengambil path dimana program tersebut dijalankan, kemudian fungsi akan merangkai path baru dan me rename path lama dari file ke path baru dan file otomatis terpindah ke path baru yaitu di dalam folder tipe file.

Pada main, saya menggunakan percabangan seperti sebagai berikut :

```C
if(argc > 2 && strcmp(argv[1], "-f") == 0){
		pthread_t tid[argc-2]; 
		int counter = 0;
		for(int i = 2; i < argc; i++){
			if(access(argv[i], F_OK) == 0){ 
				pthread_create(&tid[counter], NULL, kategorifile, (void *)argv[i]);
				counter++;
				printf("File %d : Berhasil Dikategorikan\n", i-1);
			}
			else printf("File %d : Sad, gagal :(\n", i-1);
		}
		for(int i = 0; i < counter; i++){ 
			pthread_join(tid[i], NULL); 
		}
		return 0;
	}

```
Jika argumen nya berjumlah lebih dari 2 dan argumen ke dua (indeks 1) adalah ```-f```, maka merupakan soal 3a yang hanya mengkategorikan beberapa file sesuai dengan argumen berupa path dari file. Pertama, inisialisasi thread dengan jumlah sesuai dengan jumlah file (argc - 2 untuk men skip ./soal3 -f) kemudian menggunakan loop untuk membuat thread sesuai dengan jumlah file (1 file 1 thread) yang berfungsi untuk mengkategorikan file, jika file ada maka akan muncul pesan berhasil dan jika file tidak ada maka akan muncul pesan gagal. Setelah selesai, maka dilakukan join thread yang berfungsi sama seperti ```wait()``` pada fork yaitu menunggu thread terminated.

### Soal 3b
Program juga dapat menerima opsi -d untuk melakukan pengkategorian pada suatu directory. Namun pada opsi -d ini, user hanya bisa memasukkan input 1 directory saja, tidak seperti file yang bebas menginput file sebanyak mungkin.

Contohnya adalah seperti ini :
``` 
$./soal3 -d /path/to/directory/
```
Perintah di atas akan mengkategorikan file di /path/to/directory, lalu hasilnya akan disimpan di working directory dimana program C tersebut berjalan (hasil kategori filenya bukan di /path/to/directory). Output yang dikeluarkan adalah seperti ini :
```
Jika berhasil, print “Direktori sukses disimpan!”
Jika gagal, print “Yah, gagal disimpan :(“
```
Untuk menjawab soal 3b, saya membuat fungsi kategorifolder yang berfungsi untuk mengkategorikan file dalam suatu folder tertentu. 
```C
void kategorifolder(char *pathfolder, int sizethread){ 
	DIR *fd = opendir(pathfolder);
	struct dirent *dp;
	pthread_t tid[sizethread]; 
	int counter = 0;
	char namafile[200][200] ={};
	while((dp = readdir(fd)) != NULL){
		if(dp->d_type == DT_REG){ 
			strcat (namafile[counter],pathfolder);
			strcat(namafile[counter],"/");
			strcat(namafile[counter],dp->d_name); 
			pthread_create(&tid[counter], NULL, kategorifile, (void *)namafile[counter]); 
			counter++;
		}
		else if((dp->d_type == DT_DIR) && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) { 
            char pathfolderisi[200] = {};
            strcat (pathfolderisi,pathfolder);
            strcat (pathfolderisi,"/");
            strcat (pathfolderisi,dp->d_name);  
            DIR *fd2 = opendir(pathfolderisi); 
            struct dirent *dp2;
			int sizethreadfold = 0;
			while((dp2 = readdir(fd2)) != NULL){
				if(dp2->d_type == DT_REG){
					sizethreadfold++;
				}
			}
			kategorifolder(pathfolderisi, sizethreadfold);
			closedir(fd2);
        	}
	}
	for(int i = 0; i < sizethread; i++){ 
		pthread_join(tid[i], NULL);
	} 
    	closedir(fd);
}
```
Pertama, fungsi akan membuka folder yang sesuai dengan path pada argumen, kemudian inisialisasi thread sejumlah variabel sizethread yang merupakan jumlah file (1 thread 1 file), kemudian akan membuka direktori, membaca isi direktori satu persatu, dan jika isi direktori berupa file reguler, maka fungsi akan merangkai path dari file tersebut menggunakan ```strcat()``` dan kemudian akan membuat thread untuk mengkategorikan file tersebut, dan jika isi direktori tersebut adalah suatu direktori,  maka fungsi akan merangkai path dari direktori tersebut dan membuka serta membaca direktori tersebut untuk menghitung jumlah thread yang dibutuhkan. Kemudian melakukan rekursi dengan memanggil fungsi itu sendiri untuk mengkategorikan file didalam direktori yang berada di dalam direktori. Kemudian setelah selesai, dilakukan join thread untuk menunggu semua thread terminated.

Pada main, saya menggunakan percabangan berikut untuk soal 3b :
```C
else if(argc == 3 && strcmp(argv[1], "-d") == 0){ 
	DIR *fd = opendir(argv[2]);
	if(fd){
		struct dirent *dp;
		int sizethread = 0;
		while((dp = readdir(fd)) != NULL){
			if(dp->d_type == DT_REG){ 
				sizethread++;
			}
		}
		kategorifolder(argv[2], sizethread); 
		closedir(fd);
		printf("Direktori sukses disimpan!\n");
	}
	else if(ENOENT == errno){ 
		printf("Yah, gagal disimpan :(\n");
	} 
}
```
Jika argumen yang dimasukkan adalah 3 dan argumen kedua (indeks 1) adalah ```-d```, maka itu merupakan soal 3b yang mengkategorikan isi dari direktori tertentu sesuai dengan path yang ditulis pada argumen. Pertama, pada main akan membaca isi direktori untuk menghitung jumlah thread yang akan dibuat. Setelah itu, memanggil fungsi ```kategorifolder()``` untuk mengkategorikan direktori tersebut dan jika sukses akan mengeluarkan pesan sukses, dan jika direktori yang diinput sebagai argumen tidak ada, maka akan mengeluarkan pesan error.

### Soal 3c
Selain menerima opsi-opsi di atas, program ini menerima opsi \*, contohnya ada
di bawah ini:
```
$ ./soal3 \*
```
Opsi ini akan mengkategorikan seluruh file yang ada di working directory ketika menjalankan program C tersebut.

Untuk mengerjakan soal 3c ini juga menggunakan fungsi ```kategorifolder()```, bedanya dengan soal 3b adalah direktori yang digunakan untuk argumen adalah direktori dimana program ini dijalankan.

Pada main, saya menggunakan berikut untuk soal 3c :
```C
else if(argc == 2 && strcmp(argv[1], "*") == 0){ 
		char *pathprogram;
		int sizethread = 0;
		pathprogram = getenv("PWD"); 
		DIR *dir = opendir(pathprogram);
		struct dirent *dp;
		while((dp = readdir(dir)) != NULL){
			if(dp->d_type == DT_REG){
				sizethread++;
			}
		}
		kategorifolder(pathprogram, sizethread);
		closedir(dir);
	}
```
Jika argumen berjumlah 2 dan argumen kedua (indeks 1) adalah ```*``` maka merupakan soal 3c yang mengkategorikan semua file di direktori dimana program dijalankan. Pertama - tama akan mengambil path dimana program itu dijalankan dengan menggunakan ```getenv("PWD")```, kemudian akan membuka direktori dan membaca isi direktori tersebut untuk menghitung jumlah thread yang dibutuhkan, setelah itu akan memanggil fungsi ```kategorifolder()``` untuk mengkategorikan file dalam suatu direktori tertentu.

### Soal 3d 
Semua file harus berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, file disimpan dalam folder “Unknown”. Jika file hidden, masuk folder “Hidden”.

Seperti dijelaskan diatas, fungsi ```gettipe()``` juga berlaku untuk file yang berupa Hidden dan tidak memiliki tipe atau ekstensi (Unknown), jadi file - file yang Hidden dan Unknown juga akan dikategorikan.

### Soal 3e
Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.

Seperti dijelaskan diatas juga, pada soal 3b dan soal 3c membaca isi direktori untuk menghitung berapa banyak thread yang dibutuhkan untuk melakukan kategori file, maka dari itu satu thread berlaku untuk satu file.

### Output Program
__Soal 3a__

Pada awalnya, di direktori no3 dengan path ```/home/rizqi/no3``` adalah sebagai berikut :

[![3a-1.jpg](https://i.postimg.cc/wM52SDCR/3a-1.jpg)](https://postimg.cc/n9zqm9cn)

Kemudian saya coba menggunakan perintah pada soal 3a di terminal :

[![3a-2.jpg](https://i.postimg.cc/wjj2Gy03/3a-2.jpg)](https://postimg.cc/N5ZRyMhw)

Isi dari direktori no3 menjadi sebagai berikut :

[![3a-3.jpg](https://i.postimg.cc/HLftnSH7/3a-3.jpg)](https://postimg.cc/jW4fk4Ds)

Salah satu isi direktori tipe :

[![3a-4.jpg](https://i.postimg.cc/dQR9QmfL/3a-4.jpg)](https://postimg.cc/z3Bh0W8r)


__Soal 3b__

Pada awalnya, di direktori no3 dengan path ```/home/rizqi/no3``` adalah sebagai berikut :

[![3b-1.jpg](https://i.postimg.cc/YC70rYJq/3b-1.jpg)](https://postimg.cc/BXh4NLSd)

Kemudian isi direktori soal3 yang akan dikategorikan adalah sebagai berikut :

[![3b-2.jpg](https://i.postimg.cc/VNGkWT5d/3b-2.jpg)](https://postimg.cc/67ZK9MrX)

Kemudian saya coba menggunakan perintah pada soal 3b :

[![3b-3.jpg](https://i.postimg.cc/8cVQRBd7/3b-3.jpg)](https://postimg.cc/tZDMy6Mb)

Isi dari direktori soal3 menjadi sebagai berikut :

[![3b-4.jpg](https://i.postimg.cc/m2BWhj2q/3b-4.jpg)](https://postimg.cc/4nLSSzcb)

Isi dari direktori no3 menjadi sebagai berikut :

[![3b-5.jpg](https://i.postimg.cc/1X6FTPNW/3b-5.jpg)](https://postimg.cc/vxYDgJh9)

Isi sala satu direktori tipe file adalah sebagai berikut :

[![3b-6.jpg](https://i.postimg.cc/D0FfvKMp/3b-6.jpg)](https://postimg.cc/jwgYcBKy)


__Soal 3c__

Pada awalnya, di direktori no3 dengan path ```/home/rizqi/no3``` adalah sebagai berikut :

[![3c-1.jpg](https://i.postimg.cc/fbtYXxRn/3c-1.jpg)](https://postimg.cc/Z9m9zvNV)

Kemudian saya mencoba perintah soal 3c pada terminal :

[![3c-2.jpg](https://i.postimg.cc/br8nKWHp/3c-2.jpg)](https://postimg.cc/8JKsvy00)

Isi dari direktori no3 (tempat program berjalan) menjadi sebagai berikut:

[![3c-3.jpg](https://i.postimg.cc/mkw9Rj8M/3c-3.jpg)](https://postimg.cc/YLv0z6cq)

Isi dari salah satu direktori tipe file adalah sebagai berikut :

[![3c-4.jpg](https://i.postimg.cc/h4bT94cj/3c-4.jpg)](https://postimg.cc/PNJCHjLs)


### Kendala

Tidak ada kendala dalam pengerjaan revisi modul 3 soal 3.




