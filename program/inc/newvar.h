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

#define NUM_HANDLER_THREADS 2

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

#endif
