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

#define MIN_WORKERS 5
#define MAX_WORKERS 20


/* Global Variables; remember to initialize */

EXTERN REQUESTS_DESCRIPTOR req_desc;
EXTERN STACK_DESCRIPTOR stack_desc;

EXTERN pthread_mutex_t request_mutex;	/* MUTEX to control access to protected resources */
EXTERN pthread_cond_t  got_request;		/* global condition variable for our program */
