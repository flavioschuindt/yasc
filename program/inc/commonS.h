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


/* Global defines & macros */

#define MIN_WORKERS 1
#define MAX_WORKERS 20
#define MAX_CLIENTS 1		/* soft limit (needs to be tested) due to lack of synchronization there may be a few more */


/* Global Variables; remember to initialize */

EXTERN CLIENTS_DESCRIPTOR *clients_desc;
EXTERN CLIENT *client;

EXTERN pthread_mutex_t p_mutex;	/* MUTEX to control access to protected resources */
EXTERN pthread_cond_t  p_cond_var; /* global condition variable for our program */
