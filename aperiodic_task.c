/*----------------------------------------------------------------------------*
 * fichier : aperiodic_task.c                                                 *
 * programme de création des taches apériodiques                              *
 *----------------------------------------------------------------------------*/

#include "aperiodic_task.h"
#include "noyau_prio.h"

static TACHE_APERIODIQUE file[MAX_TACHES_APERIODIQUES];
static TACHE_APERIODIQUE_FILE tache_aperiodique_file = {.file = file};

// Fonction C de tache apériodique de calcul d'une somme de 2 entiers
void tache_aperiodiqueA(TACHE_APERIODIQUE_PARAMS* params) {
    printf("%d", params->nb_args);
    if (params->nb_args != 2) {
        printf("Erreur: 2 arguments attendus, seulement %d obtenus\n", params->nb_args);
        return;
    }
    int arg1 = *(int*)(params->args[0]);
    int arg2 = *(int*)(params->args[1]);
    int sum = arg1 + arg2;
    printf("[TACHE APERIODIQUE A] %d + %d = %d\n", arg1, arg2, sum);
    return;
}

// Fonction C de tache apériodique d'affichage d'une chaine de caractère
void tache_aperiodiqueB(TACHE_APERIODIQUE_PARAMS* params) {
    if (params->nb_args != 1) {
        printf("Erreur: 1 argument attendu, seulement %d obtenu\n", params->nb_args);
        return;
    }
    char* arg1 = (char *)(params->args[0]);
    printf("%s\n", arg1);
    return;
}

// Fonction C de tache apériodique de calcul d'un AND de 2 booléens
void tache_aperiodiqueC(TACHE_APERIODIQUE_PARAMS* params) {
    if (params->nb_args != 2) {
        printf("Erreur: 2 arguments attendus, seulement %d obtenus\n", params->nb_args);
        return;
    }
    int arg1 = *(int*)(params->args[0]);
    int arg2 = *(int*)(params->args[1]);
    printf("%d\n", arg1 && arg2);
    return;
}

// Fonction C de tache apériodique de calcul d'un AND de 2 booléens
void tache_aperiodiqueD(TACHE_APERIODIQUE_PARAMS* params) {
    if (params->nb_args != 0) {
        printf("Erreur: 0 arguments attendus, %d obtenus\n", params->nb_args);
        return;
    }

    for(;;) {printf("-");}

    return;
}

// Primitives de gestion du serveur de tâches apériodiques
void aperiodique_file_init(void) {
    tache_aperiodique_file.nb_tache = 0;
    tache_aperiodique_file.queue = 0;
    tache_aperiodique_file.tete = -1;
}

int aperiodique_file_is_empty(void) {
    return tache_aperiodique_file.nb_tache == 0;
}

int aperodique_file_is_full(void) {
    return tache_aperiodique_file.nb_tache == MAX_TACHES_APERIODIQUES;
}

void aperodique_file_active_suivant(void) {
    if (aperiodique_file_is_empty()) {return;}
    int idx = tache_aperiodique_file.queue;
    tache_aperiodique_file.file[idx].fonction(tache_aperiodique_file.file[idx].params);  // Lancer la prochaine tâche

    tache_aperiodique_file.queue = (tache_aperiodique_file.queue + 1) % MAX_TACHES_APERIODIQUES ;  // On actualise l'index de la tâche apériodique courante dans la file
    tache_aperiodique_file.nb_tache -- ;  // La tâche a été retournée, on baisse donc le compteur de tâche à traiter
    if (aperiodique_file_is_empty()) {printf("Dernière tâche apériodique effectuée, attente active...\n");}
}

int aperodique_file_ajoute(TACHE_APERIODIQUE tache) {
    if (aperodique_file_is_full()) {
        printf("Le serveur de tâche apériodique est plein\n");
        return -1;
    }
    tache_aperiodique_file.tete = (tache_aperiodique_file.tete + 1) % MAX_TACHES_APERIODIQUES;
    tache_aperiodique_file.file[tache_aperiodique_file.tete] = tache;
    tache_aperiodique_file.nb_tache++;
    return 0;
}

// Primitive de création d'une tâche apériodique
void cree_tache_aperiodique(void (*fonction)(void *), TACHE_APERIODIQUE_PARAMS *params) {
    _lock_();
    if (aperodique_file_is_full()) {
        printf("[Erreur] Nombre maximal de tache aperiodique atteinte.\n");
        _unlock_();
        return;
    }

    // Création de la tache apériodique
    TACHE_APERIODIQUE tache;
    tache.fonction = fonction;
    tache.params = params;

    // Ajout de la tache apériodique dans le serveur de taches apériodiques (file)
    if (aperodique_file_ajoute(tache) == -1) {
        printf("[Erreur] L'insertion de la tâche dans le serveur a échoué.\n");
    };
    _unlock_();
}
