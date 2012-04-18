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
 *                                                           *
 *      -s name port      specify server                     *
 *      -f "*.txt"        open batch file                    *
 *                          (txt extension not enforced)     *
 *      -g                start in debug mode                *
 *      -q                quiet: suppress output             *
 *                          (generates log.txt)              *
 *                                                           *
 *************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define OWNER	/* owner of global variables */
#include <commonC.h>
#include <prototypesC.h>



#define add(m1,m2) ((m1)+(m2))			/* these will go to the server source */
#define subtract(m1,m2) ((m1)-(m2))
#define multiply(m1,m2) ((m1)*(m2))
#define divide(m1,m2) ((m1)/(m2))
#define remainder(m1,m2) ((m1)%(m2))



int main( int argc, char *argv[] ) {


	int i, lineNumber;
	char *fileName, line[MAX_LINE];
	FILE *fin=stdin;

	fout = stdout;
	DBG = 0;


	/* Handle of Flags */
	if( argc >= 1 ) {

		for(i=0;i<argc;i++) {

			if( strcmp(argv[i],"-f") == 0 ) {				/* -f */
				fin = fopen(argv[i+1],"r");
				if( fin == NULL ) {
					fprintf(fout,">> ERROR: \"%s\" File not found.", argv[i+1]);
				} else {
					fileName = argv[i+1];
					i++;		/* jumps the name argument; but if not if the file is not valid */
				}

			} else if( strcmp(argv[i],"-q") == 0 ) {		/* -q */
				fout = fopen("log.txt","a");
				fprintf(fout,">> Logging output for \"%s\"\n",fileName);

			} else if( strcmp(argv[i],"-g") == 0 ) {		/* -g */
				DBG=1;		/* repeated use of the flag won't undo the effect */
				fprintf(fout,DBG_ON);
			}

		}
	}


	lineNumber=0;
	while( fgets(line,MAX_LINE,fin) != NULL ) {    /* reads successive lines from source; repeats at the end of each instruction */

		parse_line(line);
		lineNumber++;
	}


	/* only runs if a file was provided */
	/*fprintf(fout,"________________________________________________________________\n>> Processed %d lines tottaling %d commands and %d errors\n", lineNumber, stat[0], stat[1]);*/


	/*main(0,NULL);*/	/* recursive call keeps program running in command line mode after processing a .txt */

	return 0;
}



/*TO DO: changes stderr descriptor for a .log file to use in debug mode (? implementation inside parse_line ?)
			use  strrchr( )  to enforce .txt  */
