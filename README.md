# soal-shift-sisop-modul-2-E04-2021
### Anggota:
1. [Frans Wijaya](https://github.com/franswjy403)	(05111940000098)
2. [Jagad Wijaya P.](https://github.com/Jagadwp)	(05111940000132)
3. [Fidhia Ainun K.](https://github.com/fidhiaaka)	(05111940000203)

### List soal:
1. [Soal 1](#soal1)
2. [Soal 2](#soal2)
3. [Soal 3](#soal3)

## Soal 1

## Soal 2
Diminta membantu Loba mengerjakan tugasnya, dengan detail sebagai berikut:
### Soal 2a
Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20.

Berikut matriks testcase untuk soal 2a.
```c
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
```
Blok kode yang berfungsi untuk mengalikan matriks testcase. Karena soal meminta perkalian matriks 4 x 3 dan 3 x 6, maka disediakan matrix result berukuran 4 x 6.
```c
for (bar = 0; bar<4; bar++){
        for (kol = 0; kol<6; kol++){
            for (cent = 0; cent<3; cent++){
                sum = sum + A[bar][cent]*B[cent][kol];
            }
            result[bar][kol] = sum;
            sum=0;
        }
    }
```
Setelah dikalikan dan disimpan hasilnya di array result, karena di soal 2b hasil dari soal 2a akan dipakai, maka perlu disediakan dulu blok shared memory.
```c
key_t key = 1234;
    int *value;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    //
    //
    //
    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
```
Untuk menyimpan result sementara waktu di shared memory, digunakan blok kode berikut.
```c
for (bar = 0; bar<4; bar++){
        for (kol = 0; kol<6; kol++){
            *value = result[bar][kol];
            sleep(1);
            printf("%d ", *value);
        }
        printf("\n");
    }
```
Hasil akan dikirim sel per sel, sehingga agar dapat diakses secara tepat waktu dan juga sel per sel, program soal 2a dan 2b perlu dijalankan bersamaan.
### Soal 2b
Membuat program untuk menghitung faktorial dari hasil soal 2a, dengan ketentuan panjang faktorialnya.

Untuk mengakses shared memory dari soal 2a, dilakukan pengaksesan memory dengan key sama.
```c
key_t key = 1234;
    int *value;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    //
    //
    //
    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
```
Testcase untuk soal 2b sebagai berikut.
```c
int arr[4][6];
    int pengali[4][6]={
        {7, 3, 2, 4, 0, 14},
        {3, 2, 2, 1, 0, 2},
        {2, 4, 3, 2, 1, 2},
        {0, 0, 2, 2, 3, 0}
    };
```
Karena sesuai permintaan soal, setiap perhitungan selnya dilakukan melalui thread.
```c
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
```
Adapun fungsi untuk melakukan perhitungan faktorialnya sebagai berikut.
```c
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
```
### Soal 2c
Membuat sebuah program untuk melihat 5 proses teratas yang memakan banyak resource. Untuk dapat menjalankan serangkaian command, diperlukan pipe untuk mengatur aliran data dan hasil command-command.

Karena ada tiga command yang dijalankan, maka digunakan 2 pipe. Pipe pp digunakan untuk menyalurkan hasil command ps -aux ke sort -nrk 3,3. Setelah diproses di sort -nrk 3,3, hasilnya akan dikirim melalui pipe pp1 ke command head -5.
```c
if(pipe(p) == -1)
    {
        exit(EXIT_FAILURE);
    }
    if (pipe(p1)==-1){
        exit(EXIT_FAILURE);
    }
    pp2 = fork();
    if (pp2<0) exit(EXIT_FAILURE);
    if (pp2==0){
        close(p[0]);
        dup2(p[1], STDOUT_FILENO);
        char *argv[] = {"ps", "aux", NULL};
        execv("/bin/ps", argv);
    }
    while(wait(&status)>0);
    close(p[1]);
    dup2(p[0], STDIN_FILENO);
    
    pp = fork();
    if(pp < 0){
        exit(EXIT_FAILURE);
    }
    if(pp==0){
        close(p1[0]);
        dup2(p1[1], STDOUT_FILENO);
        char* argv[]={"sort", "-nrk", "3,3", NULL};
        execv("/bin/sort", argv);
    }

    while(wait(&status1)>0);
    close(p1[1]);
    dup2(p1[0], STDIN_FILENO);
    char* argv[]={"head", "-5", NULL};
    execv("/bin/head", argv);
```
## Soal 3
Membuat sebuah program yang dapat mengelompokkan file-file berdasarkan ekstensinya.

Mode pertama adalah -f, yang mana dapat mengelompokkan banyak file (inputan berupa path filenya) sekaligus.
```c
if(strcmp(argv[1],"-f")==0)
    {
        int index = 0;
        for (int i = 2; i < argc; i++)
        {
        strcpy(args.asal, argv[i]);
        pthread_create(&tid[index], NULL, pindahf, (void *)&args);
        sleep(1);
        index++;
        }
        for (int i = 0; i < index; i++) {
            pthread_join(tid[i], NULL);
        }
    }
```
Pada mode -f ini, fungsi utama yang berperan penting adalah pindahf dan pindahFile, dimana pindahf sebagai fungsi penjembatan.
```c
void pindahFile(char *argv, char *cwd){
    int isHidden=0;
    char string[1000];
    strcpy(string, argv);
    int isFile = is_regular_file(string);
    char dot = '.'; 
    char slash = '/';
    char* tipe = strrchr(string, dot); 
    char* nama = strrchr(string, slash);
    if (nama[1]=='.'){
        isHidden = 1;
    }

    char tipeLow[100];
    if(tipe)
    {
        if((tipe[strlen(tipe)-1] >= 'a' && tipe[strlen(tipe)-1] <= 'z') || (tipe[strlen(tipe)-1] >= 'A' && tipe[strlen(tipe)-1] <= 'Z'))
        {
            strcpy(tipeLow, tipe);
            for(int i = 0; tipeLow[i]; i++){
                tipeLow[i] = tolower(tipeLow[i]);
            }
        }
    }
    else
    {
        if(!isFile){
        printf("ini adalah folder, salah argumen\n");
        return;
        }
        else
        {
        strcpy(tipeLow, " Unknown"); //tanpa ekstensi
        }
    }
        if (isHidden) {
            strcpy (tipeLow, " Hidden");
        }
        mkdir((tipeLow + 1), 0777); //Folder Ekstensi

        size_t len = 0 ;
        char a[1000] ;
        strcpy(a, argv);
        char b[1000] ;
        strcpy(b, cwd);
        strcat(b, "/");
        strcat(b, tipeLow+1);
        strcat(b, nama);
        if (!rename(a, b))printf("Berhasil Dikategorikan\n");
        else printf("Sad, Gagal :(\n");
        
        remove(a);
}
void *pindahf(void* arg){
    arg_struct args = *(arg_struct*) arg;
    pindahFile(args.asal, args.cwd);
    pthread_exit(0);
}
```
Pada fungsi pindahFile, terdapat pemanggilan fungsi is_regular_file yang berfungsi untuk mendeteksi apakah sebuah file merupakan file atau folder.
```c
int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
```
Untuk mode -d, menggunakan fungsi serupa, hanya saja terdapat perbedaan kondisi print dan apa yang di print. Fungsinya pun adalah pindahd dan pindahFiles. Tetapi sebelum masuk ke fungsi-fungsi tersebut, folder yang ingin dikelompokkan terlebih dahulu dibaca file per filenya melalui fungsi sortHere.
Berikut fungsi sortHere.
```c
void sortHere(char *asal){
    arg_struct args;
    flag=1;
    strcpy(args.cwd,"/home/frans0416/soal3");
    DIR *dirp;
    struct dirent *entry;
    dirp = opendir(asal);
    int index = 0;
    while((entry = readdir(dirp)) != NULL)
    {
        if(entry->d_type == DT_REG)
        {
            char namafile[305];
            sprintf(namafile, "%s%s", asal, entry->d_name);
            strcpy(args.asal, namafile);
            if(strcmp(namafile, "/home/frans0416/soal3/soal3.c")!=0)
            {
                pthread_create(&tid[index], NULL, pindahd, (void *)&args);
                printf("%s\n", namafile);
                sleep(1);
                index++;
            }
        }
    }
    if (!flag) printf("Yah gagal disimpan\n");
    else printf("Direktori sudah disimpan\n");
}
```
Berikut fungsi pindahd dan pindahFiles
```c
void pindahFiles(char *argv, char *cwd){
    int isHidden=0;
    char string[1000];
    strcpy(string, argv);
    int isFile = is_regular_file(string);
    char dot = '.'; 
    char slash = '/';
    char* tipe = strrchr(string, dot); 
    char* nama = strrchr(string, slash);
    if (nama[1]=='.'){
        isHidden = 1;
    }
    char tipeLow[100];
    if(tipe)
    {
        if((tipe[strlen(tipe)-1] >= 'a' && tipe[strlen(tipe)-1] <= 'z') || (tipe[strlen(tipe)-1] >= 'A' && tipe[strlen(tipe)-1] <= 'Z'))
        {
            strcpy(tipeLow, tipe);
            for(int i = 0; tipeLow[i]; i++){
                tipeLow[i] = tolower(tipeLow[i]);
            }
        }
    }
    else
    {
        if(!isFile){
        printf("ini adalah folder, salah argumen\n");
        return;
        }
        else
        {
        strcpy(tipeLow, " Unknown"); //tanpa ekstensi
        }
    }
        if (isHidden) {
            strcpy (tipeLow, " Hidden");
        }
        mkdir((tipeLow + 1), 0777); //Folder Ekstensi

        size_t len = 0 ;
        char a[1000] ;
        strcpy(a, argv);
        char b[1000] ;
        strcpy(b, cwd);
        strcat(b, "/");
        strcat(b, tipeLow+1);
        strcat(b, nama);
        if (rename(a,b)) flag=0;
        ///home/frans0416/soal3
        remove(a);
}
void *pindahd(void* arg){
    arg_struct args = *(arg_struct*) arg;
    pindahFiles(args.asal, args.cwd);
    pthread_exit(0);
}
```
Untuk mode yang terakhir, \*, menggunakan alur pemanggilan fungsi yang sama dengan proses -d, hanya saja, alamat folder yang dikirim adalah alamat folder tetap, yaitu current working directorynya.
```c
else if(strcmp(argv[1],"*")==0)
    {
        char asal[] = "/home/frans0416/soal3/";
        
        sortHere(asal);
    }
```