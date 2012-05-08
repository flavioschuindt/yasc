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


/* Global defines & macros */




/* Global Variables; remember to initialize */

EXTERN int DBG;				/* Flag for Debug mode; 'ON' for DBG odd, 'OFF' for DBG even */
EXTERN FILE *fout;			/* Output file descriptor */
EXTERN int clientSocket;	/* File descriptor for the client socket */
EXTERN struct addrinfo *server, *pntAddr;
EXTERN struct addrinfo hints;
EXTERN char *name, *port;
