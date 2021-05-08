#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

struct arg_struct
{
    int num;
    int batas;
};


int faktorial(int b, int c){
    int res = 1, i, start;

    if (b==0 || c==0) return 0;
    if (c>=b) start = 2;
    else start = b-c+1;
    for (i = start; i <= b; i++)
        res *= i;
    return res;
}

void *faktorial_util(void* a){
    struct arg_struct *args = a;
    int b = args->num;
    int c = args->batas;
    printf ("%d ", faktorial(b, c));
}

void main(){
    
    key_t key = 1234;
    int *value;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    int arr[4][6];
    int pengali[4][6]={
        {7, 3, 2, 4, 0, 14},
        {3, 2, 2, 1, 0, 2},
        {2, 4, 3, 2, 1, 2},
        {0, 0, 2, 2, 3, 0}
    };
    pthread_t tid[24];
    int i, j, l, ind = 0;
    struct arg_struct temp;

    for (i=0; i<4;i++){
        for (j=0; j<6;j++){
            arr[i][j]=*value;
            temp.num = arr[i][j];
            temp.batas = pengali[i][j];
            pthread_create(&tid[ind], NULL, &faktorial_util, (void*) &temp);
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