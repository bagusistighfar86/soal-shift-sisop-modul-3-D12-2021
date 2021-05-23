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

void* kali(void* argvar) {
    int* arg = (int*)argvar;
    int i, n = 0;

    for (i = 0; i < bk; i++) 
        n += arg[i] * arg[i + bk];

    int* p = (int*)malloc(sizeof(int));
    *p = n;
    pthread_exit(p);
}

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

    int err, k;
    pthread_t* tid = (pthread_t*)malloc((24) * sizeof(pthread_t));

    int count = 0;
    int* arg = NULL;

    for (i = 0; i < baris; i++) {
        for (j = 0; j < kolom; j++) {
            arg = (int*)malloc((6) * sizeof(int));
            // arg[0] = bk;

            for (k = 0; k < bk; k++) {
                arg[k] = m1[i][k];
                // printf("1.arg[%d] = m1[%d][%d] = %d\n",k+1,i,k,m1[i][k]);
            }
            for (k = 0; k < bk; k++) {
                arg[k + bk] = m2[k][j];
                // printf("2.arg[%d] = m1[%d][%d] = %d\n",k+1+bk,k,j,m2[k][j]);
            }

            err = pthread_create(&(tid[count++]), NULL, &kali, (void*)arg);
            if (err != 0)
                printf("\n Can't create thread : [%s]", strerror(err));
        }
    }

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
