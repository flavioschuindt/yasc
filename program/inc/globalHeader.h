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


/* CALLOC */
#define CALL(p,n) { \
	if ( ! ((p) = calloc((unsigned) (n), sizeof(*(p)))) ) { \
		fprintf(stderr, ">> ERROR: could not allocate memory.\n>> Aborting.\n"); \
		exit(-1); \
	} \
}

/* MALLOC */
#define MALL(p,n) { \
	if ( ! ((p) = malloc(n * sizeof(*(p)))) ) { \
		fprintf(stderr, ">> ERROR: could not allocate memory.\n>> Aborting.\n"); \
		exit(-1); \
	} \
}

/* THREAD CREATE */
#define PTH_CREATE(thrd,func,pnt) { \
	if ( (pthread_create((thrd), NULL, (func), (void *)(pnt))) ) { \
		fprintf(stderr,">> ERROR: could not create thread.\n>> Aborting.\n"); \
		exit(-1); \
	} \
}

/* THREAD JOIN */
#define PTH_JOIN(thrd,pntpnt) { \
	if ( (pthread_join((thrd),(pntpnt))) ) { \
		fprintf(stderr,">> ERROR: could not join thread.\n>> Aborting.\n"); \
		exit(-1); \
	} \
}


#define MAX_LINE 256	/* maximum length for a command line */
#define COM_SIZE 9 /* (bytes); size of communication package */
