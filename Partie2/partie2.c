#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> // pour les semaphores
#include <fcntl.h>     // pour les flags O_CREAT, O_EXCL, ...
#include <unistd.h>    // sleep()
#include <string.h>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */

#define _TEMPS_   3

sem_t * semTrainUn;
sem_t * semTrainDeux;
sem_t * semTrainTrois;

char debut_train1[2];
char fin_train1[2];
char debut_train2[2];
char fin_train2[2];
char debut_train3[2];
char fin_train3[2];

int rentre_train_un     = 0;
int rentre_train_deux   = 0;
int rentre_train_trois  = 0;

clock_t temps;

// pour compiler : gcc partie2.c -lpthread

char *substring(size_t start, size_t stop, const char *src, char *dst, size_t size)
{
   int count = stop - start;
   if ( count >= --size ) {
      count = size;
   }
   sprintf(dst, "%.*s", count, src + start);
   return dst;
}

void* _TrainUn_(void* p) {
    int i = 0;
    char train1[4][8] = {"A --> B", "B --> C", "C --> B", "B --> A"};

    while (i >= 0) {
        // Récupère le trajet du train 1 en global :
        substring(0, 1, train1[i%4], debut_train1, sizeof(debut_train1));
        substring(6, 1, train1[i%4], fin_train1, sizeof(fin_train1));

        // Récupère le temps moyen d'un trajet complet
        if ( strcmp(debut_train1, "B") == 0 && strcmp(fin_train1, "A") == 0 && rentre_train_un == 0) {
            clock_t temps_moyen = clock();
            printf("\nLe temps moyen du train 1 est de : %f secondes\n\n", (double) temps_moyen / 100);
            rentre_train_un = 1;
        }

        // Compare avec les autres trains le trajet :
        if(strcmp(debut_train1, fin_train2) == 0 && strcmp(fin_train1, debut_train2) == 0) {
            sem_wait(semTrainUn);

            // Récupère le temps
            temps = clock();
            printf("%f - ", (double) temps / 100);

            printf("Train 1 en approche en sens inverse %s\n", train1[i%4] );

            // Temps du trajet de 1 à 3 secondes
            sleep(rand()%_TEMPS_);

            printf("\n1 %s %s\n", debut_train1, fin_train1);
            printf("%s %s\n", debut_train2, fin_train2);

            // Récupère le temps
            temps = clock();
            printf("%f - ", (double) temps / 100);   // Et l'affiche

            printf("Le train 1 est arrivé à la gare : %s\n\n", fin_train1);

            sem_post(semTrainDeux);
        }
        else if(strcmp(debut_train1, fin_train3) == 0 && strcmp(fin_train1, debut_train3) == 0) {
            sem_wait(semTrainUn);

            // Récupère le temps
            temps = clock();
            printf("%f - ", (double) temps / 100);   // Et l'affiche

            printf("Train 1 en approche en sens inverse %s\n", train1[i%4] );

            // Temps du trajet de 1 à 3 secondes
            sleep(rand()%_TEMPS_);

            printf("\n2 %s %s\n", debut_train1, fin_train1);
            printf("%s %s\n", debut_train3, fin_train3);

            // Récupère le temps
            temps = clock();
            printf("%f - ", (double) temps / 100);   // Et l'affiche

            printf("Le train 1 est arrivé à la gare : %s\n\n", fin_train1);

            sem_post(semTrainDeux);
        }
        else {
            // Récupère le temps
            temps = clock();
            printf("%f - ", (double) temps / 100);   // Et l'affiche

            printf("Train 1 en approche : %s\n", train1[i%4]);

            // Temps du trajet de 1 à 3 secondes
            sleep(rand()%_TEMPS_);

            // Récupère le temps
            temps = clock();
            printf("%f - ", (double) temps / 100);   // Et l'affiche

            printf("Le train 1 est arrivé à la gare : %s\n\n", fin_train1);

            fflush(stdout);
        }

        i++;
    }
    return NULL;
}

void* _TrainDeux_(void* p) {
    int i = 0;
    char train2[5][8] = {"A --> B", "B --> D", "D --> C", "C --> B", "B --> A"};

    while (i >= 0) {
        // Récupère le trajet du train 2 en global :
        substring(0, 1, train2[i%5], debut_train2, sizeof(debut_train2));
        substring(6, 1, train2[i%5], fin_train2, sizeof(fin_train2));

        // Récupère le temps moyen d'un trajet complet
        if ( strcmp(debut_train2, "B") == 0 && strcmp(fin_train2, "A") == 0 && rentre_train_deux == 0) {
            clock_t temps_moyen = clock();
            printf("\nLe temps moyen du train 2 est de : %f secondes\n\n", (double) temps_moyen / 100);
            rentre_train_deux = 1;
        }

        // Compare avec les autres trains le trajet :
        if(strcmp(debut_train2, fin_train3) == 0 && strcmp(fin_train2, debut_train3) == 0) {
            sem_wait(semTrainDeux);

            // Récupère le temps
            temps = clock();
            printf("%f - ", (double) temps / 100);    // Et l'affiche

            printf("Train 2 en approche en sens inverse %s\n" , train2[i%5]);

            // Temps du trajet de 1 à 3 secondes
            sleep(rand()%_TEMPS_);

            printf("\n3 %s %s\n", debut_train2, fin_train2);
            printf("%s %s\n", debut_train3, fin_train3);

            // Récupère le temps
            temps = clock();
            printf("%f - ", (double) temps / 100);   // Et l'affiche

            printf("Le train 2 est arrivé à la gare : %s\n\n", fin_train2);

            sem_post(semTrainTrois);
        }
        else if(strcmp(debut_train2, fin_train1) == 0 && strcmp(fin_train2, debut_train1) == 0) {
            sem_wait(semTrainDeux);

            temps = clock();
            printf("%f - ", (double) temps / 100);
            printf("Train 2 en approche en sens inverse %s\n" , train2[i%5]);

            // Temps du trajet de 1 à 3 secondes
            sleep(rand()%_TEMPS_);

            printf("\n4 %s %s\n", debut_train1, fin_train1);
            printf("%s %s\n", debut_train2, fin_train2);

            temps = clock();
            printf("%f - ", (double) temps / 100);

            printf("Le train 2 est arrivé à la gare : %s\n\n", fin_train2);

            sem_post(semTrainTrois);
        }
        else {
            temps = clock();
            printf("%f - ", (double) temps / 100);

            printf("train 2 en approche : %s\n", train2[(i%5)]);

            // Temps du trajet de 1 à 3 secondes
            sleep(rand()%_TEMPS_);

            temps = clock();
            printf("%f - ", (double) temps / 100);

            printf("Le train 2 est arrivé à la gare : %s\n\n", fin_train2);

            fflush(stdout);
        }

        i++;
    }
    return NULL;
}

void* _TrainTrois_(void* p) {
    int i = 0;
    char train3[5][8] = {"A --> B", "B --> D", "D --> C", "C --> E", "E --> A"};

    while (i >= 0) {
        // Récupère le trajet du train 3 en global :
        substring(0, 1, train3[i%5], debut_train3, sizeof(debut_train3));
        substring(6, 1, train3[i%5], fin_train3, sizeof(fin_train3));

        // Récupère le temps moyen d'un trajet complet
        if ( strcmp(debut_train3, "E") == 0 && strcmp(fin_train3, "A") == 0 && rentre_train_trois == 0) {
            clock_t temps_moyen = clock();
            printf("\nLe temps moyen du train 3 est de : %f secondes\n\n", (double) temps_moyen / 100);
            rentre_train_trois = 1;
        }

        // Compare avec les autres trains le trajet :
        if(strcmp(debut_train3, fin_train2) == 0 && strcmp(fin_train3, debut_train2) == 0) {
            sem_wait(semTrainTrois);

            temps = clock();
            printf("%f - ", (double) temps / 100);

            printf("Train 3 en approche en sens inverse %s\n", train3[(i%5)] );

            sleep(rand()%_TEMPS_);

            printf("\n5 %s %s\n", debut_train3, fin_train3);
            printf("%s %s\n", debut_train2, fin_train2);

            temps = clock();
            printf("%f - ", (double) temps / 100);

            printf("Le train 3 est arrivé à la gare : %s\n\n", fin_train3);
            sem_post(semTrainUn);
        }
        else if(strcmp(debut_train3, fin_train1) == 0 && strcmp(fin_train3, debut_train1) == 0) {
            sem_wait(semTrainTrois);

            temps = clock();
            printf("%f - ", (double) temps / 100);

            printf("Train 3 en approche en sens inverse %s\n", train3[(i%5)]);

            sleep(rand()%_TEMPS_);

            printf("\n6 %s %s\n", debut_train3, fin_train3);  // debug
            printf("%s %s\n", debut_train1, fin_train1);

            temps = clock();
            printf("%f - ", (double) temps / 100);

            printf("Le train 3 est arrivé à la gare : %s\n\n", fin_train3);
            sem_post(semTrainUn);
        }
        else {
            temps = clock();
            printf("%f - ", (double) temps / 100);

            printf("train 3 en approche : %s\n", train3[(i%5)]);

            sleep(rand()%_TEMPS_);

            temps = clock();
            printf("%f - ", (double) temps / 100);

            printf("Le train 3 est arrivé à la gare : %s\n\n", fin_train3);

            fflush(stdout);
        }

        i++;
    }
    return NULL;
}

int main() {
    pthread_t ID[3];

    srand(time(NULL));

    semTrainUn    = sem_open("semTrainUn", O_CREAT, S_IRUSR | S_IWUSR, 3);
    semTrainDeux  = sem_open("semTrainDeux", O_CREAT, S_IRUSR | S_IWUSR, 0);
    semTrainTrois = sem_open("semTrainTrois", O_CREAT, S_IRUSR | S_IWUSR, 0);

    pthread_create(&ID[0], NULL, _TrainDeux_, NULL);
    pthread_create(&ID[1], NULL, _TrainTrois_, NULL);

    _TrainUn_(NULL);

    pthread_join(ID[0], 0);
    pthread_join(ID[1], 0);

    sem_close(semTrainUn);
    sem_close(semTrainDeux);
    sem_close(semTrainTrois);
    sem_unlink("semTrainUn");
    sem_unlink("semTrainDeux");
    sem_unlink("semTrainTrois");

    return 0;
}
