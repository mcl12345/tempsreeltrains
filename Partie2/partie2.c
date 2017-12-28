#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> // pour les semaphores
#include <fcntl.h>     // pour les flags O_CREAT, O_EXCL, ...
#include <unistd.h>    // sleep()
#include <string.h>

sem_t * trainA;
sem_t * trainB;
char debut_train1[2];
sem_t * trainC;
char fin_train1[2];
char debut_train2[2];
char fin_train2[2];
char debut_train3[2];
char fin_train3[2];

char *substring(size_t start, size_t stop, const char *src, char *dst, size_t size)
{
   int count = stop - start;
   if ( count >= --size ) {
      count = size;
   }
   sprintf(dst, "%.*s", count, src + start);
   return dst;
}

void* a(void* p) {
    int i = 0;
    char train1[4][8] = {"A --> B", "B --> C", "C --> B", "B --> A"};

    while (i >= 0) {
        // Récupère le trajet du train 1 en global :
        substring(0, 1, train1[i%4], debut_train1, sizeof(debut_train1));
        substring(6, 1, train1[i%4], fin_train1, sizeof(fin_train1));

        // Compare avec les autres trains le trajet :
        if(strcmp(debut_train1, fin_train2) == 0 && strcmp(fin_train1, debut_train2) == 0) {
            sem_wait(trainA);
            printf("\ntrain 1 en approche en sens inverse %s\n", train1[i%4] );
            printf("\n1 %s %s\n", debut_train1, fin_train1);
            printf("%s %s\n", debut_train2, fin_train2);
            sem_post(trainB);
        }
        else if(strcmp(debut_train1, fin_train3) == 0 && strcmp(fin_train1, debut_train3) == 0) {
            sem_wait(trainA);
            printf("\ntrain 1 en approche en sens inverse %s\n", train1[i%4] );
            printf("\n2 %s %s\n", debut_train1, fin_train1);
            printf("%s %s\n", debut_train3, fin_train3);
            sem_post(trainB);
        } else {
            printf("\ntrain 1 en approche : %s\n", train1[i%4]);
            sleep(3);
            fflush(stdout);
        }

        i++;
    }
    return NULL;
}

void* b(void* p) {
    int i = 0;
    char train2[5][8] = {"A --> B", "B --> D", "D --> C", "C --> B", "B --> A"};

    while (i >= 0) {
        // Récupère le trajet du train 2 en global :
        substring(0, 1, train2[i%5], debut_train2, sizeof(debut_train2));
        substring(6, 1, train2[i%5], fin_train2, sizeof(fin_train2));

        // Compare avec les autres trains le trajet :
        if(strcmp(debut_train2, fin_train3) == 0 && strcmp(fin_train2, debut_train3) == 0) {
            sem_wait(trainB);
            printf("train 2 en approche en sens inverse %s\n" , train2[i%5]);
            printf("\n3 %s %s\n", debut_train2, fin_train2);
            printf("%s %s\n", debut_train3, fin_train3);
            sem_post(trainC);
        }
        else if(strcmp(debut_train2, fin_train1) == 0 && strcmp(fin_train2, debut_train1) == 0) {
            sem_wait(trainB);
            printf("train 2 en approche en sens inverse %s\n" , train2[i%5]);
            printf("\n4 %s %s\n", debut_train1, fin_train1);
            printf("%s %s\n", debut_train2, fin_train2);
            sem_post(trainC);
        }
        else {
            printf("train 2 en approche : %s\n", train2[(i%5)]);
            sleep(3);
            fflush(stdout);
        }

        i++;
    }
    return NULL;
}

void* c(void* p) {
    int i = 0;
    char train3[5][8] = {"A --> B", "B --> D", "D --> C", "C --> E", "E --> A"};

    while (i >= 0) {
        // Récupère le trajet du train 3 en global :
        substring(0, 1, train3[i%5], debut_train3, sizeof(debut_train3));
        substring(6, 1, train3[i%5], fin_train3, sizeof(fin_train3));

        // Compare avec les autres trains le trajet :
        if(strcmp(debut_train3, fin_train2) == 0 && strcmp(fin_train3, debut_train2) == 0) {
            sem_wait(trainC);
            printf("train 3 en approche en sens inverse %s\n", train3[(i%5)] );
            printf("\n5 %s %s\n", debut_train3, fin_train3);
            printf("%s %s\n", debut_train2, fin_train2);
            sem_post(trainA);
        }
        else if(strcmp(debut_train3, fin_train1) == 0 && strcmp(fin_train3, debut_train1) == 0) {
            sem_wait(trainC);
            printf("train 3 en approche en sens inverse %s\n", train3[(i%5)]);
            printf("\n6 %s %s\n", debut_train3, fin_train3);
            printf("%s %s\n", debut_train1, fin_train1);
            sem_post(trainA);
        }
        else {
            printf("train 3 en approche : %s\n", train3[(i%5)]);
            sleep(3);
            fflush(stdout);
        }

        i++;
    }
    return NULL;
}

int main() {
    pthread_t ID[3];

    trainA = sem_open("trainA", O_CREAT, S_IRUSR | S_IWUSR, 3);
    trainB = sem_open("trainB", O_CREAT, S_IRUSR | S_IWUSR, 0);
    trainC = sem_open("trainC", O_CREAT, S_IRUSR | S_IWUSR, 0);

    pthread_create(&ID[0], NULL, b, NULL);
    pthread_create(&ID[1], NULL, c, NULL);

    a(NULL);

    pthread_join(ID[0], 0);
    pthread_join(ID[1], 0);

    sem_close(trainA);
    sem_close(trainB);
    sem_close(trainC);
    sem_unlink("trainA");
    sem_unlink("trainB");
    sem_unlink("trainC");

    return 0;
}
