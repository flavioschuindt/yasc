/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  YASC Client  v0.1                                        *
 *     order independent options                             *
 *      -s name port      specify server                     *
 *      -f "*.txt"        open batch file                    *
 *                          (txt extension not enforced)     *
 *      -g                start in debug mode                *
 *      -l                log: suppress output               *
 *                          (generates log.txt)              *
 *                                                           *
 *************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define OWNER	/* owner of global variables */
#include <commonC.h>
#include <prototypesC.h>



int main( int argc, char *argv[] ) {


	int i=1;
	char *fileName, line[MAX_LINE];
	time_t now;
	struct tm *local;
	FILE *fin=stdin;	/* defaults to shell */

	fout = stdout;		/* defaults to shell */
	DBG = 0;


	/* argument parsing; setup */
	if( argc > 1 ) {

		while( i < argc ) {

			/* -s */
			if( strcmp(argv[i],"-s") == 0 ) {		/* UNDER CONSTRUCTION */

/************** name & port !!!*/
				if( 0 ) {
					fprintf(stdout,">> Setup error!\n>> Can't reach %s at %s.\n", argv[i+1], argv[i+2]);
				} else {
					i+=2;	/* jumps the name and port argument; but if not if the file is not valid */
				}

			/* -f */
			} else if( strcmp(argv[i],"-f") == 0 ) {		/* if more than one file is passed as argument, only the last one is taken into account */
				fin = fopen(argv[i+1],"r");
				if( fin == NULL ) {
					fprintf(stdout,">> Setup error!\n>> File \"%s\" not found.\n", argv[i+1]);
				} else {
					fileName = argv[i+1];
					i++;	/* jumps the name argument; but not if the file isn't valid */
				}

			/* -l */
			} else if( strcmp(argv[i],"-l") == 0 ) {		/* repeated use of the flag won't undo the effect; not very useful if the command line is used, but possible nonetheless */
				fout = fopen("log.txt","a");
				if( fout == NULL ) {
					fprintf(stdout,">> Setup error!\n>> Can't open log file.\n>> Redirecting output to \"stdout\"");
					fout = stdout;
				}

			/* -g */
			} else if( strcmp(argv[i],"-g") == 0 ) {		/* repeated use of the flag won't undo the effect */
				DBG=1;

			/* invalid argument */
			} else {
				fprintf(stdout,">> Setup error!\n>> Invalid argument \"%s\" ignored.\n", argv[i]);
			}

			i++;
		}
	}
/**************************** if bad hostname or port, ask for other */

	if( fout != stdout ) {
		now = time((time_t *)NULL);
		local = localtime(&now);
		fprintf(fout,"=====================================\n");
		fprintf(fout," Logging output for \"%s\"\n",fileName);
		fprintf(fout," %d/%d/%d\t--\t%d:%d:%d\n", local->tm_mday, local->tm_mon, local->tm_year + 1900, local->tm_hour, local->tm_min, local->tm_sec);
		fprintf(fout,"=====================================\n\n");
	}
	if( DBG & 1 ) {
		fprintf(fout,DBG_ON);
	}


	/* reads successive lines from source; repeats at the end of each instruction */
	while( fgets(line,MAX_LINE,fin) != NULL ) {
		parse_line(line);
	}


	return 0;
}
