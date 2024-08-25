/*----------------------------------------------------------------------------*
 * fichier : aperiodic_task.h                                                 *
 * gestion de des taches apériodiques                                         *
 * ce fichier declare toutes les primitives de base                           *
 *----------------------------------------------------------------------------*/

#ifndef __APERIODIC_TASK_H__
#define __APERIODIC_TASK_H__

#include <stdint.h>

#define MAX_TACHES_APERIODIQUES 8

/* Définition de la structure d'une tache apériodique */
typedef struct {
    void **args;
    int nb_args;
} TACHE_APERIODIQUE_PARAMS;

/* Définition de la structure des paramètres tache apériodique */
typedef struct {
  void (*fonction)(void *);
  void  *params;
} TACHE_APERIODIQUE;

typedef struct {
  TACHE_APERIODIQUE file[MAX_TACHES_APERIODIQUES];
  int tete;
  int queue;
  int nb_tache;
} TACHE_APERIODIQUE_FILE;

// Primitives de création de tâche apériodique
void cree_tache_aperiodique(void (*fonction)(void *), TACHE_APERIODIQUE_PARAMS *params);

// Primitives de gestion du serveur de tâches apériodiques (file)
void aperiodique_file_init(void);
int aperiodique_file_is_empty(void);
int aperodique_file_is_full(void);
void aperodique_file_active_suivant(void);
int aperodique_file_ajoute(TACHE_APERIODIQUE tache);

// Fonction d'exemple de tâche apériodique
void tache_aperiodiqueA(TACHE_APERIODIQUE_PARAMS* params);
void tache_aperiodiqueB(TACHE_APERIODIQUE_PARAMS* params);
void tache_aperiodiqueC(TACHE_APERIODIQUE_PARAMS* params);
void tache_aperiodiqueD(TACHE_APERIODIQUE_PARAMS* params);
#endif
