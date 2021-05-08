#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

int faktorial(int a){
    if (a!=0) return a + faktorial (a-1);
    else return a;
}

void *faktorial_util(void* a){
    int b = (intptr_t) a;
    printf ("%d ", faktorial(b));
}

void main(){
    
    key_t key = 1234;
    int *value;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    int arr[4][6];

    pthread_t tid[50];
    int i, j, l, ind = 0, temp;

    for (i=0; i<4;i++){
        for (j=0; j<6;j++){
            arr[i][j]=*value;
            temp = arr[i][j];
            pthread_create(&tid[ind], NULL, &faktorial_util, (void*) (intptr_t) temp);
            printf("%d ", arr[i][j]);
            printf("\t");
            ind++;
            sleep(1);
        }
        printf("\n");
    }

    for (l=0; l<ind; l++){
        pthread_join(tid[l], NULL);
    }
    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}