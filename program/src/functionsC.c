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
#include <errno.h>
#include <limits.h>
#include <unistd.h>

#ifdef OWNER
#undef OWNER
#endif
#include <commonC.h>

#include <prototypesC.h>
#include <globalHeader.h>
#include <newvar.h>


void parse_line (char *string){

	int num, caret=0;
	char parameter[MAX_LINE];	/* if parameter is set as smaller than MAX_LINE, it is necessary to implement buffer overrun protection !!! */
	char *endptr, *remainingString=string;


	while( sscanf(remainingString,"%s%n",parameter,&caret) > 0 ) {
		remainingString += caret;	/* moves along the line */

		/* Evaluates parameter; if it is a number, 'D' command is implicit */
		errno = 0;		/* only way of checking over / underflow */
		num = (int) strtol(parameter,&endptr,0);

		/*
		 * CHECK FOR NUMBER
		 */
		if( errno == ERANGE ) {	/* condition of over / underflow */
			fprintf(fout,">> Parsing error!\n>> Number out of range. Use only integers between %ld and %ld.\n>> Try help for assistance.\n",LONG_MIN,LONG_MAX);

		} else if( *endptr == '\0' ) {	/* C99 implies that if it isn't ERANGE, it's 0 */

			handleRequest( 'D', num );	/* communicates with server */

		/* rejects number if it isn't surrounded by white-space exceptions are: '+' and '-' before the number */
		} else if( (*endptr != '\0') && (endptr != parameter) ) {
			fprintf(fout,">> Parsing error!\n>> Invalid number. Use only integers between %ld and %ld.\n>> Try help for assistance.\n",LONG_MIN,LONG_MAX);

		/*
		 * IF IT IS NOT A NUMBER
		 */
		} else if(  (!strcmp(parameter,"+")) || (!strcmp(parameter,"-")) || (!strcmp(parameter,"*")) ||
					(!strcmp(parameter,"/")) || (!strcmp(parameter,"%")) || (!strcmp(parameter,"R")) ||
					(!strcmp(parameter,"T")) || (!strcmp(parameter,"P")) || (!strcmp(parameter,"I")) || (!strcmp(parameter,"K"))  ) {

			handleRequest( parameter[0], 0 );	/* communicates with server */

		} else if( parameter[0] == ';' ) {	/* end of effective commands (anything past that is interpreted as comments until next NL) */
			break;

		} else if( !strcmp(parameter,"G") ) {
			DBG++;		/* test is done with bitwise AND; true for DBG odd, false for even */
			if( DBG & 1 ) {
				fprintf(fout,DBG_ON);
			} else {
				fprintf(fout,DBG_OFF);
			}

		} else if( !strcmp(parameter,"help") ) {
			fprintf(fout,">> No soup for you!\n");
			/* open help pages                          *************		    TO DO             *************/

		} else if( !strcmp(parameter,"exit") ) {
			fprintf(fout,">> NEXT!\n\n\n");
			exit(0);
			/* check if there is an open session; prompt to close it  (how ?)        *************		    TO DO             *************/

		} else {
			fprintf(fout,">> Parsing error!\n>> Unknown command \"%s\" ignored.\n>> Try help for assistance.\n",parameter);
		}
	}
}


void handleRequest ( char Req, int Data ) {
	unsigned int *returningData;
	PACKAGE outPackage, inPackage;

	MALL(returningData,1);
	outPackage.msg = Req;
	sprintf(outPackage.num,"%X",Data);

	/* synchronous handling of requests; read() blocks the client until there is an answer to read */
	write(clientSocket,(void *)&outPackage,COM_SIZE);
	read(clientSocket,(void *)&inPackage,COM_SIZE);
	sscanf(inPackage.num,"%X",returningData);	/* converts string (hexadecimal integer) to normal integer */

	if( DBG & 1 ) {

		if( Req == 'D' ) {
			fprintf(fout, "DEBUG:\t%c%d=> : =>%c\n", Req, Data, inPackage.msg);

		} else if( (Req == 'R') || (Req == 'T') || (Req == 'P') ){
			fprintf(fout, "DEBUG:\t%c=> : =>%c%d\n", Req, inPackage.msg, *returningData);

		} else {
			fprintf(fout, "DEBUG:\t%c=> : =>%c\n", Req, inPackage.msg);
		}

	} else if( ((Req == 'R') || (Req == 'T') || (Req == 'P')) && (inPackage.msg == 'V') ) {
		fprintf(fout, ":: %d\n", *returningData);

	} else if( inPackage.msg == 'E' ) {
		fprintf(fout, ":: Error!\n:: \"%c\" Command failed.\n", Req);

	} else if( inPackage.msg == 'I' ) {
		fprintf(fout, ":: Server error!\n:: Stack reinitialized.\n");
	}
}
