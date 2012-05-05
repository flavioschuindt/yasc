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

#define MAX_LINE 256	/* maximum length for a command line; currently it's not checked to ensure the health of a command*/
#define DBG_ON ">> Debug mode ON\n"
#define DBG_OFF ">> Debug mode OFF\n"


/* Global Variables; remember to initialize */

EXTERN int DBG;				/* Flag for Debug mode; 'ON' for DBG odd, 'OFF' for DBG even */
EXTERN FILE *fout;			/* Output file descriptor */
EXTERN int clientSocket;	/* File descriptor for the client socket */
