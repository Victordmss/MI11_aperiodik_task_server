/*----------------------------------------------------------------------------*
 * fichier : noyau_test.c                                                     *
 * programme de test du noyaut                                                *
 *----------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdlib.h>

#include "stm32h7xx.h"
#include "serialio.h"
#include "noyau_prio.h"
#include "noyau_file_prio.h"
#include "delay.h"
#include "TERMINAL.h"
#include "aperiodic_task.h"

TACHE	tachedefond(void);
TACHE	tacheGen(void);

#define MAX_CARA_LIGNE 80

/*
 * structure du contexte d'une tache
 */
typedef struct {
    // adresse de debut de la tache
    uint16_t Nb_tour;
    // etat courant de la tache
    uint16_t wait_time;
} NOYAU_TCB_ADD;

/*----------------------------------------------------------------------------*
 * declaration des variables du noyau comme extern pour pouvoir les
 * utiliser dans d'autres partie du code
 * *--------------------------------------------------------------------------*/
#define POS_CHRONO 10
/*
 * tableau stockant le contexte de chaque tache
 */
NOYAU_TCB_ADD _noyau_tcb_add[MAX_TACHES_NOYAU];

uint16_t pos_x = 1;
uint16_t pos_y = 10;

TACHE tachedefond(void) {
	printf("---> Creation du serveur de tâche apériodique \n");
	// 1. Creation du serveur de taches apériodiques
	aperiodique_file_init();

	// 2. Instanciation des tâches
	// 2.1 Tache A
	int a = 1, b = 2;
    void *args[] = {&a, &b};
    TACHE_APERIODIQUE_PARAMS paramsA = {args, 2};

	int c = 8, d = 8;
    void *args2[] = {&c, &d};
    TACHE_APERIODIQUE_PARAMS paramsA2 = {args2, 2};

	int e = 10, f = 2;
    void *args3[] = {&e, &f};
    TACHE_APERIODIQUE_PARAMS paramsA3 = {args3, 2};

	// 2.2 Tache D
    void *argsD[] = {NULL};
	TACHE_APERIODIQUE_PARAMS paramsD = {argsD, 0};

	// 3. Création des taches

	// 3.1 Test pour trop de tâche > 8
	/*cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA);
    cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA);
	cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA);
	cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA);
	cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA);
	cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA);
	cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA);
	cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA);
	cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA);
	cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA);
	cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA);*/

	// 3.2 Création d'aucune tâche :
	//

	// 3.3 Création d'une tâche apériodique infinie (préemptée et relancée au dépit des autres)
	/*cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueD, &paramsD);
	cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA);*/

	// 3.4 Création avec différents arguments
	/*cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA);
	cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA2);
	cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA3);*/


	printf("---> Lancement des tâches périodiques \n");
	create_task_periodic();

	// L'objectif de cette tâche est de servir de serveur de gestion de tâches apériodiques
	while (1) {
		aperodique_file_active_suivant();
	}
}


void create_task_periodic() {
	uint16_t id;

	id = 3;
	_noyau_tcb_add[id].Nb_tour = 5;
	_noyau_tcb_add[id].wait_time = 100;
	active(cree(tacheGen, id,  (void*)&_noyau_tcb_add[id] ));

	id = 8;
	_noyau_tcb_add[id].Nb_tour = 2;
	_noyau_tcb_add[id].wait_time = 50;
	active(cree(tacheGen, id,  (void*)&_noyau_tcb_add[id] ));

}

TACHE	tacheGen(void)
{
	volatile NOYAU_TCB* p_tcb = NULL;
	volatile uint16_t id_tache;
	uint16_t i, j=1;


	id_tache = noyau_get_tc();
	p_tcb = noyau_get_p_tcb(id_tache);

	volatile uint16_t Nb_tour = ((NOYAU_TCB_ADD*)(p_tcb->tcb_add))->Nb_tour;
	volatile uint16_t wait_time = ((NOYAU_TCB_ADD*)(p_tcb->tcb_add))->wait_time;


	// 2. Instanciation de tâches apériodiques dans les tâches périodiques


	// 2.1 Deux tâches de paramètres différents instantanées
	int a = 1, b = 2;
	void *args[] = {&a, &b};
	TACHE_APERIODIQUE_PARAMS paramsA = {args, 2};

	int c = 8, d = 3;
	void *args2[] = {&c, &d};
	TACHE_APERIODIQUE_PARAMS paramsA2 = {args2, 2};


	// 2.2 Une tâche infinie

	// 2.2 Tache D
    void *argsD[] = {NULL};
	TACHE_APERIODIQUE_PARAMS paramsD = {argsD, 0};

	//cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueD, &paramsD);

	// on laisse du temps à la tâche de fond de démarrer toutes les tâches
	delay_n_ticks(20);
	while(1){
		cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA);
		cree_tache_aperiodique((void (*)(void *))tache_aperiodiqueA, &paramsA2);
		while(tache_get_flag_tick(id_tache) != 0){
			_lock_();
			printf("%d : tour n° %d \n", id_tache, j);
			if (j >= Nb_tour){
					j = 1;
					delay_n_ticks(wait_time);
			} else {
					j++;
			}
			_unlock_();
			tache_reset_flag_tick(id_tache);
		}
  	  }
}


int main()
{
	usart_init(115200);
	CLEAR_SCREEN(1);
    SET_CURSOR_POSITION(1,1);
    test_colors();
    CLEAR_SCREEN(1);
    SET_CURSOR_POSITION(1,1);
    puts("Test noyau");
    puts("Noyau preemptif");
    SET_CURSOR_POSITION(5,1);
    SAVE_CURSOR_POSITION();
	start(tachedefond);
	while(1) {}
  return(0);
}
