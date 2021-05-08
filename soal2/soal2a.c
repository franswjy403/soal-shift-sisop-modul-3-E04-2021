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
        {3,4,5},
        {5,6,3}
    };

    int B [3][6]={
        {1, 2, 5, 7, 10, 1},
        {1, 6, 1, 3, 5, 2},
        {1, 4, 8, 6, 8, 1}
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
            printf("%d ", *value);
        }
        printf("\n");
    }

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}