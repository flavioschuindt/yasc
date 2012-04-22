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

/* request to be processed */
typedef struct request {
	unsigned char param1;	/*First byte of message*/
	unsigned char param2;	/*Second byte of message*/
	/* client ID; some sort of stack identification */
	struct request *next;	/*It's a linked list of requests, so we need to point to the next*/
} REQUEST;

/* Requests state */
typedef struct requests_descriptor {
	REQUEST *first;
	REQUEST *last;
	int num_requests;
} REQUESTS_DESCRIPTOR;


typedef struct stack_element {
	unsigned char operand; /*An operand in the stack*/
	struct stack_element *next;
} STACK_ELEMENT;


typedef struct stack_descriptor {
	STACK_ELEMENT *first;
	int count;
} STACK_DESCRIPTOR;


#endif