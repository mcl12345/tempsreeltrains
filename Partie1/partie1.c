#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> // pour les semaphores
#include <fcntl.h>     // pour les flags O_CREAT, O_EXCL, ...
#include <unistd.h>    // sleep()
#include <string.h>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */

#define _TEMPS_ 3

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;

char debut_train1[2];
char fin_train1[2];
char debut_train2[2];
char fin_train2[2];
char debut_train3[2];
char fin_train3[2];

clock_t temps;

// Permet de ne demander qu'une seule fois le calcul de la moyenne
int rentre_train_un     = 0;
int rentre_train_deux   = 0;
int rentre_train_trois  = 0;

// Récupère le caractère du début et de la fin de la chaine de caractere (ex: A --> B , donne A et B )
char *substring(size_t start, size_t stop, const char *src, char *dst, size_t size)
{
   int count = stop - start;
   if ( count >= --size ) {
      count = size;
   }
   sprintf(dst, "%.*s", count, src + start);
   return dst;
}

// Thread train 1
void* TrainUn(void* p) {
    int i = 0;
    char train1[4][8] = {"A --> B", "B --> C", "C --> B", "B --> A"};

    while (i >= 0) {
        // Récupère le trajet du train 1 en global dans debut_train1 et dans fin_train1 :
        substring(0, 1, train1[i%4], debut_train1, sizeof(debut_train1));
        substring(6, 1, train1[i%4], fin_train1, sizeof(fin_train1));

        // Récupère le temps moyen d'un trajet complet
        if ( strcmp(debut_train1, "B") == 0 && strcmp(fin_train1, "A") == 0) {
            clock_t temps_moyen = clock();
            if (rentre_train_un == 0) {
              printf("\nLe temps moyen du trajet du train 1 est de : %f secondes\n\n", (double) temps_moyen / 100);
            }
            else if (rentre_train_un == 1) {
                temps_moyen = clock();
                printf("\nLe temps moyen du trajet du train 1 est de : %f secondes\n\n", (double) temps_moyen / 200);
            }
            else if (rentre_train_un == 2) {
                temps_moyen = clock();
                printf("\nLe temps moyen du trajet du train 1 est de : %f secondes\n\n", (double) temps_moyen / 300);
            }
            rentre_train_un = rentre_train_un + 1;
        }

        // Compare avec les autres trains 2 et 3 le trajet du train 1 :
        if(strcmp(debut_train1, fin_train2) == 0 && strcmp(fin_train1, debut_train2) == 0) {
            pthread_mutex_lock(&mutex3);
            pthread_mutex_lock(&mutex2);

            // Récupère le temps
            temps = clock();
            printf("%f - ", (double) temps / 100);  // et l'affiche

            printf("Train 1 en approche en sens inverse %s\n", train1[i%4] );

            // Temps du trajet de 1 à 3 secondes
            sleep(rand()%_TEMPS_);

            // Récupère le temps
            temps = clock();
            printf("%f - ", (double) temps / 100);  // et l'affiche

            printf("Le train 1 est arrivé à la gare : %s\n\n", fin_train1);
            pthread_mutex_unlock(&mutex2);
            pthread_mutex_unlock(&mutex3);
        }
        else if(strcmp(debut_train1, fin_train3) == 0 && strcmp(fin_train1, debut_train3) == 0) {
          pthread_mutex_lock(&mutex3);
          pthread_mutex_lock(&mutex2);

          // Récupère le temps
          temps = clock();
          printf("%f - ", (double) temps / 100);  // et l'affiche

          printf("Train 1 en approche en sens inverse %s\n", train1[i%4] );

          // Temps du trajet de 1 à 3 secondes
          sleep(rand()%_TEMPS_);

          // Récupère le temps
          temps = clock();
          printf("%f - ", (double) temps / 100);  // et l'affiche

          printf("Le train 1 est arrivé à la gare : %s\n\n", fin_train1);

          pthread_mutex_unlock(&mutex2);
          pthread_mutex_unlock(&mutex3);
        }
        else {
            // Trajet en cours de 1 à 3 secondes de temps de trajet
            pthread_mutex_lock(&mutex1);

            // Récupère le temps
            temps = clock();
            printf("%f - ", (double) temps / 100);  // et l'affiche

            printf("Train 1 : %s\n", train1[i%4]);

            // Temps du trajet de 1 à 3 secondes
            sleep(rand()%_TEMPS_);

            // Récupère le temps
            temps = clock();
            printf("%f - ", (double) temps / 100);  // et l'affiche

            printf("Le train 1 est arrivé à la gare : %s\n\n", fin_train1);

            fflush(stdout);

            pthread_mutex_unlock(&mutex1);
        }
        i++;
    }
    return NULL;
}

// Thread train 2
void* TrainDeux(void* p) {
    int i = 0;
    char train2[5][8] = {"A --> B", "B --> D", "D --> C", "C --> B", "B --> A"};

    while (i >= 0) {
      // Récupère le trajet du train 2 en global dans debut_train2 et dans fin_train2 :
        substring(0, 1, train2[i%5], debut_train2, sizeof(debut_train2));
        substring(6, 1, train2[i%5], fin_train2, sizeof(fin_train2));

        // Récupère le temps moyen d'un trajet complet
        if ( strcmp(debut_train2, "B") == 0 && strcmp(fin_train2, "A") == 0) {
            clock_t temps_moyen = clock();
            if (rentre_train_deux == 0) {
              printf("\nLe temps moyen du trajet du train 2 est de : %f secondes\n\n", (double) temps_moyen / 100);
            }
            else if (rentre_train_deux == 1) {
                temps_moyen = clock();
                printf("\nLe temps moyen du trajet du train 2 est de : %f secondes\n\n", (double) temps_moyen / 200);
            }
            else if (rentre_train_deux == 2) {
                temps_moyen = clock();
                printf("\nLe temps moyen du trajet du train 2 est de : %f secondes\n\n", (double) temps_moyen / 300);
            }
            rentre_train_un = rentre_train_un + 1;
        }

        // Compare avec les autres trains le trajet :
        if(strcmp(debut_train2, fin_train3) == 0 && strcmp(fin_train2, debut_train3) == 0) {
          pthread_mutex_lock(&mutex1);
          pthread_mutex_lock(&mutex3);

          // Récupère le temps
          temps = clock();
          printf("%f - ", (double) temps / 100);    // et l'affiche

          printf("Train 2 en approche en sens inverse %s\n" , train2[i%5]);

          // Temps du trajet de 1 à 3 secondes
          sleep(rand()%_TEMPS_);

          // Récupère le temps
          temps = clock();
          printf("%f - ", (double) temps / 100);    // et l'affiche

          printf("Le train 2 est arrivé à la gare : %s\n\n", fin_train2);

          pthread_mutex_unlock(&mutex1);
          pthread_mutex_unlock(&mutex3);
        }
        else if(strcmp(debut_train2, fin_train1) == 0 && strcmp(fin_train2, debut_train1) == 0) {
          pthread_mutex_lock(&mutex1);
          pthread_mutex_lock(&mutex3);

          // Récupère le temps
          temps = clock();
          printf("%f - ", (double) temps / 100);    // et l'affiche

          printf("Train 2 en approche en sens inverse %s\n" , train2[i%5]);

          // Temps du trajet de 1 à 3 secondes
          sleep(rand()%_TEMPS_);

          // Récupère le temps
          temps = clock();
          printf("%f - ", (double) temps / 100);    // et l'affiche

          printf("Le train 2 est arrivé à la gare : %s\n\n", fin_train2);

          pthread_mutex_unlock(&mutex1);
          pthread_mutex_unlock(&mutex3);
        }
        else {
          // Trajet en cours avec 3 secondes de temps de trajet
          pthread_mutex_lock(&mutex2);

          // Récupère le temps
          temps = clock();
          printf("%f - ", (double) temps / 100);    // et l'affiche

          printf("Train 2 : %s\n", train2[(i%5)]);

          // Temps du trajet de 1 à 3 secondes
          sleep(rand()%_TEMPS_);

          // Récupère le temps
          temps = clock();
          printf("%f - ", (double) temps / 100);    // et l'affiche

          printf("Le train 2 est arrivé à la gare : %s\n\n", fin_train2);

          fflush(stdout);

          pthread_mutex_unlock(&mutex2);
        }
        i++;
    }
    return NULL;
}

// Thread train 3
void* TrainTrois(void* p) {
    int i = 0;
    char train3[5][8] = {"A --> B", "B --> D", "D --> C", "C --> E", "E --> A"};

    while (i >= 0) {
        // Récupère le trajet du train 3 en global dans debut_train3 et dans fin_train3 :
        substring(0, 1, train3[i%5], debut_train3, sizeof(debut_train3));
        substring(6, 1, train3[i%5], fin_train3, sizeof(fin_train3));

        // Récupère le temps moyen d'un trajet complet
        if ( strcmp(debut_train3, "E") == 0 && strcmp(fin_train3, "A") == 0) {
            clock_t temps_moyen = clock();
            if (rentre_train_trois == 0) {
              printf("\nLe temps moyen du trajet du train 3 est de : %f secondes\n\n", (double) temps_moyen / 100);
            }
            else if (rentre_train_trois == 1) {
                temps_moyen = clock();
                printf("\nLe temps moyen du trajet du train 3 est de : %f secondes\n\n", (double) temps_moyen / 200);
            }
            else if (rentre_train_trois == 2) {
                temps_moyen = clock();
                printf("\nLe temps moyen du trajet du train 3 est de : %f secondes\n\n", (double) temps_moyen / 300);
            }
            rentre_train_un = rentre_train_un + 1;
        }

        // Compare avec les autres trains le trajet :
        if(strcmp(debut_train3, fin_train2) == 0 && strcmp(fin_train3, debut_train2) == 0) {
          pthread_mutex_lock(&mutex2);
          pthread_mutex_lock(&mutex1);

          // Récupère le temps
          temps = clock();
          printf("%f - ", (double) temps / 100);    // et l'affiche

          printf("Train 3 en approche en sens inverse %s\n", train3[(i%5)] );

          // Temps du trajet de 1 à 3 secondes
          sleep(rand()%_TEMPS_);

          // Récupère le temps
          temps = clock();
          printf("%f - ", (double) temps / 100);    // et l'affiche

          printf("Le train 3 est arrivé à la gare : %s\n\n", fin_train3);

          pthread_mutex_unlock(&mutex1);
          pthread_mutex_unlock(&mutex2);
        }
        else if(strcmp(debut_train3, fin_train1) == 0 && strcmp(fin_train3, debut_train1) == 0) {
          pthread_mutex_lock(&mutex2);
          pthread_mutex_lock(&mutex1);

          // Récupère le temps
          temps = clock();
          printf("%f - ", (double) temps / 100);   // Et l'affiche

          printf("Train 3 en approche en sens inverse %s\n", train3[(i%5)]);

          // Temps du trajet de 1 à 3 secondes
          sleep(rand()%_TEMPS_);

          // Récupère le temps
          temps = clock();
          printf("%f - ", (double) temps / 100);    // Et l'affiche

          printf("Le train 3 est arrivé à la gare : %s\n\n", fin_train3);

          pthread_mutex_unlock(&mutex1);
          pthread_mutex_unlock(&mutex2);
        }
        else {
          // Trajet en cours avec 3 secondes de temps de trajet
          pthread_mutex_lock(&mutex3);

          // Récupère le temps
          temps = clock();
          printf("%f - ", (double) temps / 100);    // et l'affiche

          printf("Train 3 : %s\n", train3[(i%5)]);

          // Temps du trajet de 1 à 3 secondes
          sleep(rand()%_TEMPS_);

          // Récupère le temps
          temps = clock();
          printf("%f - ", (double) temps / 100);     // et l'affiche

          fflush(stdout);

          printf("Le train 3 est arrivé à la gare : %s\n\n", fin_train3);

          pthread_mutex_unlock(&mutex3);
        }
        i++;
    }
    return NULL;
}

int main() {
    pthread_t ID[3];

    srand(time(NULL));

    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_mutex_init(&mutex3, NULL);

    pthread_create(&ID[0], NULL, TrainDeux, NULL);
    pthread_create(&ID[1], NULL, TrainTrois, NULL);

    TrainUn(NULL);

    pthread_join(ID[0], 0);
    pthread_join(ID[1], 0);

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    pthread_mutex_destroy(&mutex3);

    return 0;
}
