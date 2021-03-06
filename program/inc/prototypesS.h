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


void master_switch ();


/*  parses command line; admin interface      *
 *  Parameters:  nothing                      *
 *  Returns:     nothing                      */
void *parse_line ();


/*  prints info for each client               */
void print_client_info ();


/*  manages number of slaves dynamically      *
 *  Parameters:  nothing                      *
 *  Returns:     nothing                      */
void *manage_pool ();


/* Get and Update the first client to be served in the list of clients */
CLIENT get_client ();


/* Add a node(client) in the clients List */
void add_client ( int FD, char *IP );


/*Remove a node (client) from the clients List*/
void remove_client ( int client_fd );


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


/* Commands handler module functions; commands not listed are handled directly */
PACKAGE mountResponsePackage(char status, int response, PACKAGE outPackage);
PACKAGE cmd_D(int operand[1], STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage);
PACKAGE cmd_T(STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage);
PACKAGE cmd_add(STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage);
PACKAGE cmd_sub(STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage);
PACKAGE cmd_div(STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage);
PACKAGE cmd_mult(STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage);
PACKAGE cmd_reminder(STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage);
PACKAGE cmd_R(STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage);
void reorganizeStack(STACK_DESCRIPTOR *stack_desc, int newValue);
void resetStack (STACK_DESCRIPTOR *stack_desc);


#endif
