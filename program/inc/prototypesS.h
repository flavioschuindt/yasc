/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Server functions' prototypes                             *
 *                                                           *
 *************************************************************/

#ifndef __PROTOTYPESS_H__		/* include guard */
#define __PROTOTYPESS_H__

#include <newvar.h>

/* Create initial server conditions updating the descriptor node */
void createInitialServerConditions();

/* Create a request, link it to the requests linked list and update number of requests */
void addRequest(unsigned char param1, unsigned char param2, pthread_mutex_t *p_mutex, pthread_cond_t *p_cond_var);

/* Get the first request on the list and returns it */
REQUEST *getRequest(pthread_mutex_t *p_mutex);

/* The 'hard work' is done here. Parse the command bytes and execute the operations needed. */
void handleParticularRequest(REQUEST *req);

/* Access requests' list and process a specific request got from the list */
void *processRequestsListTask(void *data);

/* Math module functions */
int add (int a, int b);

int sub (int a, int b);

int mult (int a, int b);

float divide (int a, int b);

#endif
