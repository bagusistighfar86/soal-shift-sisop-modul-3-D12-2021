#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

char *ubahlc(char *string){  //mengubah string menjadi lower case
	unsigned char *temp;
	temp = (unsigned char *)string;
	while(*temp){
		*temp = tolower(*temp);
		temp++;
	}
	return string;
}

char *getnama(char *direc){ //fungsi untuk hanya mengambil nama file
	char *nama;
	nama = strrchr(direc, '/'); //melihat kemunculan terakhir "/" dan return string mulai dari "/" sampai selesai
	if(nama == direc){
		return "";
	}
	return nama + 1; //men skip tanda "/" di nama[0]
}

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

void* kategorifile(void *arg){ //untuk mengkategorikan file
	char *pathasal;
	char pathfile[200];
	char *pathtipe;
	char tipe[500];
	pathasal = (char *)arg; //mengambil path file
	memcpy(pathfile, pathasal, 400); //menyalin path file
	pathtipe = gettipe(pathasal); //memanggil fungsi gettipe untuk mengambil tipe file
	strcpy(tipe, pathtipe); //menyalin tipe
	DIR *dir = opendir(pathtipe);
	if(dir){
		closedir(dir); //kalo direktori sudah ada langsung ditutup
	}
	else if(ENOENT == errno){ //direktori belum ada, bikin dengan nama tipe file
		mkdir(pathtipe, 0755);
	} 
	char *nama;
	char *pathprogram;
	char tujuan[200] = {};
	nama = getnama(pathfile); //mengambil nama file
	pathprogram = getenv("PWD"); //mengambil path dimana program dijalankan
	strcat(tujuan,pathprogram);
	strcat(tujuan,"/");
	strcat(tujuan,tipe);
	strcat(tujuan,"/");
	strcat(tujuan,nama); //merangkai path file baru 
	rename(pathfile, tujuan); //me rename path file lama menjadi path file baru
}

void kategorifolder(char *pathfolder, int sizethread){ //untuk mengkategorikan semua file dalam folder
	DIR *fd = opendir(pathfolder);
	struct dirent *dp;
	pthread_t tid[sizethread]; //inisialisasi thread
	int counter = 0;
	char namafile[200][200] ={};
	while((dp = readdir(fd)) != NULL){
		if(dp->d_type == DT_REG){ // berupa file reguler
			strcat (namafile[counter],pathfolder);
			strcat(namafile[counter],"/");
			strcat(namafile[counter],dp->d_name); //merangkai path file didalam folder yang akan dipindah
			pthread_create(&tid[counter], NULL, kategorifile, (void *)namafile[counter]); //buat thread untuk mengkategorikan file
			counter++;
		}
		else if((dp->d_type == DT_DIR) && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) { //berupa direktori
            char pathfolderisi[200] = {};
            strcat (pathfolderisi,pathfolder);
            strcat (pathfolderisi,"/");
            strcat (pathfolderisi,dp->d_name);  //merangkai path folder didalam folder
            DIR *fd2 = opendir(pathfolderisi); //open folder di dalam folder
            struct dirent *dp2;
			int sizethreadfold = 0;
			while((dp2 = readdir(fd2)) != NULL){
				if(dp2->d_type == DT_REG){
					sizethreadfold++;
				}
			}
			kategorifolder(pathfolderisi, sizethreadfold); //rekursi untuk folder didalam folder
			closedir(fd2);
        	}
	}
	for(int i = 0; i < sizethread; i++){ //join thread untuk menunggu semua thread terminated
		pthread_join(tid[i], NULL);
	} 
    	closedir(fd);
}

int main(int argc, char *argv[]){
	if(argc == 0){ 
		exit(EXIT_FAILURE);
	}

	if(argc > 2 && strcmp(argv[1], "-f") == 0){ //soal 3a
		pthread_t tid[argc-2]; //inisialisasi thread
		int counter = 0;
		for(int i = 2; i < argc; i++){
			if(access(argv[i], F_OK) == 0){ //file ada
				pthread_create(&tid[counter], NULL, kategorifile, (void *)argv[i]); //mengkategorikan file
				counter++;
				printf("File %d : Berhasil Dikategorikan\n", i-1);
			}
			else printf("File %d : Sad, gagal :(\n", i-1);
		}
		for(int i = 0; i < counter; i++){ //join thread untuk menunggu semua threaad terminated
			pthread_join(tid[i], NULL); 
		}
		return 0;
	}

	else if(argc == 3 && strcmp(argv[1], "-d") == 0){ //soal 3b
		DIR *fd = opendir(argv[2]);
		if(fd){
			struct dirent *dp;
			int sizethread = 0;
			while((dp = readdir(fd)) != NULL){
				if(dp->d_type == DT_REG){ //file regular
					sizethread++;
				}
			}
			kategorifolder(argv[2], sizethread); // memanggil fungsi kategori folder
			closedir(fd);
			printf("Direktori sukses disimpan!\n");
		}
		else if(ENOENT == errno){ // direktori tidak ada
			printf("Yah, gagal disimpan :(\n");
		} 
	}

	else if(argc == 2 && strcmp(argv[1], "*") == 0){ //soal 3c
		char *pathprogram;
		int sizethread = 0;
		pathprogram = getenv("PWD"); // mengambil path dimana program dijalankan
		DIR *dir = opendir(pathprogram);
		struct dirent *dp;
		while((dp = readdir(dir)) != NULL){
			if(dp->d_type == DT_REG){
				sizethread++;
			}
		}
		kategorifolder(pathprogram, sizethread); // memanggil fungsi kategori folder
		closedir(dir);
	}
	else{
		printf("Format input salah\n"); //diluar perintah soal no 3
		return 0;
	}
}
