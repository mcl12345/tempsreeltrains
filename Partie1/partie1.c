#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> // pour les semaphores
#include <fcntl.h>     // pour les flags O_CREAT, O_EXCL, ...
#include <unistd.h>    // sleep()
#include <string.h>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */

pthread_mutex_t mutex;
//pthread_mutex_t mutex2;
//pthread_mutex_t mutex3;
char debut_train1[2];
char fin_train1[2];
char debut_train2[2];
char fin_train2[2];
char debut_train3[2];
char fin_train3[2];

//time_t timer;
 clock_t temps;

/*
gcc -pthread partie1.c
*/

char *substring(size_t start, size_t stop, const char *src, char *dst, size_t size)
{
   int count = stop - start;
   if ( count >= --size ) {
      count = size;
   }
   sprintf(dst, "%.*s", count, src + start);
   return dst;
}

void* TrainA(void* p) {
    int i = 0;
    char train1[4][8] = {"A --> B", "B --> C", "C --> B", "B --> A"};

    while (i >= 0) {
        // Récupère le trajet du train 1 en global :
        substring(0, 1, train1[i%4], debut_train1, sizeof(debut_train1));
        substring(6, 1, train1[i%4], fin_train1, sizeof(fin_train1));
        // Compare avec les autres trains le trajet :
        if(strcmp(debut_train1, fin_train2) == 0 && strcmp(fin_train1, debut_train2) == 0) {
            pthread_mutex_lock(&mutex);
            printf("Train 1 en approche en sens inverse %s\n", train1[i%4] );
            sleep(3);
            printf("Le train 1 est arrivé à la gare : %s\n\n", fin_train1);
            printf("\n1 %s %s\n", debut_train1, fin_train1);
            printf("%s %s\n", debut_train2, fin_train2);
            pthread_mutex_unlock(&mutex);
        }
        else if(strcmp(debut_train1, fin_train3) == 0 && strcmp(fin_train1, debut_train3) == 0) {
          pthread_mutex_lock(&mutex);
          printf("Train 1 en approche en sens inverse %s\n", train1[i%4] );
          sleep(3);
          printf("Le train 1 est arrivé à la gare : %s\n\n", fin_train1);
          printf("\n1 %s %s\n", debut_train1, fin_train1);
          printf("%s %s\n", debut_train3, fin_train3);
          pthread_mutex_unlock(&mutex);
        }
        else {
            // Trajet en cours avec 3 secondes de temps de trajet
            //time(&timer);  /* get current time; same as: timer = time(NULL)  */
            temps = clock();
            printf("%f\n", (double) temps);
            printf("Train 1 : %s\n", train1[i%4]);
            sleep(3);
            printf("Le train 1 est arrivé à la gare : %s\n\n", fin_train1);
            fflush(stdout);
        }
        i++;
    }
    return NULL;
}

void* TrainB(void* p) {
    int i = 0;
    char train2[5][8] = {"A --> B", "B --> D", "D --> C", "C --> B", "B --> A"};

    while (i >= 0) {
        // Récupère le trajet du train 2 en global :
        substring(0, 1, train2[i%5], debut_train2, sizeof(debut_train2));
        substring(6, 1, train2[i%5], fin_train2, sizeof(fin_train2));

        // Compare avec les autres trains le trajet :
        if(strcmp(debut_train2, fin_train3) == 0 && strcmp(fin_train2, debut_train3) == 0) {
          pthread_mutex_lock(&mutex);
          printf("Train 2 en approche en sens inverse %s\n" , train2[i%5]);
          sleep(3);
          printf("Le train 2 est arrivé à la gare : %s\n\n", fin_train2);
          printf("\n1 %s %s\n", debut_train2, fin_train2);
          printf("%s %s\n", debut_train3, fin_train3);
          pthread_mutex_unlock(&mutex);
        }
        else if(strcmp(debut_train2, fin_train1) == 0 && strcmp(fin_train2, debut_train1) == 0) {
          pthread_mutex_lock(&mutex);
          printf("Train 2 en approche en sens inverse %s\n" , train2[i%5]);
          sleep(3);
          printf("Le train 2 est arrivé à la gare : %s\n\n", fin_train2);
          pthread_mutex_unlock(&mutex);
        }
        else {
          // Trajet en cours avec 3 secondes de temps de trajet
          temps = clock();
          printf("%f\n", (double) temps);
          printf("Train 2 : %s\n", train2[(i%5)]);
          sleep(3);
          printf("Le train 2 est arrivé à la gare : %s\n\n", fin_train2);
          fflush(stdout);
        }
        i++;
    }
    return NULL;
}

void* TrainC(void* p) {
    int i = 0;
    char train3[5][8] = {"A --> B", "B --> D", "D --> C", "C --> E", "E --> A"};

    while (i >= 0) {
        // Récupère le trajet du train 3 en global :
        substring(0, 1, train3[i%5], debut_train3, sizeof(debut_train3));
        substring(6, 1, train3[i%5], fin_train3, sizeof(fin_train3));

        // Compare avec les autres trains le trajet :
        if(strcmp(debut_train3, fin_train2) == 0 && strcmp(fin_train3, debut_train2) == 0) {
          pthread_mutex_lock(&mutex);
          printf("Train 3 en approche en sens inverse %s\n", train3[(i%5)] );
          sleep(3);
          printf("Le train 3 est arrivé à la gare : %s\n\n", fin_train3);
          printf("\n1 %s %s\n", debut_train2, fin_train2);
          printf("%s %s\n", debut_train3, fin_train3);
          pthread_mutex_unlock(&mutex);
        }
        else if(strcmp(debut_train3, fin_train1) == 0 && strcmp(fin_train3, debut_train1) == 0) {
          pthread_mutex_lock(&mutex);
          printf("Train 3 en approche en sens inverse %s\n", train3[(i%5)]);
          sleep(3);
          printf("Le train 3 est arrivé à la gare : %s\n\n", fin_train3);
          printf("\n1 %s %s\n", debut_train1, fin_train1);
          printf("%s %s\n", debut_train3, fin_train3);
          pthread_mutex_unlock(&mutex);
        } else {
          // Trajet en cours avec 3 secondes de temps de trajet
          temps = clock();
          printf("%f\n", (double) temps);
          printf("Train 3 : %s\n", train3[(i%5)]);
          sleep(3);
          printf("Le train 3 est arrivé à la gare : %s\n\n", fin_train3);
          fflush(stdout);
        }
        i++;
    }
    return NULL;
}

int main() {
    pthread_t ID[3];

    pthread_mutex_init(&mutex, NULL);
    //pthread_mutex_init(&mutex2, NULL);
    //pthread_mutex_init(&mutex3, NULL);

    pthread_create(&ID[0], NULL, TrainB, NULL);
    pthread_create(&ID[1], NULL, TrainC, NULL);

    TrainA(NULL);

    pthread_join(ID[0], 0);
    pthread_join(ID[1], 0);

    pthread_mutex_destroy(&mutex);
    //pthread_mutex_destroy(&mutex2);
    //pthread_mutex_destroy(&mutex3);

    return 0;
}
