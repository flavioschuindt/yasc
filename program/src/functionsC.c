/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Client Functions' declaration                            *
 *                                                           *
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef OWNER
 #undef OWNER
#endif
#include <commonC.h>

#include <prototypesC.h>


void parse_line (char *string){

	int caret=0;
	char parameter[MAX_LINE], *remainingString=string;

	while( sscanf(remainingString,"%s%n",parameter,&caret) > 0 ) {
		remainingString += caret;	/* moves along the line */


		if( (strtol(parameter,NULL,0)) ){	/* D command is implicit and cannot be used directly; it's used whenever a number is detected */
				/***   check for good number argument; send D + number   *****/

		} else if( (!strcmp(parameter,"+")) || (!strcmp(parameter,"-")) || (!strcmp(parameter,"*")) ||
				   (!strcmp(parameter,"/")) || (!strcmp(parameter,"%")) || (!strcmp(parameter,"R")) ||
				   (!strcmp(parameter,"T")) || (!strcmp(parameter,"P")) || (!strcmp(parameter,"I")) || (!strcmp(parameter,"K"))     ) {

			/*******/

		} else if( !strcmp(parameter,"G") ) {
			DBG++;		/* test is done with bitwise AND; true for DBG odd, false for even */
			if( DBG & 1 ){
				fprintf(fout,DBG_ON);
			} else {
				fprintf(fout,DBG_OFF);
			}
		} else if( !strcmp(parameter,"exit") ) {
			fprintf(fout,">> NEXT!\n");
			exit(0);
			/********/
		} else if( !strcmp(parameter,"help") ) {
			fprintf(fout,">> No soup for you!\n");
			/********/
		} else if( !strcmp(parameter,";") ) {		/* end of line; interpreted as comments */
			break;
		} else {
			fprintf(fout,">> Unknown command. Discarded.\n>> Try help for assistance.\n");
		}

	}

}

