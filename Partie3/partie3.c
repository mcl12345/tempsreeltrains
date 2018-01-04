#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> // pour les semaphores
#include <fcntl.h>     // pour les flags O_CREAT, O_EXCL, ...
#include <unistd.h>    // sleep()
#include <string.h>
#include <time.h>       // time_t, struct tm, difftime, time, mktime

#define _TEMPS_   3

clock_t temps;

pthread_rwlock_t lock;

char debut_train1[2];
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

void* TrainUn(void* p) {
    int i = 0;
    char train1[4][8] = {"A --> B", "B --> C", "C --> B", "B --> A"};

    while (i >= 0) {
        // Récupère le trajet du train 1 en global :
        substring(0, 1, train1[i%4], debut_train1, sizeof(debut_train1));
        substring(6, 1, train1[i%4], fin_train1, sizeof(fin_train1));

        // Compare avec les autres trains le trajet et bloque si la trajectoire inverse d'un autre train est déjà en cours :
        if(strcmp(debut_train1, fin_train2) == 0 && strcmp(fin_train1, debut_train2) == 0) {
            pthread_rwlock_rdlock(&lock);
            temps = clock();
            printf("%f - ", (double) temps / 100);
            printf("Train 1 en approche en sens inverse %s\n", train1[i%4] );
            sleep(rand()%_TEMPS_);
            temps = clock();
            printf("%f - ", (double) temps / 100);
            printf("Le train 1 est arrivé à la gare : %s\n\n", fin_train1);
            pthread_rwlock_unlock(&lock);
        }
        // Compare avec les autres trains le trajet et bloque si la trajectoire inverse d'un autre train est déjà en cours :
        else if(strcmp(debut_train1, fin_train3) == 0 && strcmp(fin_train1, debut_train3) == 0) {
            pthread_rwlock_rdlock(&lock);
            temps = clock();
            printf("%f - ", (double) temps / 100);
            printf("\nTrain 1 en approche en sens inverse %s\n", train1[i%4] );
            sleep(rand()%_TEMPS_);
            temps = clock();
            printf("%f - ", (double) temps / 100);
            printf("Le train 1 est arrivé à la gare : %s\n\n", fin_train1);
            pthread_rwlock_unlock(&lock);
        } else {
            pthread_rwlock_rdlock(&lock);
            temps = clock() / 100;
            printf("%f - ", (double) temps);
            printf("Train 1 : %s\n", train1[i%4]);
            sleep(rand()%_TEMPS_);
            temps = clock();
            printf("%f - ", (double) temps / 100);
            printf("Le train 1 est arrivé à la gare : %s\n\n", fin_train1);
            fflush(stdout);
            pthread_rwlock_unlock(&lock);
        }

        i++;
    }
    return NULL;
}

void* TrainDeux(void* p) {
    int i = 0;
    char train2[5][8] = {"A --> B", "B --> D", "D --> C", "C --> B", "B --> A"};

    while (i >= 0) {
        // Récupère le trajet du train 2 en global :
        substring(0, 1, train2[i%5], debut_train2, sizeof(debut_train2));
        substring(6, 1, train2[i%5], fin_train2, sizeof(fin_train2));

        // Compare avec les autres trains le trajet et bloque si la trajectoire inverse d'un autre train est déjà en cours :
        if(strcmp(debut_train2, fin_train3) == 0 && strcmp(fin_train2, debut_train3) == 0) {
          pthread_rwlock_rdlock(&lock);
          temps = clock();
          printf("%f - ", (double) temps / 100);
          printf("Train 2 en approche en sens inverse %s\n" , train2[i%5]);
          sleep(rand()%_TEMPS_);
          temps = clock();
          printf("%f - ", (double) temps / 100);
          printf("Le train 2 est arrivé à la gare : %s\n\n", fin_train2);
          pthread_rwlock_unlock(&lock);
        }
        // Compare avec les autres trains le trajet et bloque si la trajectoire inverse d'un autre train est déjà en cours :
        else if(strcmp(debut_train2, fin_train1) == 0 && strcmp(fin_train2, debut_train1) == 0) {
          pthread_rwlock_rdlock(&lock);
          temps = clock();
          printf("%f - ", (double) temps / 100);
          printf("Train 2 en approche en sens inverse %s\n" , train2[i%5]);
          sleep(rand()%_TEMPS_);
          temps = clock();
          printf("%f - ", (double) temps / 100);
          printf("Le train 2 est arrivé à la gare : %s\n\n", fin_train2);
          pthread_rwlock_unlock(&lock);
        }
        else {
          pthread_rwlock_rdlock(&lock);
          temps = clock() / 100;
          printf("%f - ", (double) temps);
          printf("Train 2 : %s\n", train2[(i%5)]);
          sleep(rand()%_TEMPS_);
          temps = clock();
          printf("%f - ", (double) temps / 100);
          printf("Le train 2 est arrivé à la gare : %s\n\n", fin_train2);
          fflush(stdout);
          pthread_rwlock_unlock(&lock);

        }

        i++;
    }
    return NULL;
}

void* TrainTrois(void* p) {
    int i = 0;
    char train3[5][8] = {"A --> B", "B --> D", "D --> C", "C --> E", "E --> A"};

    while (i >= 0) {
        // Récupère le trajet du train 3 en global :
        substring(0, 1, train3[i%5], debut_train3, sizeof(debut_train3));
        substring(6, 1, train3[i%5], fin_train3, sizeof(fin_train3));

        // Compare avec les autres trains le trajet et bloque si la trajectoire inverse d'un autre train est déjà en cours :
        if(strcmp(debut_train3, fin_train2) == 0 && strcmp(fin_train3, debut_train2) == 0) {
          pthread_rwlock_rdlock(&lock);
          temps = clock();
          printf("%f - ", (double) temps / 100);
          printf("Train 3 en approche en sens inverse %s\n", train3[(i%5)] );
          sleep(rand()%_TEMPS_);
          temps = clock();
          printf("%f - ", (double) temps / 100);
          printf("Le train 3 est arrivé à la gare : %s\n\n", fin_train3);
          pthread_rwlock_unlock(&lock);
        }
        // Compare avec les autres trains le trajet et bloque si la trajectoire inverse d'un autre train est déjà en cours :
        else if(strcmp(debut_train3, fin_train1) == 0 && strcmp(fin_train3, debut_train1) == 0) {
          pthread_rwlock_rdlock(&lock);
          temps = clock();
          printf("%f - ", (double) temps / 100);
          printf("Train 3 en approche en sens inverse %s\n", train3[(i%5)]);
          sleep(rand()%_TEMPS_);
          temps = clock();
          printf("%f - ", (double) temps / 100);
          printf("Le train 3 est arrivé à la gare : %s\n\n", fin_train3);
          pthread_rwlock_unlock(&lock);
        }
        else {
          pthread_rwlock_rdlock(&lock);
          temps = clock() / 100;
          printf("%f - ", (double) temps);
          printf("Train 3 : %s\n", train3[(i%5)]);
          sleep(rand()%_TEMPS_);
          temps = clock();
          printf("%f - ", (double) temps / 100);
          printf("Le train 3 est arrivé à la gare : %s\n\n", fin_train3);
          fflush(stdout);
          pthread_rwlock_unlock(&lock);
        }

        i++;
    }
    return NULL;
}

int main() {
    pthread_t ID[3];

    pthread_create(&ID[0], NULL, TrainDeux, NULL);
    pthread_create(&ID[1], NULL, TrainTrois, NULL);

    TrainUn(NULL);

    pthread_join(ID[0], 0);
    pthread_join(ID[1], 0);

    pthread_rwlock_destroy(&lock);


    return 0;
}
