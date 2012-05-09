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
int get_client ();

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
void handle_client ( int fd );


/* Math module functions */
int add (int a, int b);
int sub (int a, int b);
int mult (int a, int b);
int divide (int a, int b);
int remainder (int a, int b);


#endif
