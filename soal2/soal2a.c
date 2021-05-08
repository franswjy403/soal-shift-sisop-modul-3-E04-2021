#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(){
    key_t key = 1234;
    int *value;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    int result [4][6];
    int bar, kol, cent;
    int sum = 0;

    int A [4][3]={
        {2,3,1},
        {6,5,4},
        {7,9,8},
        {10,12,11}
    };

    int B [3][6]={
        {12, 13, 11, 15, 10, 17},
        {4, 6, 1, 3, 5, 2},
        {7, 9, 8, 14, 16, 18}
    };

    for (bar = 0; bar<4; bar++){
        for (kol = 0; kol<6; kol++){
            for (cent = 0; cent<3; cent++){
                sum = sum + A[bar][cent]*B[cent][kol];
            }
            result[bar][kol] = sum;
            sum=0;
        }
    }

    printf("Result:\n");
    for (bar = 0; bar<4; bar++){
        for (kol = 0; kol<6; kol++){
            printf("%d ", result[bar][kol]);
        }
        printf("\n");
    }

    printf("Sending:\n");
    for (bar = 0; bar<4; bar++){
        for (kol = 0; kol<6; kol++){
            *value = result[bar][kol];
            sleep(1);
            printf("cek %d ", *value);
        }
        printf("\n");
    }

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}