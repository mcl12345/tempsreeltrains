Nous avons créer 3 threads, pour chaque trains ( TrainUn, TrainDeux, TrainTrois )

Partie 1 - mutex :
Pour chaque train deux mutex bloquants pour les 2 autres threads / trains en fonctionnement.
Pour éviter que 2 trains ne se rencontrent sur la même voie.

Partie 2 - sémaphore :
Trois sémaphores bloquant pour empêcher d'avoir un train qui arrive en sens inverse.

Partie 3 - Verrous
Trois vérrous bloquant pour empêcher d'avoir un train qui arrive en sens inverse.

Makefile

ou

gcc partie1.c -lpthread

./a.out
