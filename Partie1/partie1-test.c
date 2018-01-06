#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> // pour les semaphores
#include <fcntl.h>     // pour les flags O_CREAT, O_EXCL, ...
#include <unistd.h>    // sleep()
#include <string.h>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */

#define _TEMPS_ 3

/*création des mutex*/
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fifo   = PTHREAD_MUTEX_INITIALIZER;

clock_t temps; // Le temps
int nb_train1 = 0; //declarer un nombre de train1 dans le voie
int nb_train2 = 0; //declarer un nombre de train2 dans le voie
int nb_train3 = 0; //declarer un nombre de train3 dans le voie

//declarer des elements de debut_train, fin_train, et on va etre un debut_train,fin_train
char debut_train1[2], possible_debut_train1[2];
char fin_train1[2],   possible_fin_train1[2];
char debut_train2[2], possible_debut_train2[2];
char fin_train2[2],   possible_fin_train2[2];
char debut_train3[2], possible_debut_train3[2];
char fin_train3[2],   possible_fin_train3[2];

// Permet de limiter le calcul de la moyenne à 3 fois
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

int train_en_inverse(char *possible_debut1,char* possible_fin1,char* possible_debut2, char* possible_fin2 ){
    if(!strcmp(possible_debut1,possible_fin2) && !strcmp(possible_fin1,possible_debut2)){
        return 1;
    }
    return 0;
}

int train_en_ligne(char *possible_debut1,char* possible_fin1,char* possible_debut2, char* possible_fin2){
    if(!strcmp(possible_debut1,possible_debut2) && !strcmp(possible_fin1,possible_fin2)){
        return 1;
    }
    return 0;
}

//Train 1
void* TrainUn(void *p){
    int i = 0;
    char train1[4][8] = {"A --> B", "B --> C", "C --> B", "B --> A"};

    while (i >= 0 ){
        // Récupère le trajet du train 1 en global dans debut_train1 et dans fin_train1 :
        substring(0, 1, train1[i%4], possible_debut_train1, sizeof(possible_debut_train1));
        substring(6, 1, train1[i%4], possible_fin_train1, sizeof(possible_fin_train1));

        // Récupère le temps moyen d'un trajet complet
        if ( strcmp(debut_train1, "B") == 0 && strcmp(fin_train1, "A") == 0) {
            clock_t temps_moyen = clock();
            if (rentre_train_un == 0) {
              printf("\nLe temps moyen du trajet du train 1 est de : %f secondes\n\n", (double) temps_moyen / 100- 6);
            }
            else if (rentre_train_un == 1) {
                temps_moyen = clock();
                printf("\nLe temps moyen du trajet du train 1 est de : %f secondes\n\n", (double) (temps_moyen / 100 - 6) / 2);
            }
            else if (rentre_train_un == 2) {
                temps_moyen = clock();
                printf("\nLe temps moyen du trajet du train 1 est de : %f secondes\n\n", (double) (temps_moyen / 100 - 6) / 3);
            }
            rentre_train_un = rentre_train_un + 1;
        }

        printf("Le train1 doit faire le trajet: %s\n", train1[i%4]);
        pthread_mutex_lock(&fifo);
        pthread_mutex_lock(&mutex1);
        nb_train1++;
        if (nb_train1 == 1){
            // Le premier trajet A --> B
            if(i == 0){
                pthread_mutex_lock(&mutex2);
                pthread_mutex_lock(&mutex3);
            }
            else {
                // La direction est inverse ou le meme trajet
                if (train_en_inverse(possible_debut_train1,possible_fin_train1,possible_debut_train2,possible_fin_train2)||
                    train_en_inverse(debut_train1,fin_train1,possible_debut_train2,possible_fin_train2)||
                    train_en_ligne(possible_debut_train1,possible_fin_train1,possible_debut_train2,possible_fin_train2) ){
                    pthread_mutex_lock(&mutex2);
                }
                if (train_en_inverse(possible_debut_train1,possible_fin_train1,possible_debut_train3,possible_fin_train3)||
                    train_en_inverse(debut_train1,fin_train1,possible_debut_train3,possible_fin_train3)||
                    train_en_ligne(possible_debut_train1,possible_fin_train1,possible_debut_train3,possible_fin_train3) ){
                    pthread_mutex_lock(&mutex3);
                }
            }

        }
        pthread_mutex_unlock(&mutex1);
        pthread_mutex_unlock(&fifo);
        strcpy(debut_train1,possible_debut_train1);

        // Récupère le temps
        temps = clock();
        printf("%f - ", (double) temps / 100 - 6);    // et l'affiche

        printf("Le train1 fait le trajet: %s\nTrain1 est parti de la gare: %s\n\n",train1[i%4],debut_train1);
       // printf("Train1 va le trajet: %s\n",train1[i%4]);
        sleep(rand() % 2); // on simule un traitement long
        pthread_mutex_lock(&mutex1);
        //strcpy(debut_train1,possible_debut_train1);
        //printf("Train1 est parti de la gare: %s\n\n",debut_train1);
        strcpy(fin_train1, possible_fin_train1);

        // Récupère le temps
        temps = clock();
        printf("%f - ", (double) temps / 100 - 6);    // et l'affiche

        printf("Le train1 a fini le trajet : %s\nLe train1 est arrivé à la gare:%s\n\n",train1[i%4],fin_train1);
        nb_train1--;

        if(nb_train1 == 0){
            //Le premier trajet A->B
            if(i == 0) {
                pthread_mutex_unlock(&mutex2);
                pthread_mutex_unlock(&mutex3);
            }
            else {
                // La direction n'est inverse ou le meme trajet
                if (!train_en_inverse(debut_train1,fin_train1,possible_debut_train2,possible_fin_train2)) {
                    pthread_mutex_unlock(&mutex2);
                }
                if (!train_en_inverse(debut_train1,fin_train1,possible_debut_train3,possible_fin_train3) ){
                    pthread_mutex_unlock(&mutex3);
                }
            }

        }
        pthread_mutex_unlock(&mutex1);
        sleep(rand() % 3); // Le train1 fait une pause
        i++;
    }
    return NULL;
}

//Train 2
void* TrainDeux(void *p){
    int i = 0;
    char train2[5][8] = {"A --> B", "B --> D", "D --> C", "C --> B", "B --> A"};

    while (i >= 0 ){
        // Récupère le trajet du train 1 en global dans debut_train1 et dans fin_train1 :
        substring(0, 1, train2[i%5], possible_debut_train2, sizeof(possible_debut_train2));
        substring(6, 1, train2[i%5], possible_fin_train2, sizeof(possible_fin_train2));

        // Récupère le temps moyen d'un trajet complet
        if ( strcmp(debut_train2, "B") == 0 && strcmp(fin_train2, "A") == 0) {
            clock_t temps_moyen = clock();
            if (rentre_train_deux == 0) {
              printf("\nLe temps moyen du trajet du train 2 est de : %f secondes\n\n", (double) temps_moyen / 100- 6);
            }
            else if (rentre_train_deux == 1) {
                temps_moyen = clock();
                printf("\nLe temps moyen du trajet du train 2 est de : %f secondes\n\n", (double) (temps_moyen / 100 - 6) / 2);
            }
            else if (rentre_train_deux == 2) {
                temps_moyen = clock();
                printf("\nLe temps moyen du trajet du train 2 est de : %f secondes\n\n", (double) (temps_moyen / 100 - 6) / 3);
            }
            rentre_train_deux = rentre_train_deux + 1;
        }

        printf("Le train2 doit faire le trajet: %s\n", train2[i%5]);
        pthread_mutex_lock(&fifo);
        pthread_mutex_lock(&mutex2);
        nb_train2++;
        if (nb_train2 == 1){
            //Le premier trajet A->B
            if(i == 0) {
                pthread_mutex_lock(&mutex1);
                pthread_mutex_lock(&mutex3);
            }
            else {
                //Direction inverse ou la meme trajet
                if (train_en_inverse(possible_debut_train2,possible_fin_train2,possible_debut_train1,possible_fin_train1)||
                    train_en_inverse(debut_train2,fin_train2,possible_debut_train1,possible_fin_train1)||
                    train_en_ligne(possible_debut_train2,possible_fin_train2,possible_debut_train1,possible_fin_train1) ){
                      pthread_mutex_lock(&mutex1);
                }
                if (train_en_inverse(possible_debut_train2,possible_fin_train2,possible_debut_train3,possible_fin_train3)||
                    train_en_inverse(debut_train2,fin_train2,possible_debut_train3,possible_fin_train3)||
                    train_en_ligne(possible_debut_train2,possible_fin_train2,possible_debut_train3,possible_fin_train3) ){
                      pthread_mutex_lock(&mutex3);
                }
            }

        }

        pthread_mutex_unlock(&mutex2);
        pthread_mutex_unlock(&fifo);

        strcpy(debut_train2, possible_debut_train2);

        // Récupère le temps
        temps = clock();
        printf("%f - ", (double) temps / 100 - 6);    // et l'affiche

        printf("Le train2 fait le trajet: %s\nLe train2 est parti de la gare: %s\n\n",train2[i%5],debut_train2);
        //printf("Train2 va le trajet: %s\n",train2[2%5]);
        sleep(rand() % 2); // on simule un traitement long
        pthread_mutex_lock(&mutex2);
        //strcpy(debut_train2,possible_debut_train2);
       // printf("Train2 est parti de la gare: %s\n\n",debut_train2);
        strcpy(fin_train2,possible_fin_train2);

        // Récupère le temps
        temps = clock();
        printf("%f - ", (double) temps / 100 - 6);    // et l'affiche

        printf("Le train2 a fini le trajet : %s\nLe train2 est arrivé à la gare:%s\n\n",train2[i%5],fin_train2);
        nb_train2--;

        if(nb_train2 == 0) {
            // Le premier trajet A --> B
            if(i == 0) {
                pthread_mutex_unlock(&mutex1);
                pthread_mutex_unlock(&mutex3);
            }
            else {
                // La direction n'est inverse ou le meme trajet
                if (!train_en_inverse(debut_train2,fin_train2,possible_debut_train1,possible_fin_train1) ){
                    pthread_mutex_unlock(&mutex1);
                }
                if (!train_en_inverse(debut_train2,fin_train2,possible_debut_train3,possible_fin_train3) ){
                    pthread_mutex_unlock(&mutex3);
                }
            }

        }

        pthread_mutex_unlock(&mutex2);
        sleep(rand() % 3); // Le train1 fait une pause
        i++;
    }
    return NULL;
}

// thread Train 3
void* TrainTrois(void *p){
    int i = 0;
    char train3[5][8] = {"A --> B", "B --> D", "D --> C", "C --> E", "E --> A"};

    while (i >= 0 ) {
        // Récupère le trajet du train 1 en global dans debut_train1 et dans fin_train1 :
        substring(0, 1, train3[i%5], possible_debut_train3, sizeof(possible_debut_train3));
        substring(6, 1, train3[i%5], possible_fin_train3, sizeof(possible_fin_train3));

        // Récupère le temps moyen d'un trajet complet
        if ( strcmp(debut_train3, "E") == 0 && strcmp(fin_train3, "A") == 0) {
            clock_t temps_moyen = clock();
            if (rentre_train_trois == 0) {
              printf("\nLe temps moyen du trajet du train 3 est de : %f secondes\n\n", (double) temps_moyen / 100- 6);
            }
            else if (rentre_train_trois == 1) {
                temps_moyen = clock();
                printf("\nLe temps moyen du trajet du train 3 est de : %f secondes\n\n", (double) (temps_moyen / 100 - 6) / 2);
            }
            else if (rentre_train_trois == 2) {
                temps_moyen = clock();
                printf("\nLe temps moyen du trajet du train 3 est de : %f secondes\n\n", (double) (temps_moyen / 100 - 6) / 3);
            }
            rentre_train_trois = rentre_train_trois + 1;
        }

        printf("Le train 3 doit faire le trajet: %s\n", train3[i%5]);
        pthread_mutex_lock(&fifo);
        pthread_mutex_lock(&mutex3);
        nb_train3++;
        // condition i
        if (nb_train3 == 1){
            // Le premier trajet A --> B
            if(i == 0){
                pthread_mutex_lock(&mutex1);
                pthread_mutex_lock(&mutex2);
            }
            else {
                // La direction inverse ou le meme trajet
                if (train_en_inverse(possible_debut_train3,possible_fin_train3,possible_debut_train1,possible_fin_train1)||
                    train_en_inverse(debut_train3,fin_train3,possible_debut_train1,possible_fin_train1)||
                    train_en_ligne(possible_debut_train3,possible_fin_train3,possible_debut_train1,possible_fin_train1) ){
                      pthread_mutex_lock(&mutex1);
                }
                if (train_en_inverse(possible_debut_train3,possible_fin_train3,possible_debut_train2,possible_fin_train2)||
                    train_en_inverse(debut_train3,fin_train3,possible_debut_train2,possible_fin_train2)||
                    train_en_ligne(possible_debut_train3,possible_fin_train3,possible_debut_train2,possible_fin_train2) ){
                      pthread_mutex_lock(&mutex2);
                }
            }

        }

        pthread_mutex_unlock(&mutex3);
        pthread_mutex_unlock(&fifo);

        strcpy(debut_train3, possible_debut_train3);

        // Récupère le temps
        temps = clock();
        printf("%f - ", (double) temps / 100 - 6);    // et l'affiche

        printf("Le train3 fait le trajet: %s\nLe train3 est parti de la gare: %s\n\n", train3[i%5], debut_train3);
        //printf("Train3 va le trajet: %s\n",train3[i%5]);
        sleep(rand() % 2); // on simule un traitement long
        pthread_mutex_lock(&mutex3);
        //printf("Train3 est parti de la gare: %s\n\n",debut_train3);
        //strcpy(debut_train3,possible_debut_train3);
        strcpy(fin_train3,possible_fin_train3);

        // Récupère le temps
        temps = clock();
        printf("%f - ", (double) temps / 100 - 6);    // et l'affiche

        printf("Le train3 a fini le trajet : %s\nLe train3 est arrivé à la gare:%s\n\n", train3[i%5], fin_train3);
        nb_train3--;

        if(nb_train3 == 0) {
            // Le premier trajet A --> B
            if(i == 0){
                pthread_mutex_unlock(&mutex1);
                pthread_mutex_unlock(&mutex2);
            }
            else {
                // La direction n'est inverse ou le meme trajet
                if (!train_en_inverse(debut_train3, fin_train3, possible_debut_train1, possible_fin_train1) ){
                    pthread_mutex_unlock(&mutex1);
                }
                if (!train_en_inverse(debut_train3, fin_train3, possible_debut_train2, possible_fin_train2) ){
                    pthread_mutex_unlock(&mutex2);
                }
            }

        }
        pthread_mutex_unlock(&mutex3);
        sleep(rand() % 3); //Train1 fait une pause
        i++;

    }

    return NULL;
}

int main(){
    int i;
    srand(time(NULL)); // initialisation de rand
    pthread_t tid[3];

    pthread_create(&tid[0], NULL, TrainUn,NULL);
    pthread_create(&tid[1], NULL, TrainDeux,NULL);
    pthread_create(&tid[2], NULL, TrainTrois,NULL);


    for (i = 0; i < 3; i++) {
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    pthread_mutex_destroy(&mutex3);
    pthread_mutex_destroy(&fifo);

    return 0;
}
