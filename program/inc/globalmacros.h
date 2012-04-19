/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  global macros                                            *
 *                                                           *
 *************************************************************/


/* CALLOC */
#define CALL(p,n) { \
	if ( ! ((p) = calloc((unsigned) (n), sizeof(*(p)))) ) { \
		fprintf(stderr, "ERROR: could not allocate memory at %d of %s\n", __LINE__, __FILE__); \
		exit(-1); \
	} \
}

/* MALLOC */
#define MALL(p,n) { \
	if ( ! ((p) = malloc(n * sizeof(*(p)))) ) { \
		fprintf(stderr, "ERROR: could not allocate memory at %d of %s\n", __LINE__, __FILE__); \
		exit(-1); \
	} \
}

/* THREAD CREATE */
#define PTH_CREATE(thrd,func,pnt) { \
	if ( (pthread_create((thrd), NULL, (func), (void *)(pnt))) ) { \
		printf("ERROR: could not create thread %d\t( %d of %s )\n", i, __LINE__, __FILE__); \
	} \
}

/* THREAD JOIN */
#define PTH_JOIN(thrd,pntpnt) { \
	if ( (pthread_join((thrd),(pntpnt))) ) { \
		printf("ERROR: could not join thread %d\t( %d of %s )\n", i, __LINE__, __FILE__); \
	} \
}
