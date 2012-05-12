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


/* Global defines & macros **********************/

/* SERVER settings */
#define MIN_WORKERS 2
#define MAX_WORKERS 10
#define MAX_CLIENTS 10			/* soft limit (needs to be tested) due to lack of synchronization there may be a few more */
#define POOL_REFRESH_RATE 5		/* [seconds] refresh rate for the pool size */
#define POOL_HYSTERESIS 0		/* hysteresis while upgrading the pool; note that the refresh rate also introduces hysteresis, but in an unspecified amount *
								 * represents a number of clients that may be added / removed without updating the pool                                     */
#define CLIENTS_PER_SLAVE ( MAX_CLIENTS / MAX_WORKERS )		/* average number of clients per worker; soft rule: see other settings */
#define DOORMAN_DOZE 5			/* [seconds] period of inactivity of master when MAX_CLIENTS is reached; wakes up to see if he can allow anyone else in */


/* Global Variables; remember to initialize */

EXTERN CLIENTS_DESCRIPTOR clients_desc;

EXTERN pthread_t master_pthread_t;	/* master ID */
EXTERN pthread_mutex_t p_mutex;	/* MUTEX to control access to protected resources */
EXTERN pthread_cond_t  p_cond_var; /* global condition variable for our program */


/* to avoid warnings from missing sigthread.h; it generates other misleading warnings; don't uncomment */
/*extern int pthread_kill (pthread_t __threadid, int __signo);
extern int pthread_sigmask (int __how,__const __sigset_t *__restrict __newmask,__sigset_t *__restrict __oldmask);*/
