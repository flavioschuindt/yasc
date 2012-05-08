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

/*Get and Update the first client to be served in the list of FDs*/
int getFDAndUpdateNextFD();

/*Remove a node (client) from the FDs List*/
void removeFD(int FDToBeSearched);

/*Add a node(client) in the FDs List*/
void addFD(int FDToBeAdded);

/* The 'hard work' is done here. */
void handleParticularFD(int fd);

/* Access requests' list and process a specific request got from the list */
void *processFDsListTask(void *data);

/* Math module functions */
int add (int a, int b);
int sub (int a, int b);
int mult (int a, int b);
int divide (int a, int b);
int remainder (int a, int b);


/*  slave work; handles requests from client  *
 *  Parameters:  socket FD and stack pointer  *
 *  Returns:     nothing                      */
void *handle_client ( void *fd );


/*  parses command line; admin interface      *
 *  Parameters:  nothing                      *
 *  Returns:     nothing                      */
void *parse_line ();


/*  manages number of slaves dynamically      *
 *  Parameters:  nothing                      *
 *  Returns:     nothing                      */
void *manage_pool ();

#endif
