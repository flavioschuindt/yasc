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

/* Requests to be processed */
typedef struct request {
	unsigned char param1;	/*First byte of message*/
	unsigned char param2;	/*Second byte of message*/
	/* client ID; some sort of stack identification; maybe a pointer to the appropriate stack*/
	struct request *next;	/*It's a linked list of requests, so we need to point to the next*/
} REQUEST;

/* Requests state */
typedef struct requests_descriptor {
	REQUEST *first;
	REQUEST *last;
	int num_requests;
} REQUESTS_DESCRIPTOR;

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
