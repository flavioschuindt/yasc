/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Server's generic header                                  *
 *                                                           *
 *************************************************************/


#ifdef OWNER
#define EXTERN
#else
#define EXTERN extern	/* declares external variables */
#endif


/* SERVER settings *****************************/
/* !!! read before you change, or don't dare to say I got it wrong !!! */

#define MIN_WORKERS 1			/* can be set to 0 BUT has a side effect: slaveWork() is almost always  blocked *
								 * and if there were threads to kill while there were no clients at all, than   *
								 * the thread would only be killed when the conditional mutex would kick off    *
								 * (with a new client) thus killing a thread and launching one right afterwards */
#define MAX_WORKERS 500			/* neither the maximum nor the minimum may be reached for every possible combination *
								 * depending on CLIENTS_PER_SLAVE ratio and hysteresis                               */
#define MAX_CLIENTS 1000		/* control is not synchronous with the other threads;                                      *
								 * it's a read only operation where we can ignore race conditions for performance increase */
#define CLIENTS_PER_SLAVE ( MAX_CLIENTS / MAX_WORKERS )		/* integer division !!!                                                  *
                                                             * average number of clients per worker; soft rule: see other settings   *
															 * not every combination is good:                                        *
															 * there are combinations that 'resonate' for certain values of clients  *
															 * e.g. if this ratio = 2, set hysteresis to 1 at least                  *
															 * while others just don't make any sense                                */
#define POOL_REFRESH_RATE 1		/* [seconds] refresh rate for the pool size; use nanosleep() for finer control */
#define POOL_HYSTERESIS 2		/* hysteresis while upgrading the pool; note that the refresh rate also introduces hysteresis, but in an unspecified amount *
								 * represents a number of clients that may be added / removed without updating the pool                                     */
#define DOORMAN_DOZE 5			/* [seconds] period of inactivity of master when MAX_CLIENTS is reached; wakes up to see if he can allow anyone else in */
/************************************************/




/* Global Variables; remember to initialize */

EXTERN int VRB;						/* verbose flag */
EXTERN int number_of_workers;		/* number of slaves; global to allow printing statistics directly without pipes, etc. */
EXTERN pthread_t poolManager, serverManager, master_pthread_t;	/* important threads IDs */

EXTERN CLIENTS_DESCRIPTOR clients_desc;

EXTERN sigset_t soft_kill_set;		/* mask for signals; common for every thread */

EXTERN pthread_mutex_t p_mutex;		/* MUTEX to control access to protected resources */
EXTERN pthread_cond_t  p_cond_var;	/* global condition variable for our program */




/* avoids warnings from missing sigthread.h */
#ifndef _BITS_SIGTHREAD_H

extern int pthread_kill (pthread_t __threadid, int __signo);
extern int pthread_sigmask (int __how,__const __sigset_t *__restrict __newmask,__sigset_t *__restrict __oldmask);

#endif
