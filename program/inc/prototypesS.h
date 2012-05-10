/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Server functions' prototypes                             *
 *                                                           *
 *************************************************************/

#ifndef __PROTOTYPESS_H__		/* include guard */
#define __PROTOTYPESS_H__

#include <newvar.h>

/* Create initial server conditions updating the descriptor node */
void createInitialServerConditions();


/*  parses command line; admin interface      *
 *  Parameters:  nothing                      *
 *  Returns:     nothing                      */
void *parse_line ();


/*  manages number of slaves dynamically      *
 *  Parameters:  nothing                      *
 *  Returns:     nothing                      */
void *manage_pool ();


/*Get and Update the first client to be served in the list of FDs*/
CLIENT get_client ();

/*Add a node(client) in the FDs List*/
void add_client ( int FDToBeAdded );

/*Remove a node (client) from the FDs List*/
void remove_client ( int FDToBeSearched );

/*  creates a stack associated with a particular socket (i.e. client) *
 *  Parameters:  nothing                                              *
 *  Returns:     nothing                                              */
void create_stack ( int fd );

/* Access requests' list and process a specific request got from the list */
void *slaveWork();

/*  slave work; handles requests from client  *
 *  Parameters:  socket FD and stack pointer  *
 *  Returns:     nothing                      */
void handle_client ( CLIENT client );


/* Math module functions */
int add (int a, int b);
int sub (int a, int b);
int mult (int a, int b);
int divide (int a, int b);
int remainder (int a, int b);

/* Commands handler module functions*/
PACKAGE mountResponsePackage(char status, int response, PACKAGE outPackage);
void cmd_D(int operand[1], STACK_DESCRIPTOR *stack_desc);
int cmd_T(STACK_DESCRIPTOR *stack_desc);
int cmd_P(STACK_DESCRIPTOR *stack_desc);
void cmd_add(STACK_DESCRIPTOR *stack_desc);
void cmd_sub(STACK_DESCRIPTOR *stack_desc);
void cmd_div(STACK_DESCRIPTOR *stack_desc);
void cmd_mult(STACK_DESCRIPTOR *stack_desc);
void cmd_reminder(STACK_DESCRIPTOR *stack_desc);
void reorganizeStack(STACK_DESCRIPTOR *stack_desc, int newValue);
int cmd_R(STACK_DESCRIPTOR *stack_desc);

#endif
