/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  New variables                                            *
 *                                                           *
 *************************************************************/

#ifndef __NEWVAR_H__
#define __NEWVAR_H__

typedef struct package {
	char msg;		/* message / command */
	char num[8];	/* number or padding */
} PACKAGE;

/* clients whose sessions is already opened */
typedef struct fd {
	int FD; /*socket for a specific client*/
	struct fd *next;
	struct fd *previous;
} FD;

/* A descriptor to control the "ring buffer", containing a pointer to the next client to be served and a counter that represents
   the total number of clients with session opened with the server*/
typedef struct fds_descriptor {
	FD *first; /*This is the first client with session already opened*/
	FD *last; /*This is the last client with session already opened*/
	FD *next; /*This is the next client to be served*/
	int count;
} FDs_DESCRIPTOR;

/* Operand Stack; one per client */
typedef struct stack_element {
	unsigned char operand; /*An operand in the stack*/
	struct stack_element *next;
} STACK_ELEMENT;

/* Stack status; one per client */
typedef struct stack_descriptor {
	STACK_ELEMENT *first;
	int count;
} STACK_DESCRIPTOR;


#endif
