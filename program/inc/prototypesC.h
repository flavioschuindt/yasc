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
 *  Returns:     [ number of valid commands , number of errors ]                                    */
void parse_line (char *string);




#endif
