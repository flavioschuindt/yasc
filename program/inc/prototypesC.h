/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Client functions' prototypes                             *
 *                                                           *
 *************************************************************/

#ifndef __PROTOTYPESC_H__		/* include guard */
#define __PROTOTYPESC_H__


/*  Parses a command line and calls necessary functions to handle commands as they are found valid  *
 *  Parameters:  string from shell or from .txt                                                     *
 *  Returns:     nothing                                                                            */
void parse_line (char *string);


/*  Sends a command to the server; waits for answer *
 *  Parameters:  command to be sent                 *
 *  Returns:     nothing                            */
void handleRequest ( char Req, int Data );

/*  Tries to establish connection; waits for confirmation *
 *  Parameters:  nothing                                  *
 *  Returns:     nothing                                  */
void init_session ();

/*  Terminates connection; doesn't wait for server confirmation *
 *  Parameters:  nothing                                        *
 *  Returns:     nothing                                        */
void end_session ();

#endif
