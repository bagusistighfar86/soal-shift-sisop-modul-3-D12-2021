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

Source Code :
```C
char *ubahlc(char *string){
  unsigned char *temp;
  temp = (unsigned char *)string;
  while(*temp){
    *temp = tolower(*temp); 
    temp++;
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
	char *temp = strrchr(direc, '/'); //mengambil nama file
	if(temp[1] == '.'){ // file hidden
		return tipehid;
	}
	while(i < strlen(temp) && temp[i] != '.'){ //mengecek ada titik di nama file
		i++;
	}
	if(i == strlen(temp)){ // file unknown
		return tipeunk;
	} 
	int j = i;
	char tipe[400];
	while(i < strlen(temp)){  //mengambil tipe file
		tipe[i-j] = temp[i];
		i++;
	}
	return ubahlc(tipe + 1); //memanggil fungsi ubahlc untuk mengubah tipe file ke lowercase dengan men skip titik di tipe[0]
}
```
Pada fungsi ini, pertama akan mengambil nama file dari path dengan menggunakan ```strrchr```, kemudian mengecek nama file, jika indeks 1 dari nama file (indeks 0 adalah "/") merupakan "." , berarti file tersebut akan dikategorikan sebagai Hidden. Kemudian fungsi mengecek keberadaan tanda "." pada nama file menggunakan loop while, dan jika tidak ada sampai akhir dari string, maka file akan dikategorikan sebagai Unknown. Jika terdapat tanda "." pada string, selanjutnya akan masuk ke loop kedua untuk menyalin tipe file dan fungsi akan mengembalikan string tipe mulai dari indeks 1 (indeks 0 adalah tanda "." .
