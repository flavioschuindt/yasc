/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  global Header                                            *
 *                                                           *
 *************************************************************/


/* MALLOC */
#define MALL(p,n) { \
	if ( ! ((p) = malloc(n * sizeof(*(p)))) ) { \
		fprintf(stderr, ">> ERROR: could not allocate memory.\n>> Aborting.\n"); \
		exit(-1); \
	} \
}

/* THREAD CREATE */
#define PTH_CREATE(thrd,func,pnt) { \
	if ( (pthread_create((thrd), NULL, (func), (void *)(pnt))) != 0 ) { \
		fprintf(stderr,">> ERROR: could not create thread.\n>> Aborting.\n"); \
		exit(-1); \
	} \
}

/* THREAD JOIN */
#define PTH_JOIN(thrd,pntpnt) { \
	if ( (pthread_join((thrd),(pntpnt))) != 0 ) { \
		fprintf(stderr,">> ERROR: could not join thread.\n>> Aborting.\n"); \
		exit(-1); \
	} \
}

/* THREAD DETACH */
#define PTH_DTCH(thrd) { \
	if ( (pthread_detach(thrd)) != 0 ) { \
		fprintf(stderr,">> ERROR: could not join thread.\n>> Aborting.\n"); \
		exit(-1); \
	} \
}


#define MAX_LINE 256	/* maximum length for a command line */
#define COM_SIZE 9 /* (bytes); size of communication package */


/* Server codes */
#define OK 0				/* no error */
#define BAD_CMD -1			/* commend not identified */
#define OUT_OF_RANGE 255	/* over/underflow */
#define BIG_STACK 555		/* stack is bigger than expected */
#define BAD_STACK 999		/* not enough operands for specified action */
#define DIV_0 666			/* division by 0 */
