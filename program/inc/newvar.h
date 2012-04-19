/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Server functions' new variables                          *
 *                                                           *
 *************************************************************/

#ifndef __NEWVARS_H__
#define __NEWVARS_H__

#define WORKERS 1

typedef struct 
{
	unsigned char param1;  /*First byte of message*/
	unsigned char param2; /*Second byte of message*/
	struct request *next;  /*It's a linked list of requests, so we need to point to the next*/
}request;

typedef struct
{
	request *first;
	request *last;
	int num_requests;
}requests_descriptor;

requests_descriptor *req_desc;

typedef struct
{
	unsigned char operand; /*An operand in the stack*/
	struct stack_element *next;
}stack_element;

typedef struct
{
	stack_element *first;
	int count;
}stack_descriptor;

stack_descriptor *stack_desc;

#endif