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


/* communication unit */
typedef struct package {
	char msg;		/* message / command */
	char num[8];	/* number or padding */
} PACKAGE;


/* clients whose sessions is already opened */
typedef struct client {
	int fd; /* socket fd for a specific client */
	struct client *next;
	struct client *previous;
	struct stack_descriptor *stack_desc;
} CLIENT;


/* A descriptor to control the "ring buffer", containing a pointer to the next client to be served and a counter that represents
   the total number of clients with session opened with the server*/
typedef struct clients_descriptor {
	struct client *first; /*This is the first client with session already opened*/
	struct client *last; /*This is the last client with session already opened*/
	struct client *next; /*This is the next client to be served*/
	int count;
} CLIENTS_DESCRIPTOR;


/* Operand Stack; one per client */
typedef struct stack_element {
	int operand; /*An operand in the stack*/
	struct stack_element *next;
} STACK_ELEMENT;


/* Stack status; one per client */
typedef struct stack_descriptor {
	struct stack_element *first;
	int count;
} STACK_DESCRIPTOR;


#endif
