/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Client's generic header                                  *
 *                                                           *
 *************************************************************/


#ifdef OWNER
#define EXTERN
#else
#define EXTERN extern	/* declares external variables */
#endif


/* CLIENT settings *****************************/

#define TIME_OUT 5	/* [seconds] idle time necessary to close session automatically */
#define TIME_OUT_MSG ":: Idle for 5 minute.\n:: Closed session.\n"

/***********************************************/


/* Global Variables; remember to initialize */

EXTERN int DBG;				/* Flag for Debug mode; 'ON' for DBG odd, 'OFF' for DBG even */
EXTERN int init;			/* flag for session state: 0 -> no session; 1 -> there is a session already */
EXTERN int clientSocket;	/* File descriptor for the client socket */
EXTERN char *name, *port;
EXTERN FILE *fout;			/* Output file descriptor */
EXTERN struct addrinfo *server, *pntAddr;
EXTERN struct addrinfo hints;

