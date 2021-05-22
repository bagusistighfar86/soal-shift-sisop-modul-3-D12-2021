# soal-shift-sisop-modul-3-D12-2021
## Anggota Kelompok
| Nama | NRP |
|------|-----|
|Muhammad Bagus Istighfar|05111940000049|
|Rizqi Rifaldi|05111940000068|
|Afdhal Ma'ruf Lukman|05111940007001|

## Soal 1
## Soal 2
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


